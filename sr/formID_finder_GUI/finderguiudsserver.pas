unit FinderGUIUDSServer;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, UnixDomainSocketServer;

type
  TFinderGUIUDSServer = class(TUnixDomainSocketServer)
    public
      { constructor }
      constructor Create;

      { returns true, if the server has recieved some data }
      function hasData: Boolean;

      { returns the recieved data, if any }
      function getData: AnsiString;
    protected
      m_ExpectData: Boolean;
      m_Data: AnsiString;

      procedure serveClient(const client_socket_fd: Integer; var closeWhenDone: Boolean); virtual;
  end;//class

implementation

uses
  BaseUnix;

constructor TFinderGUIUDSServer.Create;
begin
  inherited Create;
  m_ExpectData := true;
  m_Data := '';
end;//construc

function TFinderGUIUDSServer.hasData: Boolean;
begin
  Result:= not m_ExpectData;
end;//func

function TFinderGUIUDSServer.getData: AnsiString;
begin
  Result:= m_Data;
end;//func

procedure TFinderGUIUDSServer.serveClient(const client_socket_fd: Integer; var closeWhenDone: Boolean);
var charPtr: PChar;
    tempPtr: PChar;
    len: Integer;
    buff_len, buff_offset: Integer;
    read_result: Integer;
begin
  //only read data, if we really expect some data
  if (m_ExpectData) then
  begin
    buff_len:= 4096;
    buff_offset:= 0;
    charPtr:= GetMem(buff_len);
    repeat
      read_result:= FpRead(m_sockfd, @charPtr[buff_offset], buff_len);
      if (read_result>0) then
      begin
        //data was read
        buff_offset:= buff_offset + read_result;
        //do we need to allocate more memory?
        if (buff_offset>=buff_len) then
        begin
          tempPtr:= GetMem(buff_len*2);
          Move(charPtr^, tempPtr^, buff_len);
          FreeMem(charPtr, buff_len);
          buff_len:= buff_len*2;
          charPtr:= tempPtr;
          tempPtr:= nil;
        end;//if more memory needed
      end;
    until read_result<=0;
    //have we been successful?
    if (read_result>=0) then
    begin
      charPtr[buff_offset]:= #0;
      m_Data := charPtr;
      m_ExpectData:= false;
    end;//if read was a success
  end;//if data is expected
end;//proc

end.

