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

#ifndef MWTP_UNIXDOMAINSOCKETSERVER_HPP
#define MWTP_UNIXDOMAINSOCKETSERVER_HPP

#include <string>

namespace MWTP
{

class UnixDomainSocketServer
{
  public:
    /** \brief constructor */
    UnixDomainSocketServer();

    /** \brief destructor */
    virtual ~UnixDomainSocketServer();

    /** \brief tries to activate the "server", using the specified file name as socket name
     *
     * \param file file name of the Unix domain socket
     */
    bool activate(const std::string& file);


    /** \brief deactivates the server, if it's active */
    void deactivate();

    /** \brief checks whether the server is active or not.
     * \return Returns true, if the server is active.
     */
    bool isActive() const;


    /** \brief tries to make the socket a non-blocking socket
     *
     * \return Returns true in case of success.
     */
    bool makeNonBlocking();


    /** \brief checks whether the socket is a non-blocking socket
     *
     * \return Returns true, if the socket is a non-blocking socket.
     */
    bool isNonBlocking() const;


    /** \brief checks whether a server shutdown was requested
     *
     * \return Returns true, if the shutdown flag was set.
     */
    bool shutdownRequested() const;


    /** \brief clears the shutdown flag */
    void clearShutdownFlag();


    /** \brief starts listening for incoming connections
     *
     * ...until a shutdown is requested,
     * the specified time has elapsed or a connection was accepted - whatever
     * comes first. For blocking sockets, the time value has basically no mean-
     * ing, the function just waits for the first connection.
     *
     * \param milliseconds the amount of milliseconds the function waits for connections
     */
    bool startListening(const unsigned int milliseconds);

  protected:
    /** \brief tries to send a C++ string to a client
     *
     * \param clientSocketFD file descriptor of the client socket
     * \param message the std::string that shall be sent
     * \return Returns true, if the complete string was sent.
     *         Returns false, if an error occurred.
     */
    bool sendString(const int clientSocketFD, const std::string& message);


    /** \brief tries to receive a C++ string over a socket connection
     *
     * \param clientSocketFD file descriptor of the client socket
     * \param message reference to the C++ string that is used to store the message
     * \return Returns true, if the operation was successful. Returns false otherwise.
     * \remarks If the function fails, the contents of message will not be changed.
     */
    bool receiveString(const int clientSocketFD, std::string& message);


    /** \brief virtual function that handles the client/server transactions. Has to be
     * implemented in a derived class.
     *
     * \param client_socket_fd contains the file descriptor of the client socket
                               that is connected with the server
     */
    virtual void serveClient(const int client_socket_fd, bool& closeWhenDone) = 0;

    bool m_Active;
    bool m_shutdown;
    int m_sockfd;
    std::string m_File;
}; //class

} //namespace

#endif // MWTP_UNIXDOMAINSOCKETSERVER_HPP
