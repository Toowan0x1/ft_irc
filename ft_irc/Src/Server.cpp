/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:27:00 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/18 13:27:01 by oel-houm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"
#include "../Include/Client.hpp"
#include "../Include/Channel.hpp"

void    Server::acceptConnection() {
    struct sockaddr_in clientAddr;
    socklen_t clientSize = sizeof(clientAddr);
    int clientSocketFd = accept(this->_serverSocketFd, (sockaddr*)&clientAddr, &clientSize);
    if (clientSocketFd < 0)
        throw std::runtime_error("ERROR: Problem with client connecting!");
    if (fcntl(clientSocketFd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Error setting client socket to non-blocking mode!");
    //int sa = getnameinfo((sockaddr*)&client, sizeof(client), hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
    // if (sa == 0)
        // std::cout << "Accepted connection on descriptor " << clientSocket << "(host=" << hbuf << ", port=" << sbuf << ")" << std::endl;
    // // adding it to POLL's array of sruct so it get monitored as well 
    pollfd pfd;
    pfd.fd = clientSocketFd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    this->_pfds.push_back(pfd);
    //
    Client *my_client = new Client(clientSocketFd);
    my_client->_IPAddress = inet_ntoa(clientAddr.sin_addr);
    this->_clientList.push_back(my_client);
    std::cout << "[+] New Client Connected, Client IP Address " << my_client->_IPAddress << std::endl;
}

void    Server::setServerSocket() {
    int optValue = 1;
    this->_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_serverSocketFd < 0)
        throw std::runtime_error("Failed to create server socket");
    if (setsockopt(this->_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &optValue, sizeof(optValue)) < 0)
        throw std::runtime_error("Failed to set SO_REUSEADDR option"); 
    if (fcntl(this->_serverSocketFd, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Failed to set socket to non-blocking mode");
}

void    Server::setServerAddrInfo() {
    memset(&this->_addr, 0, sizeof(this->_addr));
    this->_addr.sin_family = AF_INET;
    this->_addr.sin_addr.s_addr = INADDR_ANY;
    this->_addr.sin_port = htons(this->_port);
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        this->_hostname = hostname;
    } else {
        throw std::runtime_error("Error getting hostname");
    }
    struct pollfd serverPfd;
    serverPfd.fd = this->_serverSocketFd;
    serverPfd.events = POLLIN;
    serverPfd.revents = 0;
    this->_pfds.push_back(serverPfd); // this->_pfds[0] = serverPfd;
}

void    Server::bindServerSocket() {
    if (bind(this->_serverSocketFd, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) < 0) {
        throw std::runtime_error("Failed to bind the server socket");
    }
    // std::cout << "Server Socket has been created and binded successfully" << std::endl;
}

// getnameinfo()
// server loop [3]
// [-] Client Disconnected

/*
std::string Server::addClient(struct pollfd _poll, std::string line)
{
    std::stringstream toowan(line);
    std::string pass;
    toowan >> pass;
    if (pass != "PASS")
        return "464 :Password incorrect\r\n";
    iss >> pass;
    if (!pass.empty() && pass[0] == ':')
        pass = pass.substr(1);
    if (pass != this->_password)
        return "464 :Password incorrect\r\n";
    this->list[_poll.fd].setRegistered(true);
    return "";
}
*/

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream ss(s);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

bool    Server::startsWith(const std::string &str, const std::string &target)
{
    size_t      spacePos = str.find(' '); // "        cmd    " spaces before the cmd!
    std::string firstWord = str.substr(0, spacePos);
    return (firstWord == target);
}

void    Server::parse_cmd(std::string line, int i) {
    i = i -1; // because i = 1 and the index of clients starts from 0
    // this->_clientList[i]->_buffer = line;
    // std::cout << 0 << ":" << _clientList[i]->_buffer << std::endl;
    if (startsWith(line, "connect") || startsWith(line, "CONNECT")) {
        //std::cout << "yes> " << line << std::endl;
    }
    if (startsWith(line, "pass") || startsWith(line, "PASS")) {
        this->_clientList[i]->_authenticated = false;
        //int j = line.find("PASS");
        // if (line)
            // this->_clientList[0]->_password = line.substr(j+1);
        if (line.substr(4+1).compare("IRC-1337") == 0) {
            this->_clientList[i]->_authenticated = true;
            std::cout << "true" << std::endl;
        }
        else {
            std::cout << "false" << std::endl;
        }
    }
    if (startsWith(line, "server") || startsWith(line, "SERVER")) {
        /**/
    }
    if (startsWith(line, "nick") || startsWith(line, "NICK")) {
        this->_clientList[i]->_nickname = line.substr(4+1);
        std::cout << "nick: " << this->_clientList[i]->_nickname << std::endl;
    }
    if (startsWith(line, "user") || startsWith(line, "USER")) {
        /**/
        this->_clientList[i]->_username = line.substr(4+1);
        std::cout << "user: " << this->_clientList[i]->_username << std::endl;
    }
    if (startsWith(line, "join") || startsWith(line, "JOIN")) {
        /**/
    }
    if (startsWith(line, "pass") || startsWith(line, "PASS"))
    {
        this->_clientList[i]->_password = line.substr(4+1);
        std::cout << "pass: " << this->_clientList[i]->_password << std::endl;
    }
    if (startsWith(line, "listz") || startsWith(line, "LISTZ")) {
        /**/
        std::cout << "authenticated " << this->_clientList[i]->_authenticated << std::endl;
        std::cout << "pass " << this->_clientList[i]->_password << std::endl;
        std::cout << "user " << this->_clientList[i]->_username << std::endl;
        std::cout << "nick " << this->_clientList[i]->_nickname << std::endl;
    }
    
    // realname  _buffer
}

void    Server::start() {
    std::cout << "Irc server Launched" << std::endl;
    this->setServerSocket();
    this->setServerAddrInfo();
    this->bindServerSocket();

    if (listen(this->_serverSocketFd, SOMAXCONN) < 0) {
        throw std::runtime_error("ERROR: Failed to listen for connections: " + std::string(strerror(errno)));
    }
    std::cout << "[*] Listening ..." << std::endl;
    
    if (fcntl(this->_serverSocketFd, F_SETFL, O_NONBLOCK) == -1) {
        std::runtime_error("Error setting socket to non-blocking mode");//color
    }

    while (1) {
        if (poll(this->_pfds.data(), this->_pfds.size(), -1) < 0) {
            throw std::runtime_error("ERROR: poll failed"); // error monitoring sockets!
        }
        for (unsigned int i = 0; i < this->_pfds.size(); i++)
        {
            if (this->_pfds[i].revents == 0)
                continue ;
            if (this->_pfds[i].revents == POLLIN && this->_pfds[i].fd == this->_serverSocketFd) {
                acceptConnection(); // handle error in accept(serversocketfd, nullptr, nullptr) if == -1 < 0
            }
            if (this->_pfds[i].revents == POLLIN && this->_pfds[i].fd != this->_serverSocketFd) {
                // acceptMessage();
                char buffer[1024];
                std::memset(buffer, 0, sizeof(buffer));
                int receivedBytes = recv(this->_pfds[i].fd, buffer, sizeof(buffer), 0);
                /* Set msgSize */
                if (receivedBytes < 0)
                    std::runtime_error("recv error");
                if (receivedBytes == 0) {
                }
                if (receivedBytes > 0) {
                    buffer[receivedBytes] = '\0'; // Null-terminate the received data
                    //std::cout << "received bytes: \n <" << buffer << ">" << std::endl;
                    std::istringstream iss(buffer);
                    std::string line;
                    while (std::getline(iss, line, '\n'))
                    {
                        //std::cout << "line: " << line << std::endl;
                        parse_cmd(line, i);
                    }
                }
            }
            if (this->_pfds[i].revents == 17 || this->_pfds[i].revents == POLLHUP) {
            // if (this->_fds[i].revents & (POLLHUP | POLLERR)) {
                // handleDisconnect();
            }
        }
        // disconnect the client if the _keepAlive boolean is false 
        for (unsigned int i = 0; i < this->_clientList.size(); i++)
        {
            if (this->_clientList[i]->_keepAlive) {
                // handle_disconnection(i + 1);
            }
        }
    }
}

/*
Stream class to operate on strings.
Objects of this class use a string buffer that contains a sequence of characters. This sequence of characters can be accessed directly as a string object, using member str.
iss: input string stream.

https://github.com/hikaarru/ft_irc/blob/main/srcs/server.cpp
yascont/internet-relay-chat-server
*/

/*
// Determine the size of the incoming message
// determine the size of the incoming message without actually removing any data from the socket.
int messageSize = recv(_fds[i].fd, nullptr, 0, MSG_PEEK); // When MSG_PEEK is set, the data is "peeked at" but not actually consumed, so it will still be available for subsequent calls to recv.

// Dynamically allocate a buffer based on the message size
char* buffer = new char[messageSize];

// Receive the complete message
int bytesRead = recv(_fds[i].fd, buffer, messageSize, 0);

// Process the message as before...

// Don't forget to free the allocated memory when done
delete[] buffer;

*/

Server::Server(std::string port, std::string password) {
    this->_port = atoi(port.c_str());
    if (this->_port == 0 || (this->_port < 0 || this->_port > 65535))
        throw std::runtime_error("Invalid port");
    this->_password = password;
    if (!(this->_password.length() >= 8 && this->_password.length() <= 16)) // != Password || empty
        throw std::runtime_error("Invalid password");
}

Server::~Server() {
    for (unsigned int i = 0; i < this->_clientList.size(); i++) {
        delete _clientList[i];
        close(this->_pfds[i + 1].fd);
    }
    /*
    delete all channels:
    {
        ...
    }
    */
   // if there is allocated commands in memory, then close all
    // close(this->serverSocketFd);
    if (this->_pfds[0].fd)//this->_pfds.size())
        close(this->_pfds[0].fd);
    std::cout << "Server closed!" << std::endl;
}