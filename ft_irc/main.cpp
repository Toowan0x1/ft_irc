/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:22:36 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/17 11:22:39 by oel-houm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Include/Server.hpp"
#include "Include/Client.hpp"
#include "Include/Channel.hpp"

Server::Server(std::string port, std::string password) {
    this->_port = atoi(port.c_str());
    if (this->_port == 0 || (this->_port < 0 && this->_port > 65535))
        throw std::runtime_error("Invalid port");
    this->_password = password;
    if (!(this->_password.length() >= 8 && this->_password.length() <= 16))
        throw std::runtime_error("Invalid password");
}

void    Server::setServerSocket() {
    int optValue = 1;
    this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_serverSocket < 0) // == -1
        throw std::runtime_error("Failed to create socket");
    // set the SO_REUSEADDR socket option so the TIME_WAIT get ignored
    if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &optValue, sizeof(optValue)) < 0)
        throw std::runtime_error("Failed to set SO_REUSEADDR option");
    if (fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK) < 0)
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
    struct pollfd _pollfd;
    _pollfd.fd = this->_serverSocket; // The file descriptor to monitor for events.
    _pollfd.events = POLLIN; // POLLIN monitoring for incoming data.
    _pollfd.revents = 0; // Initialize revents to 0. This field will be updated by poll() to indicate which events occurred.
    this->_fds[0] = _pollfd;
    /*
    Poll Events:
    - POLLIN: Data can be read.
    - POLLOUT: Data can be written.
    - POLLERR: An error condition occurred.
    - POLLHUP: The file descriptor has been disconnected.
    Revents:
    - The events that actually occurred on the file descriptor.
      After the poll system call returns, this field is filled
      by the kernel to indicate which events occurred.
    */
}

void    Server::start() {
    /*
    1. Create Socket
    Create Epoll
    2. Set addr info
    3. Bind Socket
    4. Listen Socket
    5. Add Socket to epoll
    6. Read Socket */

    /* binding the server */
    if (bind(this->_serverSocket, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) < 0)
    {
        throw std::runtime_error("Failed to bind socket"); // ("Failed to bind to " + std::to_string(this->port));
    }
    // std::cout << "Server Socket has been created and binded successfully" << std::endl;
    /* listening */
    if (listen(this->_serverSocket, 0) < 0)
    {
        throw std::runtime_error("Failed to listen for connections");
        // throw std::runtime_error("Error on listen: " + std::string(strerror(errno)));
    }
    // std::cout << "Listening ..." << std::endl;
    // set the status flags of the server's socket to non-blocking mode
    if (fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK) == -1) {
        std::runtime_error("Error setting socket to non-blocking mode");
    }

    /* ---------- ------------ ----------------- ------------------- */
    // making the first elem of poll structs array (which contain the server socket's FD)
    // pollfd serv_fd = {this->server_fd, POLLIN, 0};
    // this->p_fd.push_back(serv_fd);

    int fdnbr = 1;
    // main loop for our server
    while (1) {
        // monitoring the socket using poll
        // poll(_fds, SIZE, timeout_in_milliseconds);
        // Handle events that occurred on file descriptors.
        /*
        fds: An array of structures describing the file descriptors to be monitored.
        nfds: The number of elements in the fds array.
        timeout: The timeout period in milliseconds (-1 means wait indefinitely).
        */
       // handle different events, such as new connections, incoming messages, and disconnections.
       // poll(this->p_fd.data(), this->p_fd.size(), -1)
        if (poll(this->_fds, SIZE, -1) < 0) {
            throw std::runtime_error("poll failed !");
        }
        //for (int i = 0; i < this->_fds; i++)
        for (int i = 1; i < fdnbr; i++)
        {
            if (this->_fds[i].revents == 0) // come back to it
                continue ;
            if (this->_fds[i].revents == POLLIN && this->_fds[i].fd == this->_serverSocket)
            // equivalent to "if (this->_fds[i].revents & POLLIN)"
            {
                // handle_new_conection();
                // else HANDLE INCOMING MSG accept_message();
            }
            if (this->_fds[i].revents == POLLIN && this->_fds[i].fd != this->_serverSocket)
            // equivalent to "if (this->_fds[0].fd != 0 &&  this->_fds[0].revents == POLLIN)"
            {
                // accept_message();
            }
            if (this->_fds[i].revents == 17 || this->_fds[i].revents == POLLHUP)
            {
                // if (this->_fds[i].revents & (POLLHUP | POLLERR)) {
                // handle_disconnection();
            }
        }
        // disconnect the client if the _keepAlive boolean is false 
        for (int i = 0; i < SIZE; i++) // SIZE = allClients.size() clientList.size()
        {
            // if (this->clientList[i]->_keepAlive)
            // {
            //     handle_disconnection(i + 1);
            // }
        }
    }
}



/* I- multiplexing I/O in a non-blocking way.*/
/*
    this->p_fd.push_back(pd);

    // making the new client class
    Client *my_client = new Client(fd);
    // converting the IPv4 client address from its binary representation to a human-readable string format
    my_client->_IPAddress = inet_ntoa(addr.sin_addr);
    this->all_clients.push_back(my_client);
    logger.info("a new client has connected");
*/
void    Server::handle_new_connection() // accept a connection
{
    struct sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    // creating an FD socket for he new connection
    int clientSocket = accept(this->_serverSocket, (sockaddr*)&client, &clientSize);
    if (clientSocket < 0)
        throw std::runtime_error(""); // "Problem with client connecting!"
    // setting the new socket to non-blocking mode
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error(""); // Error setting socket to nonblocking mode!
    // // adding it to POLL's array of sruct so it get monitored as well 
    pollfd pd; // {,,}
    pd.fd = clientSocket;
    pd.events = POLLIN;
    pd.revents = 0;
    // push_back the created pd to the array of pds
    // this->_pds.push_back(pd) // client_fds!
}
 int     main(int ac, char **av)
{
    if (ac != 3) {
        std::cerr << "Invalid number of arguments." << std::endl;
        std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return 1;
    }
    try {
        Server ircServer(av[1], av[2]);
        ircServer.start();
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}
