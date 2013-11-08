/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2012 Thoronador

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

#ifndef MWTP_UNIXDOMAINSOCKETCLIENT_H
#define MWTP_UNIXDOMAINSOCKETCLIENT_H

#include <string>

namespace MWTP
{

class UnixDomainSocketClient
{
  public:
    /* constructor */
    UnixDomainSocketClient();

    /* destructor */
    ~UnixDomainSocketClient();


    /* tries to the Unix Domain Socket server using the socket file

       parameters:
           file - file name of the unix domain socket to be used
    */
    bool connectToServer(const std::string& file);

    /* returns true, if the client has established a connection to a "server" */
    bool isConnected() const;

    /* tries to read up to bytes bytes from the connection into an buffer

       parameters:
           buffer - the buffer that shall hold the read data
           bytes  - the maximum number of bytes that shall be read
    */
    int readConnection(void * buffer, const size_t bytes);

    /* tries to write bytes bytes from buffer to the connection

       parameters:
           buffer - pointer to the buffer that holds the data
           bytes  - number of bytes that shall be written from the buffer
    */
    int writeConnection(const void * buffer, const size_t bytes);
  protected:
    bool m_Connected;
    int m_sockfd;
}; //class

} //namespace

#endif // MWTP_UNIXDOMAINSOCKETCLIENT_H
