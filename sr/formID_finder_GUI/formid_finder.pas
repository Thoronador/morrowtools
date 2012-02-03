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
    ShowMessage('Hit WndCallback');
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
    i: Integer;
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
    i:= 0;
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

{ TForm1 }

procedure TForm1.SearchButtonClick(Sender: TObject);
var rev, return: Cardinal;
    success: Boolean;
    errString: string;
    lastError: DWORD;
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
  ResultStringGrid.Clean(0, 1, 5, 1024*1024*1024, []);
  ResultStringGrid.Cells[0,1] := 'Searching...';
  //run the programme
  ResultStringGrid.Cells[0,1] := 'Starting formID_finder...';
  m_HasData:= false;
  m_ExpectsData:= true;
  return:= RunFormIDFinder(KeywordEdit.Text, success, 'NULL', self.Caption);
  ResultStringGrid.Cells[0,1] := 'run function finished.';
  Application.ProcessMessages;
  //did the programme execution fail?
  if (not success) then
  begin
    ResultStringGrid.Cells[0,1] := 'Could not start formID_finder.';
    //FreePipes;
    Application.ProcessMessages;
    ShowMessage('Error while starting formID_finder!');
    Exit;
  end;//if
  ResultStringGrid.Cells[0,1] := 'formID_finder executed successfully.';
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
    ResultStringGrid.Cells[0,1] := errString;
    Application.ProcessMessages;
    //FreePipes;
    Exit;
  end;//if return code<>0
  //now get the data
  if (not m_HasData) then
  begin
    ShowMessage('No data recieved!');
    Exit;
  end;//if
  ResultStringGrid.Cells[0,1] := 'data recieved!';
  Application.ProcessMessages;
  ShowMessage('Recieved Content was:'+#13#10+'"'
              +m_StringData+'"');
end;//proc

procedure TForm1.FormCreate(Sender: TObject);
begin
  m_ExpectsData:= false;
  m_HasData:= false;
  m_StringData:= '';
  m_PrevWndProc:= Windows.WNDPROC(SetWindowLong(Self.Handle,GWL_WNDPROC,PtrInt(@WndCallback)));
end;

procedure TForm1.WMCopyData(var Msg: TWMCopyData);
begin
  ResultStringGrid.Cells[0,2] := 'Hit WMCopyData!';
  if (not m_ExpectsData) then Msg.Result:= 1
  else begin
    m_StringData:= PChar(Msg.CopyDataStruct^.lpData);
    Msg.Result:= 1;
    m_ExpectsData:= false;
    m_HasData:= true;
  end;//else
end;//proc

initialization
  {$I formid_finder.lrs}

end.

