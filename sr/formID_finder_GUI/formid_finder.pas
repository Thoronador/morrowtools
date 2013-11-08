{
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

    This programme is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This programme is distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this programme.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
}

unit formID_finder;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, LResources, Forms, Controls, Graphics, Dialogs,
{$IFDEF Windows }
  Windows, Messages,
{$ELSE}
  BaseUnix, UnixType, FinderGUIUDSServer,
{$ENDIF}
  StdCtrls, Grids, Menus, LConvEncoding;

type

{$IFDEF Windows }
  TWMCopyData = packed record
    Msg: Cardinal;
    From: HWND;
    CopyDataStruct: PCopyDataStruct;
    Result: Longint;
  end;
{$ENDIF}

  { TForm1 }

  TForm1 = class(TForm)
    MainMenu1: TMainMenu;
    MenuItemVersion: TMenuItem;
    MenuItemClose: TMenuItem;
    MenuItemGUI: TMenuItem;
    MenuItemHelp: TMenuItem;
    SearchButton: TButton;
    KeywordEdit: TEdit;
    ResultStringGrid: TStringGrid;
    procedure FormCreate(Sender: TObject);
    procedure KeywordEditKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure MenuItemCloseClick(Sender: TObject);
    procedure MenuItemVersionClick(Sender: TObject);
    procedure SearchButtonClick(Sender: TObject);
  private
    { private declarations }
    m_ExpectsData: Boolean;
    m_HasData: Boolean;
    m_StringData: AnsiString;
    {$IFDEF WINDOWS }
    procedure WMCopyData(var Msg: TWMCopyData); message WM_COPYDATA;
    {$ENDIF}
    procedure DataToStringGrid(var total: Integer);
    procedure CleanUpGrid(const info: string);
  public
    { public declarations }
  end;

  function escapeKeyword(const str1: string): string;

const
{$IFDEF WINDOWS }
  cProgrammeName: string = 'formID_finder.exe';
{$ELSE}
  cProgrammeName: string = 'formID_finder';
{$ENDIF}
  cMinRevision = 413;
  // All versions older than rev520 are considered as outdated.
  cFirstRevisionNotOutdated = 520;

var
  Form1: TForm1;
  {$IFDEF WINDOWS }
  //window proc
  m_PrevWndProc: WNDPROC;
  {$ENDIF}

implementation

{$IFDEF WINDOWS }
function WndCallback(Ahwnd: HWND; uMsg: UINT; wParam: WParam; lParam: LParam):LRESULT; stdcall;
var temp: TWMCopyData;

begin
  if uMsg=WM_COPYDATA then
  begin
    temp.From:= Ahwnd;
    temp.Msg:= uMsg;
    temp.Result:= 0;
    temp.CopyDataStruct:= Pointer(lParam);
    Form1.WMCopyData(temp);
    result:= temp.Result;
    exit;
  end
  else result:=CallWindowProc(m_PrevWndProc, Ahwnd, uMsg, WParam, LParam);
end;
{$ELSE}

//tries to spawn a child process and returns true on success, or false on
// failure. The child's process ID is stored in pid in case of success.
function spawnProcess(progName: AnsiString; args: PPChar; var pid: TPid): Boolean;
begin
  pid:= FpFork;
  //-1 means fork failed
  if (pid=-1) then
  begin
    Result:= false;
  end
  //non-zero means success, return value of fork() is child's pid
  else if (pid<>0) then Result:= true
  //zero means we are in the child process
  else begin
    FpExecv(progName, args);
    //if we get to this point, an error occured, because execv() does not return
    // in case of success
    Result:= false;
    Halt;
    Abort;
  end;//else
end;//func

{$ENDIF}

