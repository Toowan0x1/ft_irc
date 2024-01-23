/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:27:00 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/22 21:28:54 by oel-houm         ###   ########.fr       */
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
                //if (receivedBytes == 0)
                    //
                if (receivedBytes > 0) {
                    buffer[receivedBytes] = '\0'; // Null-terminate the received data
                    //std::cout << "received bytes: \n" << buffer << std::endl;
                    std::string cap, pass, nick, user;
                    std::vector<std::string> lines;
                    std::string line;
                    // split the received message into parts (commands and parameters)
                    std::vector<std::string> parts;
                    std::istringstream iss(buffer);
                    int j = 0;
                    while (std::getline(iss, line, '\n')) {
                        lines.push_back(line);
                        j++;
                    }
                    // std::cout << "total lines (" << j << ")" << std::endl;
                    int i = 0;
                    int flag = 0;
                    while (i < j) {
                        // std::cout << "line " << i << ": " << lines[i] << std::endl;
                        std::vector<std::string> tokens;
                        std::string token;
                        std::istringstream parts(lines[i]);
                        std::string tmp;
                        while (std::getline(parts, token, ' '))
                        {
                            //
                            tokens.push_back(token);
                            // token.push_back('\0');
                            //std::cout << "token(" << i << "): " << token << std::endl;
                            // (token == "LIST" || tokens[0] == "LIST" || tokens[0] == "list")
                            if (!(tokens[0].find("LIST")))
                            {
                                std::cout << "token(LIST): " << token << std::endl;
                            }
                            else if (tokens[0] == "CAP")
                            {
                                std::cout << "token(CAP): " << token << std::endl;
                            }
                            else if (tokens[0] == "USER")
                            {
                                flag = 1;
                                //std::cout << "token(USER): " << token << std::endl;
                                //if (!(tmp.empty())){
                                    tmp += " ";
                                    tmp += token;
                                //}
                            }
                            else if (tokens[0] == "PASS")
                            {
                                std::cout << "token(PASS): " << token << std::endl;
                            }
                            else if (tokens[0] == "NICK")
                            {
                                std::cout << "token(NICK): " << token << std::endl;
                            }
                            else if (tokens[0] == "JOIN")
                            {
                                std::cout << "token(JOIN): " << token << std::endl;
                            }
                            else if (tokens[0] == "MSG")
                            {
                                std::cout << "token(MSG): " << token << std::endl;
                            }
                            else if (tokens[0] == "PRIVMSG")
                            {
                                std::cout << "token(PRIVMSG): " << token << std::endl;
                            }
                            else if (tokens[0] == "CONNECT" || tokens[0] == "connect")
                            {
                                std::cout << "token(CONNECT): " << token << std::endl;
                            }
                            else if (tokens[0] == "quit" || tokens[0] == "QUIT")
                            {
                                std::cout << "token(QUIT): " << token << std::endl;
                            }
                            //
                        }
                        i++;
                        //flag = 1;
                        if (flag == 1)
                        {
                        std::cout <<"U>"<< tmp << std::endl;
                        std::cout <<"U>"<< tmp.substr(4+1+1) << std::endl;
                        }
                    }
                    if (flag == 1)
                    {
                        //std::cout << "token 0:" << tokens[i] << std::endl
                    }
                    // size_t i = 0; i < lines.size(); ++i
                    // for (const auto& line : lines) {
                    // for (size_t i = 0; i < lines.size(); i++) {
                    //     std::istringstream lineStream(line);
                    //     std::string part;
                    //     if (std::getline(lineStream, part, ' '))
                    //     {
                    //         std::cout << part << std::endl;
                    //         if (part == "CAP"){
                    //             cap = line.substr(part.length() + 1);
                    //             std::cout << "==>" << line.substr(part.length() + 1) << std::endl;
                    //         }
                    //         if (part == "PASS"){
                    //             pass = line.substr(part.length() + 1);
                    //         }
                    //         if (part == "NICK"){
                    //             nick = line.substr(part.length() + 1);
                    //         }
                    //         if (part == "USER"){
                    //             user = line.substr(part.length() + 1);
                    //         }
                    //     }
                    // }
                    /* == == == */
                    // std::cout << "0" << cap << std::endl;
                    // std::cout << "1" << pass << std::endl;
                    // std::cout << "2" << nick << std::endl;
                    // std::cout << "3" << user << std::endl;
                    /* == == == */
                    // do {
                    //     std::string part;
                    //     iss >> part;
                    //     parts.push_back(part);
                    // } while (iss);
                    // //std::cout << parts[0] << parts[1]<< std::endl;
                    // //std::string receivedData(buffer);
                    // //std::cout << "" << std::endl;
                    // // handle NICK cmd:
                    // if (parts.size() >= 2 && parts[0] == "/nick") {
                    //     // if user have already a name or wanna change it 
                    //     // +] New Client Connected, Client IP Address 127.0.0.1   (clientId=)
                    //     // throw catch error
                    //     std::cout << "User " << parts[1] << " has set a nickname (" << std::endl;
                    // }
                    // // else if (parts.)
                    // // {
                    // //     //
                    // // }
                    // /* problem in sending receivng cmds */
                    // // /server 127.0.0.1 1337 IRC-1337
                    // else if (parts[0] == "/join") {
                    //     std::cout << "user /join the channel | " << parts[0] << parts[1]  << "|" << std::endl;
                    // }
                    // else if (parts.size() >= 2 && parts[0] == "/PASS") {
                    //     std::cout << "User has been set a password" << std::endl;
                    // }
                    // // == 2 !
                    // else if (parts.size() == 2 && (parts[0] == "/EXIT" || parts[0] == "/QUIT")) {
                    //     std::cout << "User exit the server" << parts.size() << std::endl;
                    // }
                    // else if (receivedBytes == 0) {
                    //     // connection closed by the client handle_disconnection
                    // }
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
