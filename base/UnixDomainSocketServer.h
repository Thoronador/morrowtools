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

    /* returns true, if the shutdown flag was set */
    bool shutdownRequested() const;

    /* starts listening for incoming connections until a shutdown is requested */
    bool startListening();
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