function GetFormIDFinderRevision: Cardinal;
{$IFDEF WINDOWS }
var si: TStartupInfo;
    pi: TProcessInformation;
    exitCode: DWORD;
{$ELSE}
const ArgOne: PChar = '--version-with-exitcode';
var   ArgArray: PPChar;
      pid, wait_pid_result: TPid;
      status: cint;
{$ENDIF}
begin
  {$IFDEF WINDOWS }
  FillChar(si, SizeOf(si), 0);
  with si do
  begin
    cb:= sizeof(si);
    dwFlags:= STARTF_USESHOWWINDOW;
    wShowWindow:= SW_HIDE;
  end;
  FillChar(pi, sizeof(pi), 0);
  // start the child process
  if(CreateProcess(nil, //no module name (use command line)
      PChar(cProgrammeName+' --version-with-exitcode'), //command line
      nil, //process handle not inheritable
      nil, //thread handle not inheritable
      FALSE, //set handle inheritance to false
      0, //creation flags
      nil, //use parent's environment block
      nil, //use parent's starting directory
      si, //pointer to STARTUPINFO structure
      pi) //pointer to PROCESS_INFORMATION structure
    <>false) then
  begin
    //process started successfully
    exitCode:= STILL_ACTIVE;
    repeat
      //wait half a second
      WaitForSingleObject(pi.hProcess, 500);
      //get exit code
      GetExitCodeProcess(pi.hProcess, @exitCode);
      //.. process messages to avoid application freeze
      Application.ProcessMessages;
    until (exitCode<>STILL_ACTIVE);
    //close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    Result:= exitCode;
  end
  else begin
    //create process failed
    Result:= 0;
  end;
  {$ELSE} //end of windows-specific code section, starting Linux code
  GetMem(ArgArray, 3*sizeof(PChar));
  ArgArray[0]:= PChar(cProgrammeName);
  ArgArray[1]:= ArgOne;
  ArgArray[2]:= nil;
  if (spawnProcess(cProgrammeName, ArgArray, pid)) then
  begin
    repeat
      wait_pid_result:= FpWaitPid(pid, status, WNOHANG);
      //.. process messages to avoid application freeze
      Application.ProcessMessages;
    until (wait_pid_result<>0);
    //-1 means error
    if (wait_pid_result=-1) then Result:= 0
    else begin
      if (wifexited(status)) then
      Result:= wexitstatus(status)
      else Result:= 0;
      //dirty hack to get the "real" exit code (Unix truncates to least 8 bits?)
      // -- will only work as expected for future revision numbers up to 511
      if ((Result>20) and (Result<256)) then Result:= Result+256;
    end;//else
  end//if
  else begin
    //could not create process
    Result:= 0;
  end;//else
  FreeMem(ArgArray);
  {$ENDIF}
end;//func

function CheckRevision: Boolean;
begin
  Result:= (GetFormIDFinderRevision>=cMinRevision);
end;//func

function CheckRevision(const rev: Cardinal): Boolean;
begin
  Result:= (rev>=cMinRevision);
end;//func

