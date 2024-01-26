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
    // eq to this->_pfds[0] = serverPfd;
    this->_pfds.push_back(serverPfd);
}

void    Server::bindServerSocket() {
    if (bind(this->_serverSocketFd, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) < 0) {
        throw std::runtime_error("Failed to bind the server socket");
    }
}



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

int     countArguments(std::string line) {
    std::istringstream iss(line);
    std::string word;
    int count;

    while (iss >> word)
        count++;
    return (count);
}

void    Server::parse_cmd(std::string line, int i) {
    i = i - 1; // because i = 1 and the index of clients starts from 0
    std::cout << "clientFd=" << i << std::endl;
    // this->_clientList[i]->_buffer = line;
    // std::cout << 0 << ":" << _clientList[i]->_buffer << std::endl;
    // std::cout << "%" << line << "%" << std::endl;
    if (startsWith(line, "connect") || startsWith(line, "CONNECT")) {
        //std::cout << "yes> " << line << std::endl;
    }
    else if (startsWith(line, "pass") || startsWith(line, "PASS")) {
        int args = countArguments(line);
        if (args > 1)
        {
            std::string cmd = "pass";
            int start = cmd.length() + 1;
            this->_clientList[i]->_authenticated = false; // dirha f init <defConstructor>
            this->_clientList[i]->_password = line.substr(start);
            //if (this->_clientList[i]->_password.compare(Server::_password) == 0)
            if (this->_clientList[i]->_password.compare(Server::_password) == 0)
            {
                this->_clientList[i]->_authenticated = true;
            }
        }
    }
    else if (startsWith(line, "server") || startsWith(line, "SERVER")) {
        /**/
    }
    else if (startsWith(line, "nick") || startsWith(line, "NICK")) {
        int args = countArguments(line);
        if (args > 1) {
            std::string cmd = "nick";
            int start = cmd.length() + 1;
            int end = findFirstSpecialChar(line.substr(start));
            this->_clientList[i]->_nickname = line.substr(start, end);
        }
    }
    else if (startsWith(line, "user") || startsWith(line, "USER")) {
        int args = countArguments(line);
        if (args > 1 && args >= 5)
        {
            std::istringstream iss(line);
            std::string arg1, arg2, arg3, arg4, arg5, tmp;
            int ii = 0;
            while (iss && ii <= args)
            {
                if (ii == 0) {
                    /*donothing*/
                }
                else if (ii == 1)
                    iss >> arg1;
                else if (ii == 2)
                    iss >> arg2;
                else if (ii == 3)
                    iss >> arg3;
                else if (ii == 4)
                    iss >> arg4;
                else if (ii == 5)
                {
                    iss >> tmp;
                    arg5 = tmp;
                }
                else if (ii > 5)
                {
                    arg5 += " ";
                    iss >> tmp;
                    arg5 += tmp;
                }
                ii++;
            }
            // Pick a nick:
            // -<nickname>- This nickname is registred. Please choose a different nickname.
            if (arg3.length() == 1 && (arg3[0] == '0' || arg3[0] == 'i'
                    || arg3[0] == 'o' || arg3[0] == 'w'
                    || arg3[0] == 'a' || arg3[0] == 'r')) {
                        this->_clientList[i]->_userMode = arg3;
                    /* ... */
            } else {
                std::cout << "[syntax error]: error in setting user mode!" << std::endl;
                /* usage: i o w a r */
            }
            /* check double username */
            std::vector<Client *> clientList;
            clientList = this->_clientList;
            size_t t = 0;
            int res = 0;
            while (t < clientList.size()) // > or <=
            {
                if (clientList[t]->_username == arg2)
                    res = 1;
                t++;
            }
            if (res == 0)
                this->_clientList[i]->_username = arg2;
            else {
                std::cout << "-" << arg2 << "- This username is registred. Please choose a different username." << std::endl;
            }

            this->_clientList[i]->_username = arg2;
            this->_clientList[i]->_userMode = arg3;
            this->_clientList[i]->_realName = arg5;
        }
    }
    else if (startsWith(line, "join") || startsWith(line, "JOIN")) {
        /* banana */
        /*
        [client->server]
        [client->server]:toowan@0 JOIN #seclab
        */
        /* #hackforums has been created! */
        //check if authenticated and have nick, user, realname etc
        // if user disconnected o dar connect next time, kaytra mochkil dyal double userNIckname
        // 
        int args = countArguments(line);
        if (args > 1 && args == 2)
        {
            std::string arg1, arg2;
            std::istringstream iss(line);
            int k = 0;
            while (iss && k < args)
            {
                if (k == 0)
                    iss >> arg1;
                if (k == 1)
                    iss >> arg2;
                k++;
            }
            // channel names MUST start with #
            if (arg2[0] != '#') {
                /**/
            }
            // Creating new channel
            Channel *newChannel = new Channel(arg2);//arg2 channel
            this->_channels.push_back(newChannel);
            // add a user to channel members
            newChannel->_members.push_back(this->_clientList[i]); // clientlist i + 1 -1 
            /* inform everyone that user has joined
                ...
            */
        }
        else{
            //("461 " + _nickName + " JOIN :Not enough parameters");
            //std::cout << "" << std::endl;
        }
    }
    else if (startsWith(line, "quit") || startsWith(line, "QUIT"))
    {
        int args = countArguments(line);
        if (args > 1) {
            std::string cmd = "quit";
            int start = cmd.length() + 1;
            this->_clientList[i]->_leaveMsg = ":";
            this->_clientList[i]->_leaveMsg += line.substr(start);
            // handle disconnect
        }
        else {
            this->_clientList[i]->_leaveMsg = ":Leaving the server";
            // handle disconnect
            std::string nickname = this->_clientList[i]->_nickname;
            int fd;
            fd = this->_pfds[i+1].fd;
            delete this->_clientList[i];//
            this->_clientList.erase(this->_clientList.begin() + i);//
            this->_pfds.erase(this->_pfds.begin() + i + 1);
            close(fd);
            std::cout << "~" << nickname << " has been disconnected" << std::endl;
        }
    }
    else if (startsWith(line, "whois") || startsWith(line, "WHOIS"))
    {
        int args = countArguments(line);
        if (args > 1) {
            std::string cmd = "whois";
            int start = cmd.length() + 1;
            std::string nickname = line.substr(start);
            if (this->_clientList[i]->_nickname == nickname)
            {
                /* if whois <my_nickname> */
            }
            /* else */

            std::vector<Client *> clientList;
            clientList = this->_clientList;
            size_t i = 0; // change i to j or y or whatever
            while (i < clientList.size())
            {
                if (clientList[i]->_nickname == line.substr(start)) {
                    std::cout << "==========================================================" << std::endl;
                    std::cout << "userName:\t\"" << this->_clientList[i]->_username << "\"" << std::endl;
                    std::cout << "realName:\t\"" << this->_clientList[i]->_realName << "\"" << std::endl;
                    std::cout << "nickName:\t\"" << this->_clientList[i]->_nickname << "\"" << std::endl;
                    std::cout << "userMode:\t\"" << this->_clientList[i]->_userMode << "\"" << std::endl;
                    std::cout << "passWord:\t\"" << this->_clientList[i]->_password << "\"" << std::endl;
                    std::cout << "IP Address:\t\"" << this->_clientList[i]->_IPAddress << "\"" << std::endl;
                    std::cout << "authenticated:\t\"" << this->_clientList[i]->_authenticated << "\"" << std::endl;
                    std::cout << "keepAlive:\t\"" << this->_clientList[i]->_keepAlive << "\"" << std::endl;
                    std::cout << "leaveMsg:\t\"" << this->_clientList[i]->_leaveMsg << "\"" << std::endl;
                    std::cout << "==========================================================" << std::endl;
                    break ;
                }
                ++i; //i++;
            }
        }
    }
    // realname  _buffer
}

