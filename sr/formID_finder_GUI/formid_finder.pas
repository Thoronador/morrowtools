unit formID_finder;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, LResources, Forms, Controls, Graphics, Dialogs,
{$IFDEF Windows }
  Windows, Messages,
{$ENDIF}
  StdCtrls, Grids;

type

  TWMCopyData = packed record
    Msg: Cardinal;
    From: HWND;
    CopyDataStruct: PCopyDataStruct;
    Result: Longint;
  end;

  { TForm1 }

  TForm1 = class(TForm)
    SearchButton: TButton;
    KeywordEdit: TEdit;
    ResultStringGrid: TStringGrid;
    procedure FormCreate(Sender: TObject);
    procedure SearchButtonClick(Sender: TObject);
  private
    { private declarations }
    procedure WMCopyData(var Msg: TWMCopyData); message WM_COPYDATA;
    procedure DataToStringGrid(var total: Integer);
    procedure CleanUpGrid(const info: string);
    m_ExpectsData: Boolean;
    m_HasData: Boolean;
    m_StringData: AnsiString;
  public
    { public declarations }
  end; 

const
{$IFDEF WINDOWS }
  cProgrammeName: string = 'formID_finder.exe';
{$ELSE}
  cProgrammeName: string = 'formID_finder';
{$ENDIF}
  cMinRevision = 413;

var
  Form1: TForm1;
  //window proc
    m_PrevWndProc: WNDPROC;

implementation

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


function GetFormIDFinderRevision: Cardinal;
var si: TStartupInfo;
    pi: TProcessInformation;
    exitCode: DWORD;
    hi: BOOL;
begin
  FillChar(si, SizeOf(si), 0);
  with si do
  begin
    cb := sizeof(si);
    dwFlags := STARTF_USESHOWWINDOW;
    wShowWindow := SW_HIDE;
  end;
  FillChar(pi, sizeof(pi), 0);
  hi:= FALSE;
  // start the child process
  if(CreateProcess(nil, //no module name (use command line)
      PChar(cProgrammeName+' --version-with-exitcode'), //command line
      nil, //process handle not inheritable
      nil, //thread handle not inheritable
      hi, //set handle inheritance to false
      0, //creation flags
      nil, //use parent's environment block
      nil, //use parent's starting directory
      si, //pointer to STARTUPINFO structure
      pi) //pointer to PROCESS_INFORMATION structure
    <>false) then
  begin
    //process started successfully
    exitCode := STILL_ACTIVE;
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
    Result := exitCode;
  end
  else begin
    //create process failed
    Result:= 0;
  end;
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
    413: Result:= 'v0.17.rev413, 2012-02-03';
  else
    Result:= 'v0.17 or later';
  end;//case
end;//func


function RunFormIDFinder(const keyword: string; var success: Boolean; const s1, s2: string): Cardinal;
var si: TStartupInfo;
    pi: TProcessInformation;
    exitCode: DWORD;
begin
  success:= false;
  FillChar(si, SizeOf(si), 0);
  with si do
  begin
    cb := sizeof(si);
    dwFlags := STARTF_USESHOWWINDOW;
    wShowWindow := SW_HIDE;
  end;
  FillChar(pi, sizeof(pi), 0);
  // start the child process
  if(CreateProcess(nil, //no module name (use command line)
      PChar(cProgrammeName+' --keyword "'+keyword+'" --send-data "'+s1+'" "'+s2+'"'), //command line
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
    exitCode := STILL_ACTIVE;
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
    Result := exitCode;
    success := true;
  end
  else begin
    //create process failed
    success:= false;
    Result:= 0;
  end;
end;//func

function isTrimableChar(const c: char): Boolean;
begin
  Result:= ((c=' ') or (c=#13) or (c=#10) or (c=#9));
end;//func

procedure trim4(var str1: string);
var position: Integer;
    look: Integer;
    size: Integer;
begin
  position := -1;
  look := 1;
  size := length(str1);
  while (look<=size) do
  begin
    if (isTrimableChar(str1[look])) then
      position := look
    else
      break;
    look:= look+1;
  end;//while
  if (position > -1) then
  begin
    if (position<size) then str1:= Copy(str1, position+1, size)
    else str1:= '';
  end;//if

  position := - 1;
  look := length(str1);
  while (look>=1) do
  begin
    if (isTrimableChar(str1[look])) then
      position := look
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
  if (not CheckRevision) then
  begin
    ShowMessage('Error: You have an older, unsupported version of '
               +cProgrammeName+'! To use this programme properly, you need '
               +' version 0.17.rev413 or later.');
    Exit;
  end;//if
  //remove old results
  CleanUpGrid('Searching...');
  //run the programme
  CleanUpGrid('Starting formID_finder...');
  m_HasData:= false;
  m_ExpectsData:= true;
  return:= RunFormIDFinder(KeywordEdit.Text, success, 'NULL', self.Caption);
  CleanUpGrid('formID_finder finished.');
  Application.ProcessMessages;
  m_ExpectsData:= false;
  //did the programme execution fail?
  if (not success) then
  begin
    CleanUpGrid('Could not start formID_finder.');
    Application.ProcessMessages;
    ShowMessage('Error while starting formID_finder!');
    Exit;
  end;//if
  CleanUpGrid('formID_finder executed successfully.');
  Application.ProcessMessages;
  //check return code
  if (return<>0) then
  begin
    errString:= 'Error while executing formID_finder! ';
    case return of
      1: errString:= errString+'Invalid parameters!';
      2: errString:= errString+'File error!';
      3: errString:= errString+'Data error!';
      10: errString:= errString+'formID_finder could not find GUI window!';
      11: errString:= errString+'formID_finder does not support sending data!';
    else
      errString:= errString+'Unknown error!';
    end;//case
    CleanUpGrid(errString);
    Application.ProcessMessages;
    ShowMessage(errString);
    Exit;
  end;//if return code<>0
  //now get the data
  if (not m_HasData) then
  begin
    CleanUpGrid('No data.');
    ShowMessage('No data recieved from formID_finder!');
    Exit;
  end;//if
  trim4(m_StringData);
  //ShowMessage('Trimmed recieved Content is:'+#13#10+'"'+m_StringData+'"');
  CleanUpGrid('Preparing data for display...');
  Application.ProcessMessages;
  DataToStringGrid(total);
end;//proc

procedure TForm1.FormCreate(Sender: TObject);
begin
  m_ExpectsData:= false;
  m_HasData:= false;
  m_StringData:= '';
  m_PrevWndProc:= Windows.WNDPROC(SetWindowLong(Self.Handle,GWL_WNDPROC,PtrInt(@WndCallback)));
  Application.Title:= self.Caption;
end;

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

procedure TForm1.DataToStringGrid(var total: Integer);
const nameCol = 0;
      editorIDCol = 1;
      formIDCol = 2;
      typeCol = 3;
var typeString: string;
    workData: AnsiString;
    extracted: string;
    currentRow, pos_i, delimPos: Integer;
begin
  typeString:= '';
  workData:= m_StringData;
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
      ResultStringGrid.Cells[formIDCol, currentRow]:= extracted;
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
    //unknown line found
    else begin
      extracted:= 'Error: got unknown string data line! Work data length is'
                  +IntToStr(length(WorkData))+';';
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

