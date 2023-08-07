{
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

unit UnixDomainSocketServer;

{ Note: This file is basically a reimplementation of the UnixDomainSocketServer
        class from /base/UnixDomainSocketServer.[h|cpp] files. }

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils;

type
  TUnixDomainSocketserver = class
    public
      { constructor }
      constructor Create;

      { destructor }
      destructor Destroy;

      { tries to activate the "server", using the specified file name as socket
        name

        parameters:
            file - file name of the unix domain socket
      }
      function activate(const the_file: string): Boolean;

      { deactivates the server, if it's active }
      procedure deactivate;

      { returns true, if the server is active }
      function isActive(): Boolean;

      { tries to make the socket a non-blocking socket and returns true in case
         of success
      }
      function makeNonBlocking: Boolean;

      { returns true, if the socket is a non-blocking socket }
      function isNonBlocking: Boolean;

      { returns true, if the shutdown flag was set }
      function shutdownRequested: Boolean;

      { clears the shutdown flag }
      procedure clearShutdownFlag;

      { starts listening for incoming connections until a shutdown is requested,
        the specified time has elapsed or a connection was accepted - whatever
        comes first. For blocking sockets, the time value has basically no mean-
        ing, the function just waits for the first connection.

        parameters:
            milliseconds - the amount of milliseconds the function waits for connections
      }
      function startListening(const milliseconds: Cardinal): Boolean;
    protected
      m_Active: Boolean;
      m_shutdown: Boolean;
      m_sockfd: Integer;
      m_File: string;

      { virtual function that handles the client/server transactions. Has to be
        implemented in a derived class.

        parameters:
            client_socket_fd - contains the file descriptor of the client socket
                               that is connected with the server
      }
      procedure serveClient(const client_socket_fd: Integer; var closeWhenDone: Boolean); virtual; abstract;
  end;//class

implementation

uses
  Sockets, BaseUnix, Unix;


function bindErrorCodeToString(const error_code: Integer): string;
begin
{ //doesn't find those constants, but which unit does contain those error constants?
  case (error_code) of
    SYS_EADDRINUSE: Result:= 'The specified address is already in use.';
    SYS_EADDRNOTAVAIL:
         Result:= 'The specified address is not available from the local machine.';
    SYS_EAFNOSUPPORT:
        Result:= 'The specified address is not a valid address for the address family of the specified socket.';
    SYS_EBADF:
        Result:= 'The socket argument is not a valid file descriptor.';
    SYS_EINVAL:
        Result:= 'The socket is already bound to an address, and the protocol '
                +'does not support binding to a new address; or the socket has '
                +'been shut down. OR The address_len argument is not a valid '
                +'length for the address family.';
    SYS_ENOTSOCK:
        Result:= 'The socket argument does not refer to a socket.';
    SYS_EOPNOTSUPP:
        Result:= 'The socket type of the specified socket does not support binding to an address.';
    SYS_EACCES:
        Result:= 'A component of the path prefix denies search permission, or '
                +'the requested name requires writing in a directory with a mode that denies write permission.';
    SYS_EDESTADDRREQ, SYS_EISDIR:
        Result:= 'The address argument is a null pointer.';
    SYS_EIO:
        Result:= 'An I/O error occurred.';
    SYS_ELOOP:
        Result:= 'A loop exists in symbolic links encountered during resolution of the pathname in address.';
    SYS_ENAMETOOLONG:
        Result:= 'A component of a pathname exceeded NAME_MAX characters, or an entire pathname exceeded PATH_MAX characters.';
    SYS_ENOENT:
        Result:= 'A component of the pathname does not name an existing file or the pathname is an empty string.';
    SYS_ENOTDIR:
        Result:= 'A component of the path prefix of the pathname in address is not a directory.';
    SYS_EROFS:
        Result:= 'The name would reside on a read-only file system.';
    //EACCES
    //The specified address is protected and the current user does not have permission to bind to it.
    SYS_EISCONN:
        Result:= 'The socket is already connected.';
    SYS_ENOBUFS:
        Result:= 'Insufficient resources were available to complete the call.';
  else
  }
    Result:= 'Unexpected error code!';
  { end;//case }
end;//function

constructor TUnixDomainSocketServer.Create;
begin
  m_Active := false;
  m_shutdown := false;
  m_sockfd := -1;
  m_File := '';
end;//construc

destructor TUnixDomainSocketServer.Destroy;
begin
  //shut down here
  deactivate;
end;//destruc

function TUnixDomainSocketServer.activate(const the_file: string): Boolean;
var serv_addr: sockaddr_un;
    sockfd: Integer;
begin
  if (m_Active) then
  begin
    Result:= true;
    Exit;
  end;

  sockfd := FpSocket(AF_UNIX, SOCK_STREAM, 0);
  if (sockfd < 0) then
  begin
    WriteLn('Error while opening socket!');
    Result:= false;
    Exit;
  end;//if
  FillChar(serv_addr, sizeof(serv_addr), 0);
  serv_addr.sun_family := AF_UNIX;
  //C++: snprintf(serv_addr.sun_path, file.length()+1, file.c_str());
  Move(the_File, serv_addr.sun_path, length(the_File));
  serv_addr.sun_path[length(the_File)+1] := #0;
  if (FpBind(sockfd, @serv_addr, sizeof(sockaddr_un)) < 0) then
  begin
    WriteLn('SocketServ: Error on binding! Error code is ', SocketError, '.');
    WriteLn('That means: ', bindErrorCodeToString(SocketError));
    Result:= false;
    Exit;
  end;
  m_Active:= true;
  m_sockfd:= sockfd;
  m_File:= the_file;
  Result:= true;
end;//func

function TUnixDomainSocketServer.isActive: Boolean;
begin
  Result:= m_Active;
end;//func

procedure TUnixDomainSocketServer.deactivate;
begin
  if (not m_Active) then Exit;
  CloseSocket(m_sockfd);
  m_sockfd := -1;
  FpUnlink(m_File);
  m_File := '';
  m_Active := false;
end;//proc

function TUnixDomainSocketServer.makeNonBlocking: Boolean;
var flags: Integer;
begin
  if (not m_Active) then
  begin
    Result:= false;
    Exit;
  end;
  flags := FpFcntl(m_sockfd, F_GETFL, 0); //get the existing flags
  if (flags<0) then
  begin
    //failed
    Result:= false;
    Exit;
  end;
  flags := FpFcntl(m_sockfd, F_SETFL, flags or O_NONBLOCK); //add non-blocking flag
  Result:= (flags<>-1);
end;//func

function TUnixDomainSocketServer.isNonBlocking: Boolean;
var flags: Integer;
begin
  if (not m_Active) then
  begin
     Result:= false;
     Exit;
  end;//if
  flags := FpFcntl(m_sockfd, F_GETFL, 0); //get the existing flags
  if (flags<0) then
  begin
    //failed
    Result:= false;
    Exit;
  end;//if
  Result:= ((flags and O_NONBLOCK)<>0);
end;//function

function TUnixDomainSocketServer.shutdownRequested: Boolean;
begin
  Result:= m_shutdown;
end;//func

procedure TUnixDomainSocketServer.clearShutdownFlag;
begin
  m_shutdown := false;
end;//proc

function TUnixDomainSocketServer.startListening(const milliseconds: Cardinal): Boolean;
var read_flags: TFDSet;
    curr_time: timeval;
    end_time: timeval;
    wait_intervall: timeval;
    done: Boolean;
    nonBlocking: Boolean;
    cli_addr: sockaddr_un;
    clilen: socklen_t;
    newsockfd: Integer;
    closeFDWhenDone: Boolean;
begin
  if (not m_Active) then
  begin
    Result:= false;
    Exit;
  end;//if
  wait_intervall.tv_sec := milliseconds div 1000; //seconds
  wait_intervall.tv_usec := (milliseconds mod 1000)*1000; //microseconds

  FpGettimeofday(@curr_time, nil);
  //workaround for C's timeadd() function: timeradd(&curr_time, &wait_intervall, &end_time);
  end_time.tv_sec :=  curr_time.tv_sec + wait_intervall.tv_sec;
  end_time.tv_usec :=  curr_time.tv_usec + wait_intervall.tv_usec;
  if (end_time.tv_usec>=1000000) then
  begin
    end_time.tv_sec:= end_time.tv_sec+1;
    end_time.tv_usec:= end_time.tv_usec-1000000;
  end;
  //end of workaround

  done := false;
  nonBlocking := isNonBlocking;

  while ((not shutdownRequested) and (not done)) do
  begin
    if (FpListen(m_sockfd,5) <> 0) then
    begin
      WriteLn('SocketServ: Error on listen!');
      Result:= false;
      Exit;
    end;//if

    if (nonBlocking) then
    begin
      FpFD_ZERO(read_flags);
      FpFD_SET(m_sockfd, read_flags);
      if (FpSelect(m_sockfd+1, @read_flags, nil, nil, @wait_intervall)<0) then
      begin
        WriteLn('SocketServ: Error on select!');
        Result:= false;
        Exit;
      end;

    end;//if

    if ((not nonBlocking) or (FpFD_ISSET(m_sockfd, read_flags)>0)) then
    begin
      clilen := sizeof(cli_addr);
      newsockfd := FpAccept(m_sockfd, @cli_addr, @clilen);
      if (newsockfd < 0) then
      begin
        WriteLn('SocketServ: Error on accept!');
        Result:= false;
        Exit;
      end;
      closeFDWhenDone := true;
      serveClient(newsockfd, closeFDWhenDone);
      if (closeFDWhenDone) then
        CloseSocket(newsockfd);
      done := true;
    end;//if ready
    if (not done) then
    begin
      FpGettimeofday(@curr_time, nil);
      done := ((curr_time.tv_sec>end_time.tv_sec)
                or ((curr_time.tv_sec=end_time.tv_sec) and (curr_time.tv_usec>=end_time.tv_usec)));
    end;//if not done
  end;//while
  Result := true;
end;//func

end.

