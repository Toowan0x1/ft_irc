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

//_joinedChannel
// void    Server::removeClientFromChannels(Client *client)
// {
//     //Channel channels;

//     for (size_t i = 0; i < _channels.size(); ++i) {
//         if (_channels[i]->_name == client->_joinedChannel) {
//             _channels[i]->clients.erase(_channels[i]->_members.begin() + j);
//             break;
//         }
//     }
//     // notify other clients about the removal
// }

void Server::removeClientFromChannels(Client *client) {
    for (size_t i = 0; i < _channels.size(); ++i) {
        if (_channels[i]->_name == client->_joinedChannel) {
            // Find the client in the channel's list of clients
            for (size_t j = 0; j < _channels[i]->_members.size(); ++j) {
                if (_channels[i]->_members[j] == client) {
                    // Remove the client from the channel's list of clients
                    _channels[i]->_members.erase(_channels[i]->_members.begin() + j);
                    // Optionally notify other clients about the removal
                    break;
                }
            }
            break; // Stop searching for the channel once the client is found and removed
        }
    }
}

void    Server::handleDisconnection(int i) {
    i = i - 1; // because i = 1 and the index of clients starts from 0
    int fd = this->_pfds[i + 1].fd;
    delete this->_clientList[i];
    this->_clientList.erase(this->_clientList.begin() + i);
    this->_pfds.erase(this->_pfds.begin() + i + 1);
    //removeClientFromChannels(this->_clientList[i]);
    close(fd);
    std::cout << "~" << this->_clientList[i]->_nickname << " has been disconnected" << std::endl;
}

void    Server::handleNewConnection()
{
    struct sockaddr_in clientAddr;
    socklen_t clientSize = sizeof(clientAddr);

    int clientSocketFd = accept(this->_serverSocketFd, (sockaddr*)&clientAddr, &clientSize);
    if (clientSocketFd < 0)
        throw std::runtime_error("ERROR: Problem with client connecting!");
    
    if (fcntl(clientSocketFd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Error setting client socket to non-blocking mode!");
    /*
    int sa = getnameinfo((sockaddr*)&client, sizeof(client), hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
    if (sa == 0)
        std::cout << "Accepted connection on descriptor " << clientSocket << "(host=" << hbuf << ", port=" << sbuf << ")" << std::endl;
    */
    pollfd pfd;
    pfd.fd = clientSocketFd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    this->_pfds.push_back(pfd);

    Client *my_client = new Client(clientSocketFd);
    my_client->_IPAddress = inet_ntoa(clientAddr.sin_addr);
    /* make this peice of code in single function */
    char hostbuffer[256];
    int hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    if (hostname != -1)
        my_client->_hostname = hostbuffer;
    /* == == == == == == == == == == == == == == */
    this->_clientList.push_back(my_client);
    std::cout << "[+] New Client Connected, (IP=" << my_client->_IPAddress << ", HOST=" << my_client->_hostname << ")" << std::endl;
}

void    Server::setServerSocket()
{
    int optValue = 1;
    this->_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_serverSocketFd < 0)
        throw std::runtime_error("Failed to create server socket");
    if (setsockopt(this->_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &optValue, sizeof(optValue)) < 0)
        throw std::runtime_error("Failed to set SO_REUSEADDR option"); 
    if (fcntl(this->_serverSocketFd, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Failed to set socket to non-blocking mode");
}

void    Server::setServerAddrInfo()
{
    memset(&this->_addr, 0, sizeof(this->_addr));

    this->_addr.sin_family = AF_INET;
    this->_addr.sin_addr.s_addr = INADDR_ANY;
    this->_addr.sin_port = htons(this->_port);
    
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0)
        this->_hostname = hostname;
    else
        throw std::runtime_error("Error getting hostname");

    struct pollfd serverPfd;
    serverPfd.fd = this->_serverSocketFd;
    serverPfd.events = POLLIN;
    serverPfd.revents = 0;
    this->_pfds.push_back(serverPfd);
}

void    Server::bindServerSocket()
{
    if (bind(this->_serverSocketFd, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) < 0) {
        throw std::runtime_error("Failed to bind the server socket");
    }
}

bool    Server::startsWith(const std::string &str, const std::string &target)
{
    size_t spacePos = str.find(' '); // "        cmd    " spaces before the cmd!
    std::string firstWord = str.substr(0, spacePos);
    return (firstWord == target);
}

std::size_t     findFirstSpecialChar(std::string str) {
    for (std::size_t i = 0; str.length(); i++) {
        char currentChar = str[i];
        if (currentChar == ' ' || currentChar == '\t' || currentChar == '\n' || currentChar == '\0')
            return (i);
    }
    return std::string::npos;
}

int     countArguments(std::string line) { // countCommandArgs
    std::istringstream iss(line);
    std::string word;
    int count = 0;

    while (iss >> word)
        count++;
    return (count);
}

void    Server::parse_cmd(std::string line, int i) {
    i = i - 1; // because i = 1 and the index of clients starts from 0
    //std::cout << "clientFd=" << i << std::endl;
    // this->_clientList[i]->_buffer = line;
    // std::cout << 0 << ":" << _clientList[i]->_buffer << std::endl;
    // std::cout << "%" << line << "%" << std::endl;
    // if (startsWith(line, "connect") || startsWith(line, "CONNECT")) {
    //     //std::cout << "yes> " << line << std::endl;
    // }
    // else if (startsWith(line, "server") || startsWith(line, "SERVER")) {
    //     /**/
    // }
    if (startsWith(line, "pass") || startsWith(line, "PASS"))
        Pass(line, i);
    else if (startsWith(line, "nick") || startsWith(line, "NICK"))
        Nick(line, i);
    else if (startsWith(line, "user") || startsWith(line, "USER"))
        User(line, i);
    else if (startsWith(line, "join") || startsWith(line, "JOIN"))
        Join(line, i);
    else if (startsWith(line, "quit") || startsWith(line, "QUIT"))
        Quit(line, i);
    else if (startsWith(line, "list") || startsWith(line, "LIST"))
        List(line, i);

    // else if (startsWith(line, "invite") || startsWith(line, "Invite"))
    //     Invite(line, i);
    // else if (startsWith(line, "kick") || startsWith(line, "Kick"))
    //     Kick(line, i);
    // else if (startsWith(line, "mode") || startsWith(line, "Mode"))
    //     Mode(line, i);
    // else if (startsWith(line, "msg") || startsWith(line, "Msg"))
    //     Msg(line, i);
    // else if (startsWith(line, "privmsg") || startsWith(line, "Privmsg"))
    //     Privmsg(line, i);
    // else if (startsWith(line, "topic") || startsWith(line, "Topic"))
    //     Topic(line, i);
    else if (startsWith(line, "who") || startsWith(line, "Who"))
        Who(line, i);

    else if (startsWith(line, "whois") || startsWith(line, "WHOIS"))
        Whois(line, i);
    // realname  _buffer
}

void    Server::AcceptMsg(int i) {
    // use client buffer instead
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));

    int receivedBytes = recv(this->_pfds[i].fd, buffer, sizeof(buffer), 0);
    if (receivedBytes < 0) {
        std::runtime_error("recv error");
    }

    // the recv call will return 0 or a negative value, indicating that the client has closed the connection.
    else if (receivedBytes <= 0) {
        handleDisconnection(i);
    }
    else if (receivedBytes > 0) {
        buffer[receivedBytes] = '\0';
        std::stringstream iss(buffer);
        std::string line;
        while (std::getline(iss, line, '\n'))
        {
            int ii = 0;
            while (line[ii])
            {
                if ((int)line[ii] == 13)
                    line.erase(ii, 1);
                ii++;
            }
            //std::cout << "cmd: $" << line << "$" << std::endl;
            parse_cmd(line, i);
        }
    }
}

