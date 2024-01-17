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
#define PASSWORD "password123456"

class Server {
    private:
        std::string             _hostname;
        int                     _serverSocket;
        std::string             _password;
        int                     _port;
        struct soketaddr_in     _addr;
        struct pollfd           _fds[SIZE]; // Managing communication with multiple clients or channels concurrently.
        std::vector<Client *>   clientList;
        std::vector<Channel *>  _channels; // channelList;
    public:
        Server(std::string port, std::string password);
        void    start();
        void    handle_new_connection();
        // Server();
};
