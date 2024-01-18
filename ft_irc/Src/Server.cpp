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
    struct sockaddr_in client; // clientAddr
    socklen_t clientSize = sizeof(client);
    int clientSocketFd = accept(this->_serverSocketFd, (sockaddr*)&client, &clientSize);
    if (clientSocketFd < 0)
        throw std::runtime_error(""); // "Problem with client connecting!"
    if (fcntl(clientSocketFd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Error setting client socket to non-blocking mode!");
    //int sa = getnameinfo((sockaddr*)&client, sizeof(client), hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
    // if (sa == 0)
        // std::cout << "Accepted connection on descriptor " << clientSocket << "(host=" << hbuf << ", port=" << sbuf << ")" << std::endl;
    // // adding it to POLL's array of sruct so it get monitored as well 
    pollfd pfd = {clientSocketFd, POLLIN, 0};
    this->_pfds.push_back(pfd);
    Client *my_client = new Client(clientSocketFd);
    my_client->_IPAddress = inet_ntoa(client.sin_addr);
    this->_clientList.push_back(my_client);
    std::cout << "a new client has been connected." << std::endl;
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

// associating an IP address and port number with a socket
void    Server::bindServerSocket() {
    if (bind(this->_serverSocketFd, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) < 0) {
        throw std::runtime_error("Failed to bind server socket to port " + std::to_string(this->_port));
    }
    std::cout << "Server Socket has been created and binded successfully" << std::endl;
}

// addClient Function
// getnameinfo()
// main loop

// acceptConnection || handleNewConnection
// acceptMessage
// handleDisconnection

void    Server::start() {
    this->setServerSocket();
    this->setServerAddrInfo();
    this->bindServerSocket();

    if (listen(this->_serverSocketFd, SOMAXCONN) < 0) {
        throw std::runtime_error("Failed to listen for connections");
        // throw std::runtime_error("Error on listen: " + std::string(strerror(errno)));
    }
    std::cout << "Listening ..." << std::endl;
    
    if (fcntl(this->_serverSocketFd, F_SETFL, O_NONBLOCK) == -1) {
        std::runtime_error("Error setting socket to non-blocking mode");
    }

    while (1) {
        if (poll(this->_pfds.data(), _pfds.size(), -1) < 0) {
            throw std::runtime_error("poll failed !");
        }
        for (int i = 0; i < this->_pfds.size(); i++)
        {
            if (this->_pfds[i].revents == 0)
                continue ;
            if (this->_pfds[i].revents == POLLIN && this->_pfds[i].fd == this->_serverSocketFd)
            // equivalent to "if (this->_fds[i].revents & POLLIN)"
            {
                acceptConnection();
                // else HANDLE INCOMING MSG accept_message();
            }
            if (this->_pfds[i].revents == POLLIN && this->_pfds[i].fd != this->_serverSocketFd)
            // equivalent to "if (this->_fds[0].fd != 0 &&  this->_fds[0].revents == POLLIN)"
            {
                // acceptMessage();
            }
            if (this->_pfds[i].revents == 17 || this->_pfds[i].revents == POLLHUP)
            {
                // if (this->_fds[i].revents & (POLLHUP | POLLERR)) {
                // handle_disconnection();
            }
        }
        // disconnect the client if the _keepAlive boolean is false 
        for (int i = 0; i < this->_clientList.size(); i++)
        {
            if (this->_clientList[i]->_keepAlive) {
                // handle_disconnection(i + 1);
            }
        }
    }
}

Server::Server(std::string port, std::string password) {
    this->_port = atoi(port.c_str());
    if (this->_port == 0 || (this->_port < 0 && this->_port > 65535))
        throw std::runtime_error("Invalid port");
    this->_password = password;
    if (!(this->_password.length() >= 8 && this->_password.length() <= 16)) // != Password || empty
        throw std::runtime_error("Invalid password");
}

Server::~Server() {
    for (int i = 0; i < this->_clientList.size(); i++) {
        delete _clientList[i];
        close(this->_pfds[i + 1].fd);
    }
    if (this->_pfds[0].fd)//this->_pfds.size())
        close(this->_pfds[0].fd);
}
