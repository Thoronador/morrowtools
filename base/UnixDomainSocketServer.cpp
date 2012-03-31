/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2012 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "UnixDomainSocketServer.h"
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <fcntl.h>

namespace MWTP
{

std::string bindErrorCodeToString(const int error_code)
{
  switch (error_code)
  {
    case EADDRINUSE:
         return "The specified address is already in use.";
         break;
    case EADDRNOTAVAIL:
         return "The specified address is not available from the local machine.";
         break;
    case EAFNOSUPPORT:
         return "The specified address is not a valid address for the address family of the specified socket.";
         break;
    case EBADF:
         return "The socket argument is not a valid file descriptor.";
         break;
    case EINVAL:
         return "The socket is already bound to an address, and the protocol does not support binding to a new address; or the socket has been shut down. OR The address_len argument is not a valid length for the address family.";
         break;
    case ENOTSOCK:
         return "The socket argument does not refer to a socket.";
         break;
    case EOPNOTSUPP:
         return "The socket type of the specified socket does not support binding to an address.";
         break;
    case EACCES:
         return "A component of the path prefix denies search permission, or the requested name requires writing in a directory with a mode that denies write permission.";
         break;
    case EDESTADDRREQ:
    case EISDIR:
         return "The address argument is a null pointer.";
         break;
    case EIO:
         return "An I/O error occurred.";
         break;
    case ELOOP:
         return "A loop exists in symbolic links encountered during resolution of the pathname in address.";
         break;
    case ENAMETOOLONG:
         return "A component of a pathname exceeded {NAME_MAX} characters, or an entire pathname exceeded {PATH_MAX} characters.";
    case ENOENT:
         return "A component of the pathname does not name an existing file or the pathname is an empty string.";
    case ENOTDIR:
         return "A component of the path prefix of the pathname in address is not a directory.";
    case EROFS:
         return "The name would reside on a read-only file system.";

    //EACCES
    //The specified address is protected and the current user does not have permission to bind to it.
    case EISCONN:
         return "The socket is already connected.";
    case ENOBUFS:
         return "Insufficient resources were available to complete the call.";
    default:
         return "Unexpected error code!";
  }//swi
}


UnixDomainSocketServer::UnixDomainSocketServer()
{
  m_Active = false;
  m_shutdown = false;
  m_sockfd = -1;
  m_File = "";
}

UnixDomainSocketServer::~UnixDomainSocketServer()
{
  //shut down here
  deactivate();
}

bool UnixDomainSocketServer::activate(const std::string& file)
{
  if (m_Active) return true;

  struct sockaddr_un serv_addr;
  int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    std::cout << "Error while opening socket!\n";
    return false;
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sun_family = AF_UNIX;
  snprintf(serv_addr.sun_path, file.length()+1, file.c_str());
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr_un)) < 0)
  {
    std::cout << "SocketServ: Error on binding! Error code is "<<errno
              <<".\nThat means: "<<bindErrorCodeToString(errno) <<"\n";
    return false;
  }
  m_Active = true;
  m_sockfd = sockfd;
  m_File = file;
  return true;
}

bool UnixDomainSocketServer::isActive() const
{
  return m_Active;
}

void UnixDomainSocketServer::deactivate()
{
  if (!m_Active) return;
  close(m_sockfd);
  m_sockfd = -1;
  unlink(m_File.c_str());
  m_File.clear();
  m_Active = false;
}

bool UnixDomainSocketServer::makeNonBlocking()
{
  if (!m_Active) return false;
  int flags = fcntl(m_sockfd, F_GETFL, 0); //get the existing flags
  if (flags<0)
  {
    //failed
    return false;
  }
  flags = fcntl(m_sockfd, F_SETFL, flags | O_NONBLOCK); //add non-blocking flag
  return (flags!=-1);
}

bool UnixDomainSocketServer::isNonBlocking() const
{
  if (!m_Active) return false;
  const int flags = fcntl(m_sockfd, F_GETFL, 0); //get the existing flags
  if (flags<0)
  {
    //failed
    return false;
  }
  return ((flags & O_NONBLOCK)!=0);
}

bool UnixDomainSocketServer::shutdownRequested() const
{
  return m_shutdown;
}

void UnixDomainSocketServer::clearShutdownFlag()
{
  m_shutdown = false;
}

bool UnixDomainSocketServer::startListening(const unsigned int milliseconds)
{
  if (!m_Active) return false;

  fd_set read_flags;
  struct timeval curr_time;
  struct timeval end_time;
  struct timeval wait_intervall;
  wait_intervall.tv_sec = milliseconds / 1000; //seconds
  wait_intervall.tv_usec = (milliseconds%1000)*1000; //microseconds

  gettimeofday(&curr_time, NULL);
  timeradd(&curr_time, &wait_intervall, &end_time);

  bool done = false;
  const bool nonBlocking = isNonBlocking();

  while ((!shutdownRequested()) and (!done))
  {
    if (listen(m_sockfd,5) != 0)
    {
      std::cout << "SocketServ: Error on listen!\n";
      return false;
    }

    if (nonBlocking)
    {
      FD_ZERO(&read_flags);
      FD_SET(m_sockfd, &read_flags);
      if (select(m_sockfd+1, &read_flags, NULL, NULL, &wait_intervall)<0)
      {
        std::cout << "SocketServ: Error on select!\n";
        return false;
      }

    }

    if ((!nonBlocking) or (FD_ISSET(m_sockfd, &read_flags)))
    {
      struct sockaddr_un cli_addr;
      socklen_t clilen = sizeof(cli_addr);
      int newsockfd = accept(m_sockfd, (struct sockaddr *) &cli_addr, &clilen);
      if (newsockfd < 0)
      {
        std::cout << "SocketServ: Error on accept!\n";
        return false;
      }
      bool closeFDWhenDone = true;
      serveClient(newsockfd, closeFDWhenDone);
      if (closeFDWhenDone)
        close(newsockfd);
      done = true;
    }//if ready
    if (not done)
    {
      gettimeofday(&curr_time, NULL);
      done = ((curr_time.tv_sec>end_time.tv_sec)
                or ((curr_time.tv_sec==end_time.tv_sec) and (curr_time.tv_usec>=end_time.tv_usec)));
    }//if not done
  }//while
  return true;
}

} //namespace