void    Server::start() {
    std::cout << "Irc server Launched" << std::endl;
    this->setServerSocket();
    this->setServerAddrInfo();
    this->bindServerSocket();

    /* Listening */
    if (listen(this->_serverSocketFd, SOMAXCONN) < 0) {
        throw std::runtime_error("ERROR: Failed to listen for connections: " + std::string(strerror(errno)));
    }
    std::cout << "[*] Listening ..." << std::endl;
    
    /* non-blocking sockets, are often used in scenarios where you want to handle multiple
    connections or perform other tasks while waiting for socket operations to complete. */
    if (fcntl(this->_serverSocketFd, F_SETFL, O_NONBLOCK) == -1) {
        std::runtime_error("Error setting socket to non-blocking mode");
    }

    /* Our server Loop */
    while (1)
    {
        if (poll(this->_pfds.data(), this->_pfds.size(), -1) < 0)
            throw std::runtime_error("poll failed, error monitoring sockets!");
        for (unsigned int i = 0; i < this->_pfds.size(); i++)
        {
            if (this->_pfds[i].revents == 0)
                continue ;
            if (this->_pfds[i].revents == POLLIN && this->_pfds[i].fd == this->_serverSocketFd)
                handleNewConnection();
            if (this->_pfds[i].revents == POLLIN && this->_pfds[i].fd != this->_serverSocketFd)
                AcceptMsg(i);
            if (this->_pfds[i].revents & POLLHUP || this->_pfds[i].revents & POLLERR)
                handleDisconnection(i);
        }
        // disconnect the client if the _keepAlive boolean is false 
        for (unsigned int i = 0; i < this->_clientList.size(); i++) {
            if (this->_clientList[i]->_keepAlive == 0)
                handleDisconnection(i);
        }
    }
}

Server::Server(std::string port, std::string password) {
    this->_port = atoi(port.c_str());
    if (this->_port == 0 || (this->_port < 0 || this->_port > 65535))
        throw std::runtime_error("Invalid port");
    this->_password = password;
    if (!(this->_password.length() >= 8 && this->_password.length() <= 16))
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

// getnameinfo()
// server loop [3]
// [-] Client Disconnected
// handle password before joining channel
// return "464 :Password incorrect\r\n";
// setRegistred true => false 

// TO DO:
// when someone change his nickname inform all the channel members,




/*
std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream ss(s);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}
*/


/*
:irc.host.com NOTICE * :*** Looking up your hostname...
:irc.host.com NOTICE * :*** Could not resolve your hostname: Domain not found; using your IP address () instead.
*/