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

#include <iostream>     // For standard input/output operations
#include <string.h>     // For string manipulation functions
#include <unistd.h>     // For POSIX API functions (e.g., close, read, write)
#include <stdlib.h>     // For general-purpose functions (e.g., malloc, free)
#include <sys/types.h>  // For various data types used in system calls
#include <sys/socket.h> // For socket-related functions
#include <netinet/in.h> // For Internet address structures
#include <arpa/inet.h>  // For functions to manipulate IP addresses
#include <netdb.h>      // For functions to query DNS (e.g., gethostbyname)
#include <fcntl.h>      // For file control (e.g., fcntl)
#include <errno.h>      // For error handling (errno variable)
#include <sys/poll.h>   // For polling file descriptors
#include <vector>       // For dynamic arrays
#include <map>          // For associative arrays (maps)
#include <sstream>      // For string stream operations
#include <fstream>      // For file stream operations
#include <cstring>      // For C-style string manipulation functions

class Client;
class Channel;

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
        std::vector<Channel *>  _channels; // channelList;
    public:
        Server(std::string port, std::string password);
        void    start();
        void    setServerSocket();
        void    setServerAddrInfo();
        void    bindServerSocket();
        void    handleNewConnection();
        void    parse_cmd(std::string, int i); //int i
        void    User(std::string line, int clientFd);
        void    Pass(std::string line, int clientFd);
        void    Nick(std::string line, int clientFd);
        void    Quit(std::string line, int clientFd);
        void    Join(std::string line, int clientFd);
        void    List(std::string line, int clientFd);

        void    Kick(std::string line, int clientFd);
        void    Mode(std::string line, int clientFd);
        void    Topic(std::string line, int clientFd);
        void    Who(std::string line, int clientFd);
        void    Whois(std::string line, int clientFd);
        void    Msg(std::string line, int clientFd);
        void    Send(std::string line, int clientFd);
        void    Leave(std::string line, int clientFd);
        // void    Privmsg(std::string line, int clientFd);
        // void    Invite(std::string line, int clientFd);
        
        void    removeClientFromChannels(Client *client);
        bool    startsWith(const std::string&, const std::string&);
        ~Server();
        void    AcceptMsg(int i);
        void    handleDisconnection(int i);
        int     countChannelMembers(std::string channelName);

        std::string getClientHostName(Client *client);
        
};
// clientPfds
// serverPfd
// fcntl
// gns3
// packet tracer tb

#endif
