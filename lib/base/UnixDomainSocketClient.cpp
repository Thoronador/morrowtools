/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2012, 2014, 2015  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "UnixDomainSocketClient.hpp"
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace MWTP
{

std::string connectErrnoToString(int error_code)
{
  switch(error_code)
  {
    case EADDRNOTAVAIL:
         return "The specified address is not available from the local machine.";
         break;
    case EAFNOSUPPORT:
         return "The specified address is not a valid address for the address family of the specified socket.";
         break;
    case EALREADY:
         return "A connection request is already in progress for the specified socket.";
         break;
    case EBADF:
         return "The socket argument is not a valid file descriptor.";
         break;
    case ECONNREFUSED:
         return "The target address was not listening for connections or refused the connection request.";
         break;
    case EINPROGRESS:
         return "O_NONBLOCK is set for the file descriptor for the socket and the connection cannot be immediately established; the connection shall be established asynchronously.";
         break;
    case EINTR:
         return "The attempt to establish a connection was interrupted by delivery of a signal that was caught; the connection shall be established asynchronously.";
         break;
    case EISCONN:
         return "The specified socket is connection-mode and is already connected.";
         break;
    case ENETUNREACH:
         return "No route to the network is present.";
         break;
    case ENOTSOCK:
         return "The socket argument does not refer to a socket.";
         break;
    case EPROTOTYPE:
         return "The specified address has a different type than the socket bound to the specified peer address.";
         break;
    case ETIMEDOUT:
         return "The attempt to connect timed out before a connection was made.";
         break;
    case EIO:
         return "An I/O error occurred while reading from or writing to the file system.";
         break;
    case ELOOP:
         return "A loop exists in symbolic links encountered during resolution of the pathname in address.";
         break;
    case ENAMETOOLONG:
         return "A component of a pathname exceeded {NAME_MAX} characters, or an entire pathname exceeded {PATH_MAX} characters.";
         break;
    case ENOENT:
         return "A component of the pathname does not name an existing file or the pathname is an empty string.";
         break;
    case ENOTDIR:
         return "A component of the path prefix of the pathname in address is not a directory.";
         break;
    default:
         return "Unknown error code.";
         break;
  }//swi
}

UnixDomainSocketClient::UnixDomainSocketClient()
: m_Connected(false),
  m_sockfd(-1)
{
}

UnixDomainSocketClient::~UnixDomainSocketClient()
{
  closeConnection();
}

bool UnixDomainSocketClient::connectToServer(const std::string& file)
{
  int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0)
  {
    std::cout << "SocketClient: Error opening socket!\n";
    return false;
  }

  struct sockaddr_un serv_addr;
  /* start with a clean address structure */
  memset(&serv_addr, 0, sizeof(struct sockaddr_un));

  serv_addr.sun_family = AF_UNIX;
  snprintf(serv_addr.sun_path, file.length()+1, file.c_str());

  const int conn_status = connect(socket_fd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr_un));
  if (conn_status != 0)
  {
    close(socket_fd);
    std::cout << "SocketClient: Error: connect() failed!\n"
              << "Error code is "<< errno<<", indicating the following error:\n"
              << connectErrnoToString(errno)<<"\n";
    return false;
  }
  m_sockfd = socket_fd;
  m_Connected = true;
  return true;
}

bool UnixDomainSocketClient::isConnected() const
{
  return m_Connected;
}

int UnixDomainSocketClient::readConnection(void * buffer, const size_t bytes)
{
  return read(m_sockfd, buffer, bytes);
}

int UnixDomainSocketClient::writeConnection(const void * buffer, const size_t bytes)
{
  return write(m_sockfd, buffer, bytes);
}

bool UnixDomainSocketClient::sendString(const std::string& message)
{
  const std::string::size_type len = message.size()+1;
  int written = 0;
  while (written<len)
  {
    int new_written = writeConnection(message.c_str()+written, len);
    if (new_written>0)
    {
      written += new_written;
    }
    else
    {
      //some kind of error occurred or the connection was closed
      break;
    }
  }//while
  return (written>=len);
}

bool UnixDomainSocketClient::receiveString(std::string& message)
{
  std::string result;
  const int cBufferSize = 4096;
  char buffer[cBufferSize];
  int read = 0;
  int current_read = 0;
  do
  {
    memset(buffer, '\0', cBufferSize);
    current_read = recv(m_sockfd, buffer, cBufferSize-1, 0);
    if (current_read>0)
    {
      read += current_read;
      result += std::string(buffer);
    }
    else
    {
      //error or connection closed
      break;
    }
  } while (current_read>=cBufferSize-1);
  if (current_read>0)
  {
    message = result;
    return true;
  }
  return false;
}

void UnixDomainSocketClient::closeConnection()
{
  if (m_Connected)
  {
    close(m_sockfd);
    m_Connected = false;
    m_sockfd = -1;
  }
}

} //namespace
