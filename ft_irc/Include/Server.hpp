/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:19:52 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/17 11:21:09 by oel-houm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>	
#include <string.h>	
#include <unistd.h>	
#include <stdlib.h>	
#include <sys/types.h>	// For various data types used in system calls
#include <sys/socket.h>	// For socket-related functions
#include <netinet/in.h>	// For Internet address structures
#include <arpa/inet.h>	// For functions to manipulate IP addresses
#include <netdb.h>	// For functions to query DNS (e.g., gethostbyname)
#include <fcntl.h>	// For file control (e.g., fcntl)
#include <errno.h>	// For error handling (errno variable)
#include <sys/poll.h>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <cstring>

#define SIZE 512
#define PASSWORD "IRC-1337"

class Client;

class Server {
    private:
        std::string             _hostname;
        int                     _serverSocketFd;
        std::string             _password;
        int                     _port;
        struct sockaddr_in      _addr;
        // struct pollfd        _clients_pfds[SIZE]; // Managing communication with multiple clients or channels concurrently.
        std::vector<pollfd>     _pfds; // Managing communication with multiple clients or channels concurrently.
        std::vector<Client *>   _clientList;
        //std::vector<Channel *>  _channels; // channelList;
    public:
        Server(std::string port, std::string password);
        void    start();
        void    setServerSocket();
        void    setServerAddrInfo();
        void    bindServerSocket();
        void    acceptConnection();
        void    parse_cmd(std::string, int i);//int i
        bool    startsWith(const std::string&, const std::string&);
        ~Server();

        void AcceptMsg(int i);
};
// clientPfds
// serverPfd
// fcntl
// gns3

#endif