function RevisionToVersion(const rev: Cardinal): string;
begin
  case rev of
    0: Result:= 'unknown version';
    1..316: Result:= 'v0.01 or earlier';
    317..320: Result:= 'v0.02.rev'+IntToStr(rev);
    321..323: Result:= 'v0.03.rev'+IntToStr(rev);
    324..325: Result:= 'v0.04.rev'+IntToStr(rev);
    326: Result:= 'v0.05.rev326, 2011-12-13';
    327: Result:= 'v0.06.rev327, 2011-12-13';
    328: Result:= 'v0.07.rev328, 2011-12-14';
    329..344: Result:= 'v0.08.rev'+IntToStr(rev);
    345: Result:= 'v0.09.rev, 2011-12-22';
    346: Result:= 'v0.10.rev, 2011-12-22';
    347: Result:= 'v0.11.rev, 2011-12-23';
    348..370: Result:= 'v0.12.rev'+IntToStr(rev)+', 2011-12-25';
    371..392: Result:= 'v0.13.rev'+IntToStr(rev)+', 2012-01-05';
    393..399: Result:= 'v0.14.rev'+IntToStr(rev)+', 2012-01-16';
    400..403: Result:= 'v0.15.rev'+IntToStr(rev)+', 2012-01-17';
    404..412: Result:= 'v0.16.rev'+IntToStr(rev)+', 2012-01-22';
    413..420: Result:= 'v0.17.rev'+IntToStr(rev)+', 2012-02-03';
    421..442: Result:= 'v0.17.rev'+IntToStr(rev)+', 2012-02-19';
    443..453: Result:= 'v0.18.rev'+IntToStr(rev)+', 2012-03-15';
    454: Result:=  'v0.19_pre.rev454, 2012-03-25';
    455..459: Result:= 'v0.19.rev'+IntToStr(rev)+', 2012-03-25';
    460..471: Result:= 'v0.19.rev'+IntToStr(rev)+', 2012-03-31';
    472..491: Result:= 'v0.20.rev'+IntToStr(rev)+', 2012-06-12';
    492..508: Result:= 'v0.20b.rev492, 2012-10-15';
    509..511: Result:= 'v0.21.rev509, 2013-02-12';
    512: Result:= 'v0.22.rev512~experimental, 2013-02-21';
    513: Result:= 'v0.22.rev513~experimental, 2013-02-22';
    514: Result:= 'v0.22.rev514~experimental-2, 2013-02-24';
    515: Result:= 'v0.22b.rev515~experimental, 2013-02-25';
    516: Result:= 'v0.22c.rev516~experimental, 2013-02-26';
    517: Result:= 'v0.22d.rev517~experimental, 2013-02-27';
    518: Result:= 'v0.22e.rev518~experimental, 2013-02-28';
    519: Result:= 'v0.23.rev519~experimental, 2013-03-02';
    520: Result:= 'v0.24.rev520, 2013-03-14';
    521: Result:= 'v0.24b.rev521, 2013-03-19';
  else
    Result:= 'v0.24b or later';
  end;//case
end;//func

function RevisionIsExperimental(const rev: Cardinal): Boolean;
begin
  case rev of
    512..519: Result:= true;
  else
    Result:= false;
  end;//case
end;//func

