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

#ifndef MWTP_UNIXDOMAINSOCKETSERVER_H
#define MWTP_UNIXDOMAINSOCKETSERVER_H

#include <string>

namespace MWTP
{

class UnixDomainSocketServer
{
  public:
    /* constructor */
    UnixDomainSocketServer();

    /* destructor */
    ~UnixDomainSocketServer();

    /* tries to activate the "server", using the specified file name as socket
       name

       parameters:
           file - file name of the unix domain socket
    */
    bool activate(const std::string& file);

    /* deactivates the server, if it's active */
    void deactivate();

    /* returns true, if the server is active */
    bool isActive() const;

    /* tries to make the socket a non-blocking socket and returns true in case
       of success
    */
    bool makeNonBlocking();

    /* returns true, if the socket is a non-blocking socket */
    bool isNonBlocking() const;

    /* returns true, if the shutdown flag was set */
    bool shutdownRequested() const;

    /* clears the shutdown flag */
    void clearShutdownFlag();

    /* starts listening for incoming connections until a shutdown is requested,
       the specified time has elapsed or a connection was accepted - whatever
       comes first. For blocking sockets, the time value has basically no mean-
       ing, the function just waits for the first connection.

       parameters:
           milliseconds - the amount of milliseconds the function waits for connections
    */
    bool startListening(const unsigned int milliseconds);
  protected:
    /* virtual function that handles the client/server transactions. Has to be
       implemented in a derived class.

       parameters:
           client_socket_fd - contains the file descriptor of the client socket
                              that is connected with the server
    */
    virtual void serveClient(const int client_socket_fd, bool& closeWhenDone) = 0;

    bool m_Active;
    bool m_shutdown;
    int m_sockfd;
    std::string m_File;
}; //class

} //namespace

#endif // MWTP_UNIXDOMAINSOCKETSERVER_H
