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
    if (this->_serverSocketFd < 0) // == -1
        throw std::runtime_error("Failed to create socket");
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
    // pollfd server_pfd = {this->_serverSocketFd, POLLIN, 0};
    // this->_pfds.push_back(server_pfd); // this->_pfds[0] = server_pfd;
}

void    Server::start() {
    if (bind(this->_serverSocketFd, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) < 0) {
        throw std::runtime_error("Failed to bind socket"); // ("Failed to bind to " + std::to_string(this->port));
    }
    // std::cout << "Server Socket has been created and binded successfully" << std::endl;
    if (listen(this->_serverSocketFd, 0) < 0) {
        throw std::runtime_error("Failed to listen for connections");
    }
        // throw std::runtime_error("Error on listen: " + std::string(strerror(errno)));
    // std::cout << "Listening ..." << std::endl;
    // set the status flags of the server's socket to non-blocking mode
    if (fcntl(this->_serverSocketFd, F_SETFL, O_NONBLOCK) == -1) {
        std::runtime_error("Error setting socket to non-blocking mode");
    }
    pollfd server_pfd = {this->_serverSocketFd, POLLIN, 0};
    this->_pfds.push_back(server_pfd); // this->_pfds[0] = server_pfd;

    /* -------------------- -------------------- -------------------- -------------------- */
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

Server::~Server() {
    // delete all reserved clients + close clients socket fds
    for (int i = 0; i < this->_clientList.size(); i++) {
        delete _clientList[i];
        close(this->_pfds[i + 1].fd); // i+1 because the i=0 reserved for the server;
    }
    // delete server socket
    /*
        ...
    */
}