void    Server::AcceptMsg(int i) {
    //
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    int receivedBytes = recv(this->_pfds[i].fd, buffer, sizeof(buffer), 0);
    if (receivedBytes < 0) {
        std::runtime_error("recv error");
    }
    // if (bytesRead <= 0) { handle diconnection of client here }
    else if (receivedBytes == 0) { // the recv call will return 0 or a negative value, indicating that the client has closed the connection.
        /* if the client disconnected */
        // handle disconnection here
        i = i -1;
        std::string nickname = this->_clientList[i]->_nickname;
        int fd;
        fd = this->_pfds[i+1].fd;
        delete this->_clientList[i];//
        this->_clientList.erase(this->_clientList.begin() + i);//
        this->_pfds.erase(this->_pfds.begin() + i + 1);
        close(fd);
        std::cout << "~" << nickname << " has been disconnected" << std::endl;
        std::cout << "[-] " << "~" << nickname << " has been disconnected" << std::endl;
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
            parse_cmd(line, i);
        }
    }
}

//void    Server::listen

void    Server::start() {
    std::cout << "Irc server Launched" << std::endl;
    this->setServerSocket();
    this->setServerAddrInfo();
    this->bindServerSocket();

    if (listen(this->_serverSocketFd, SOMAXCONN) < 0) {
        throw std::runtime_error("ERROR: Failed to listen for connections: " + std::string(strerror(errno)));
    }
    std::cout << "[*] Listening ..." << std::endl;
    
    /* non-blocking sockets, are often used in scenarios where you want to handle multiple
    connections or perform other tasks while waiting for socket operations to complete. */
    if (fcntl(this->_serverSocketFd, F_SETFL, O_NONBLOCK) == -1) {
        std::runtime_error("Error setting socket to non-blocking mode");
    }

    while (1) {
        if (poll(this->_pfds.data(), this->_pfds.size(), -1) < 0)
            throw std::runtime_error("poll failed, error monitoring sockets!");
        for (unsigned int i = 0; i < this->_pfds.size(); i++)
        {
            if (this->_pfds[i].revents == 0)
                continue ;
            if (this->_pfds[i].revents == POLLIN && this->_pfds[i].fd == this->_serverSocketFd)
                acceptConnection();
            if (this->_pfds[i].revents == POLLIN && this->_pfds[i].fd != this->_serverSocketFd) {
                AcceptMsg(i); //recvmsgs
            }
            if (this->_pfds[i].revents == 17 || this->_pfds[i].revents == POLLHUP)
            // if (this->_pfds[i].revents & POLLHUP || this->_pfds[i].revents & POLLERR)
            {
                int fd;
                fd = this->_pfds[i].fd;
                delete this->_clientList[i - 1];
                this->_clientList.erase(this->_clientList.begin() + i - 1);
                this->_pfds.erase(this->_pfds.begin() + i);
                close(fd);
                std::cout << "Client disconnected" << std::endl;
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

// getnameinfo()
// server loop [3]
// [-] Client Disconnected
// handle password before joining channel
// return "464 :Password incorrect\r\n";
// setRegistred true => false 