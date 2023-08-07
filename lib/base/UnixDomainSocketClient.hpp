/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2012, 2014  Thoronador

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

#ifndef MWTP_UNIXDOMAINSOCKETCLIENT_HPP
#define MWTP_UNIXDOMAINSOCKETCLIENT_HPP

#include <string>

namespace MWTP
{

class UnixDomainSocketClient
{
  public:
    /** \brief constructor
     */
    UnixDomainSocketClient();

    /** \brief destructor */
    ~UnixDomainSocketClient();


    /** \brief tries to the Unix Domain Socket server using the socket file
     *
     * \param file file name of the unix domain socket to be used
     * \return Returns true, if connection could be established.
     */
    bool connectToServer(const std::string& file);


    /* returns true, if the client has established a connection to a "server" */
    /** \brief checks, if the client has established a connection to a server
     *
     * \return Returns true, if the client has established a connection to a "server".
     */
    bool isConnected() const;


    /** \brief tries to read up to bytes bytes from the connection into an buffer
     *
     * \param buffer the buffer that shall hold the read data
     * \param bytes  the maximum number of bytes that shall be read
     * \return Returns the number of bytes actually read. If an error occurred, -1 is returned.
     */
    int readConnection(void * buffer, const size_t bytes);


    /** \brief tries to write bytes from a buffer to the connection
     *
     * \param buffer pointer to the buffer that holds the data
     * \param bytes number of bytes that shall be written from the buffer
     * \return Returns the number of bytes actually written. If an error occurred, -1 is returned.
     */
    int writeConnection(const void * buffer, const size_t bytes);


    /** \brief tries to send a C++ string over the connection
     *
     * \param message the std::string that shall be sent
     * \return Returns true, if the complete string was sent.
     *         Returns false, if an error occurred.
     */
    bool sendString(const std::string& message);


    /** \brief tries to receive a C++ string over the connection
     *
     * \param message reference to the C++ string that is used to store the message
     * \return Returns true, if the operation was successful. Returns false otherwise.
     * \remarks If the function fails, the contents of message will not be changed.
     */
    bool receiveString(std::string& message);


    /** \brief closes an existing connection */
    void closeConnection();
  protected:
    bool m_Connected; /**< holds the connection state */
    int m_sockfd; /**< socket file descriptor */
}; //class

} //namespace

#endif // MWTP_UNIXDOMAINSOCKETCLIENT_HPP