function RevisionIsOutdated(const rev: Cardinal): Boolean;
begin
  { All versions older than rev520 are outdated (except 0, because that means no
    revision found. }
  Result:= ((rev<cFirstRevisionNotOutdated) and (rev<>0));
end;//func

function RunFormIDFinder(const keyword: string; var success: Boolean; const s1, s2: string; const rev: Cardinal): Cardinal;
{$IFDEF WINDOWS }
var si: TStartupInfo;
    pi: TProcessInformation;
    exitCode: DWORD;
    commandLine: AnsiString;
{$ELSE}
const ArgOne: PChar = '--keyword';
      ArgThree: PChar = '--send-data';
var   ArgArray: PPChar;
      pid, wait_pid_result: TPid;
      status: cint;
      server: TFinderGUIUDSServer;
{$ENDIF}
begin
  success:= false;
  {$IFDEF WINDOWS }
  FillChar(si, SizeOf(si), 0);
  with si do
  begin
    cb:= sizeof(si);
    dwFlags:= STARTF_USESHOWWINDOW;
    wShowWindow:= SW_HIDE;
  end;
  FillChar(pi, sizeof(pi), 0);
  // start the child process
  commandLine:= cProgrammeName+' --keyword "'+UTF8ToCP1252(escapeKeyword(keyword))
               +'" --send-data "'+s1+'" "'+s2+'"';
  { formID_finder from v0.24.rev520 on needs the parameter --show-file to show
    files, too, and recognize the calling programme as a newer GUI version. }
  if (rev>=520) then
  begin
    commandLine:= commandLine + ' --show-files';
  end;
  if(CreateProcess(nil, //no module name (use command line)
      PChar(commandLine), //command line
      nil, //process handle not inheritable
      nil, //thread handle not inheritable
      false, //set handle inheritance to false
      0, //creation flags
      nil, //use parent's environment block
      nil, //use parent's starting directory
      si, //pointer to STARTUPINFO structure
      pi) //pointer to PROCESS_INFORMATION structure
    <>false) then
  begin
    //process started successfully
    exitCode:= STILL_ACTIVE;
    repeat
      //wait quarter of a second
      WaitForSingleObject(pi.hProcess, 250);
      //get exit code
      GetExitCodeProcess(pi.hProcess, @exitCode);
      //.. process messages to avoid application freeze
      Application.ProcessMessages;
    until (exitCode<>STILL_ACTIVE);
    //close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    Result:= exitCode;
    success:= true;
  end
  else begin
    //create process failed
    success:= false;
    Result:= 0;
  end;
  {$ELSE}
  //TODO: not implemented on Linux yet
  if (not server.activate('./fid_uds_server')) then
  begin
    Result:= 0;
    success:= false;
    Exit;
  end;
  if (not server.makeNonBlocking) then
  begin
    Result:= 0;
    success:= false;
    Exit;
  end;
  server.startListening(1); //start listening for one ms to get server ready

  GetMem(ArgArray, 7*sizeof(PChar));
  ArgArray[0]:= PChar(cProgrammeName);
  ArgArray[1]:= ArgOne;
  ArgArray[2]:= PChar(UTF8ToCP1252(escapeKeyword(keyword)));
  ArgArray[3]:= ArgThree;
  ArgArray[4]:= 'socket';
  ArgArray[5]:= './fid_uds_server';
  ArgArray[6]:= nil;
  if (spawnProcess(cProgrammeName, ArgArray, pid)) then
  begin
    repeat
      wait_pid_result:= FpWaitPid(pid, status, WNOHANG);
      //.. process messages to avoid application freeze
      Application.ProcessMessages;
      server.startListening(125); //listen for 125 ms
      Application.ProcessMessages;
    until (wait_pid_result<>0);
    //-1 means error
    if (wait_pid_result=-1) then Result:= 0
    else begin
      if (wifexited(status)) then
      begin
        Result:= wexitstatus(status);
        success:= true;
      end
      else Result:= 0;
    end;//else
  end//if
  else begin
    //could not create process
    Result:= 0;
  end;//else
  FreeMem(ArgArray);
  {$ENDIF}
end;//func

function escapeKeyword(const str1: string): string;
var i, len: Integer;
begin
  Result:= str1;
  len:= length(str1);
  if (len>0) then
  begin
    i:= 1;
    while (i<=len) do
    begin
       if (Result[i]='"') then
       begin
         //add escaping backslash
         Result:= copy(Result, 1, i-1)+'\'+copy(Result, i, len-i+1);
         //length increased by one, so change len to reflect that
         len:= len+1;
         //increase i by one, so we don't run into that double quote sign again
         i:= i+1;
       end;//if double quote
    i:= i+1;
    end;//while
  end;//if
end;//function

function isTrimableChar(const c: char): Boolean;
begin
  Result:= ((c=' ') or (c=#13) or (c=#10) or (c=#9));
end;//func

procedure trim4(var str1: string);
var position: Integer;
    look: Integer;
    size: Integer;
begin
  position:= -1;
  look:= 1;
  size:= length(str1);
  while (look<=size) do
  begin
    if (isTrimableChar(str1[look])) then
      position:= look
    else
      break;
    look:= look+1;
  end;//while
  if (position > -1) then
  begin
    if (position<size) then str1:= Copy(str1, position+1, size)
    else str1:= '';
  end;//if

  position:= - 1;
  look:= length(str1);
  while (look>=1) do
  begin
    if (isTrimableChar(str1[look])) then
      position:= look
    else
      break;
    look:= look-1;
  end;//while
  if (position > -1) then
  begin
      if (position>1) then str1:= Copy(str1, 1, position-1)
      else str1:= '';
  end;//if
end;//proc

{ TForm1 }

procedure TForm1.CleanUpGrid(const info: string);
begin
  ResultStringGrid.RowCount:= 2;
  ResultStringGrid.Clean(0, 1, 5, 1, []);
  ResultStringGrid.Cells[0,1]:= info;
end;//proc

procedure TForm1.SearchButtonClick(Sender: TObject);
var rev, return: Cardinal;
    success: Boolean;
    errString: string;
    total: Integer;
begin
  KeywordEdit.Text:= trim(KeywordEdit.Text);
  if (length(KeywordEdit.Text)=0) then
  begin
    ShowMessage('Error: you have to enter a keyword before you can start the search!');
    Exit;
  end;//if
  if (not FileExists(cProgrammeName)) then
  begin
    ShowMessage('Error: file '+cProgrammeName+' was not found in current directory!');
    Exit;
  end;
  rev := GetFormIDFinderRevision;
  self.Caption := 'Form ID Finder - GUI (using '+RevisionToVersion(rev)+')';
  if (not CheckRevision(rev)) then
  begin
    ShowMessage('Error: You have an older, unsupported version of '
               +cProgrammeName+'! To use this programme properly, you need '
               +' version 0.17.rev413 or later.');
    Exit;
  end;//if
  SearchButton.Enabled:= false;
  //remove old results
  CleanUpGrid('Searching...');
  //run the programme
  CleanUpGrid('Starting formID_finder...');
  m_HasData:= false;
  m_ExpectsData:= true;
  return:= RunFormIDFinder(KeywordEdit.Text, success, 'NULL', self.Caption, rev);
  CleanUpGrid('formID_finder finished.');
  Application.ProcessMessages;
  m_ExpectsData:= false;
  //did the programme execution fail?
  if (not success) then
  begin
    CleanUpGrid('Could not start formID_finder.');
    Application.ProcessMessages;
    ShowMessage('Error while starting formID_finder!');
    SearchButton.Enabled:= true;
    Exit;
  end;//if
  CleanUpGrid('formID_finder executed successfully.');
  Application.ProcessMessages;
  //check return code
  if (return<>0) then
  begin
    errString:= 'Error while executing formID_finder! ';
    case return of
      1: errString:= errString+'Invalid parameters! If you get this error, '
                     +'this could be an indication, that you don''t have Skyrim'
                     +' installed on your machine. However, in that case you '
                     +'might not be able to make any use of that programme '
                     +'anyway!';
      2: errString:= errString+'File error! If you get this error, '
                     +'this could be an indication, that you don''t have Skyrim'
                     +' installed on your machine. However, in that case you '
                     +'might not be able to make much use of that programme '
                     +'anyway!';
      3: errString:= errString+'Data error!';
      10: errString:= errString+'formID_finder could not find GUI window!';
      11: errString:= errString+'formID_finder does not support sending data!';
      12: errString:= errString+'formID_finder could not send data via Unix '
                      +'domain socket!';
    else
      errString:= errString+'Unknown error! Error code is '+IntToStr(return)+'.';
    end;//case
    CleanUpGrid(errString);
    Application.ProcessMessages;
    ShowMessage(errString);
    SearchButton.Enabled:= true;
    Exit;
  end;//if return code<>0
  //now get the data
  if (not m_HasData) then
  begin
    CleanUpGrid('No data.');
    ShowMessage('No data recieved from formID_finder!');
    SearchButton.Enabled:= true;
    Exit;
  end;//if
  trim4(m_StringData);
  //ShowMessage('Trimmed recieved Content is:'+#13#10+'"'+m_StringData+'"');
  CleanUpGrid('Preparing data for display...');
  Application.ProcessMessages;
  DataToStringGrid(total);
  SearchButton.Enabled:= true;
end;//proc

procedure TForm1.FormCreate(Sender: TObject);
begin
  m_ExpectsData:= false;
  m_HasData:= false;
  m_StringData:= '';
  {$IFDEF WINDOWS }
  m_PrevWndProc:= Windows.WNDPROC(SetWindowLong(Self.Handle,GWL_WNDPROC,PtrInt(@WndCallback)));
  {$ENDIF}
  Application.Title:= self.Caption;
end;

procedure TForm1.KeywordEditKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if ((Key=13) and (SearchButton.Enabled)) then SearchButtonClick(Sender);
end;

procedure TForm1.MenuItemCloseClick(Sender: TObject);
begin
  Close;
end;

procedure TForm1.MenuItemVersionClick(Sender: TObject);
var str1: string;
    foundRev: Cardinal;
    allIsFine: Boolean;
begin
  allIsFine:= true;
  str1:= 'GUI version: rev521'+#13#10+cProgrammeName+' version: ';
  if (not FileExists(cProgrammeName)) then
  begin
    str1:= str1 + 'not found';
    foundRev:= 0;
    allIsFine:= false;
  end
  else begin
    foundRev:= GetFormIDFinderRevision;
    str1:= str1 + RevisionToVersion(foundRev);
  end;//else
  str1:= str1+#13#10#13#10+'Your version of '+cProgrammeName;
  if (not CheckRevision(foundRev)) then
  begin
    str1:= str1+' is outdated, the minimum required version is '
          +RevisionToVersion(cMinRevision)+'.';
    allIsFine:= false;
  end
  else begin
    str1:= str1+' meets the minimum requirements for the GUI.';
  end;//else
  if (RevisionIsExperimental(foundRev)) then
  begin
    str1:= str1 + #13#10#13#10 + 'You are using an experimental version of '
          + cProgrammeName + '!'+#13#10+'That means this version has known shortcomings'
          + ' or errors and should only be used for test purposes.';
    allIsFine:= false;
  end;//if
  if (RevisionIsOutdated(foundRev)) then
  begin
    str1:= str1 + #13#10#13#10 + 'You are using an old version of '
          + cProgrammeName + '.'+ #13#10 + 'It is recommended to update to '
          + RevisionToVersion(cFirstRevisionNotOutdated) + ' or later!';
    allIsFine:= false;
  end;//if
  if (allIsFine) then str1:= str1 + ' All is fine. :)';
  ShowMessage(str1);
end;

{$IFDEF WINDOWS }
procedure TForm1.WMCopyData(var Msg: TWMCopyData);
begin
  if (not m_ExpectsData) then Msg.Result:= 1
  else begin
    m_StringData:= PChar(Msg.CopyDataStruct^.lpData);
    Msg.Result:= 1;
    m_ExpectsData:= false;
    m_HasData:= true;
  end;//else
end;//proc
{$ENDIF}

procedure TForm1.DataToStringGrid(var total: Integer);
const nameCol = 0;
      editorIDCol = 1;
      formIDCol = 2;
      fileCol = 3;
      typeCol = 4;
var typeString: string;
    workData: AnsiString;
    extracted: string;
    currentRow, pos_i, delimPos: Integer;
begin
  typeString:= '';
  workData:= CP1252ToUTF8(m_StringData);
  trim4(workData);
  total:= -1;
  currentRow:= 0;
  while (length(workData)>0) do
  begin
    //check for start of new type
    if (copy(workData, 1, 9)='Matching ') then
    begin
      pos_i:= pos(':', workData);
      delimPos:= pos(#10, workData);
      if ((pos_i=0) or ((delimPos<>0) and (pos_i>delimPos))) then
      begin
        //error
        CleanUpGrid('Error: got unexpected string data in type line!');
        ShowMessage('Error: got unexpected string data in type line!');
        Exit;
      end;//if
      typeString:= copy(workData, 10, pos_i-10);
      workData:= copy(workData, pos_i+1, length(WorkData));
      trim4(workData);
    end//if "Matching ..."
    //check for full in-game name
    else if (workData[1]='"') then
    begin
      currentRow:= currentRow+1; //start of new row
      if (ResultStringGrid.RowCount<=currentRow) then
        ResultStringGrid.RowCount:= currentRow+1;
      pos_i:= pos(#10, workData); //search end of line
      if (pos_i=0) then
      begin
        //error
        CleanUpGrid('Error: got unexpected string data in name line!');
        ShowMessage('Error: got unexpected string data in name line!');
        Exit;
      end;//if
      extracted:= copy(WorkData, 2, pos_i-3);
      trim4(extracted);
      ResultStringGrid.Cells[nameCol, currentRow]:= extracted;
      ResultStringGrid.Cells[typeCol, currentRow]:= typeString;
      WorkData:= copy(WorkData, pos_i, length(WorkData));
      trim4(WorkData);
    end//if full name
    //check for formID
    else if (copy(workData, 1, 8)='form ID ') then
    begin
      if ((currentRow=0) or (typeString='')) then
      begin
        //error
        CleanUpGrid('Error: got unexpected form ID in string data!');
        ShowMessage('Error: got unexpected form ID in string data!');
        Exit;
      end;//if
      pos_i:= pos(#10, workData); //search end of line
      if (pos_i=0) then
      begin
        //error
        CleanUpGrid('Error: got unexpected string data in formID line!');
        ShowMessage('Error: got unexpected string data in formID line!');
        Exit;
      end;//if
      extracted:= copy(WorkData, 9, pos_i-9);
      trim4(extracted);
      ResultStringGrid.Cells[formIDCol, currentRow]:= copy(extracted, 1, 8);
      delimPos:= length(extracted);
      if (delimPos<=16) then
        ResultStringGrid.Cells[fileCol, currentRow]:= 'Skyrim.esm(?)'
      else
        ResultStringGrid.Cells[fileCol, currentRow]:= copy(extracted, 17, delimPos-17);
      WorkData:= copy(WorkData, pos_i, length(WorkData));
      trim4(WorkData);
    end//if form ID
    //check for editor ID
    else if (copy(workData, 1, 11)='editor ID "') then
    begin
      if ((currentRow=0) or (typeString='')) then
      begin
        //error
        CleanUpGrid('Error: got unexpected editor ID in string data!');
        ShowMessage('Error: got unexpected editor ID in string data!');
        Exit;
      end;//if
      pos_i:= pos(#10, workData); //search end of line
      if (pos_i=0) then
      begin
        //error
        CleanUpGrid('Error: got unexpected string data in editor ID line!');
        ShowMessage('Error: got unexpected string data in editor ID line!');
        Exit;
      end;//if
      extracted:= copy(WorkData, 12, pos_i-12);
      trim4(extracted);
      if (length(extracted)>0) then
        if (extracted[length(extracted)]='"') then
          extracted:= copy(extracted, 1, length(extracted)-1);
      ResultStringGrid.Cells[editorIDCol, currentRow]:= extracted;
      WorkData:= copy(WorkData, pos_i, length(WorkData));
      trim4(WorkData);
    end//if editorID
    //check for very last line
    else if (copy(workData, 1, 30)='Total matching objects found: ') then
    begin
      workData:= copy(workData, 30, length(WorkData));
      trim4(WorkData);
      total:= currentRow;
      if ((currentRow>0) and (ResultStringGrid.RowCount>currentRow+1)) then
        ResultStringGrid.RowCount:= currentRow+1;
      if (currentRow=0) then
      begin
        CleanUpGrid('No matching objects!');
      end;//zero rows
      ShowMessage('Search finished. Number of matching objects is '+IntToStr(total)+'.');
      Exit;
    end//if last line
    //check for last line of an object type
    else if (copy(workData, 1, 15)='Total matching ') then
    begin
      pos_i:= pos(#10, workData); //search end of line
      if (pos_i=0) then
      begin
        //error
        CleanUpGrid('Error: got unexpected string data in summary line!');
        ShowMessage('Error: got unexpected string data in summary line!');
        Exit;
      end;//if
      workData:= copy(workData, pos_i, length(workData));
      trim4(workData);
    end//if last line of a type
    else if (copy(workData,1, 8)='indices:') then
    begin
      //quest indices, but we cannot process them (yet), so skip the line
      pos_i:= pos(#10, workData); //search end of line
      if (pos_i=0) then
      begin
        //error
        CleanUpGrid('Error: got unexpected string data in indices line!');
        ShowMessage('Error: got unexpected string data in indices line!');
        Exit;
      end;//if
      WorkData:= copy(WorkData, pos_i, length(WorkData));
      trim4(WorkData);
    end//if quest indices
    //unknown line found
    else begin
      extracted:= 'Error: got unknown string data line! Work data length is '
                  +IntToStr(length(WorkData))+'.';
      if (length(WorkData)>0) then
      extracted:= extracted+#13#10+'First characters are: "'
                  +copy(WorkData, 1, 20)+'..."';
      CleanUpGrid(extracted);
      ShowMessage(extracted);
      Exit;
    end;//else unknown line
  end;//while
end;//proc

initialization
  {$I formid_finder.lrs}

end.

