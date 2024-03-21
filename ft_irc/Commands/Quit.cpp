/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 17:53:38 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/25 17:53:39 by oel-houm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"
#include "../Include/Client.hpp"
#include "../Include/Channel.hpp"

static int     countArguments(std::string line) {
    std::istringstream iss(line);
    std::string word;
    int count = 0;

    while (iss >> word)
        count++;
    return (count);
}

static void    sendMsg(int fd, std::string msg)
{
    const char *_msg = msg.c_str();
    size_t msgSize = strlen(_msg);
    if (send(fd, _msg, msgSize, 0) < 0) {
        std::cout << "send failed" << std::endl;
    }
}

void    Server::Quit(std::string line, int i) {
    int args = countArguments(line);
    if (args > 1) {
        std::stringstream iss(line);
        std::string cmd, message;

        // loop on arguments
        int j = 0;
        while (iss && j <= args)
        {
            if (j == 1)
                iss >> cmd;     // Command
            else if (j == 2)
                iss >> message; // Message
            j++;
        }
        //handleDisconnection(i + 1);
        this->_clientList[i]->_leaveMsg = message;
        /* Example:
        :username!user@host QUIT :bye guys! */
        std::string messageToSend = ":" + _clientList[i]->_username + "!" + _clientList[i]->_username + "@" + this->_hostname + " QUIT " + this->_clientList[i]->_leaveMsg + "\n";
        // loop on all clients and anounce them that client x has been disconnected
        size_t y = 0;
        while (y < _clientList.size()) { // && _clientList[i]-> joined to #channel
            if (_clientList[y]->_authenticated == 1)
                sendMsg(_clientList[y]->_clientFd, messageToSend);
            y++;
        }
    }
    else {
        // anounce server and clients
        // handleDisconnection(i + 1);
        // the client will be removed from vector stack here
        // and the following line will not works due that
        this->_clientList[i]->_leaveMsg = ":Leaving the server";
        std::string messageToSend = ":" + _clientList[i]->_username + "!" + _clientList[i]->_username + "@" + this->_hostname + " QUIT " + this->_clientList[i]->_leaveMsg + "\n";
        // loop on all clients and anounce them that client x has been disconnected
        size_t j = 0;
        while (j < _clientList.size()) { // && _clientList[i]-> joined to #channel
            sendMsg(_clientList[j]->_clientFd, messageToSend);
            j++;
        }
    }
    //std::cout << "0 >>" << this->_clientList[0]->_nickname << "\n";
    //std::cout << "1 >>" << this->_clientList[1]->_nickname << "\n";
    _clientList[i]->_joined = false;
    _clientList[i]->_joinedChannel = "";
    std::string user;
    if (this->_clientList[i]->_nickname.empty())
        user = this->_clientList[i]->_nickname_tmp;
    else
        user = this->_clientList[i]->_nickname;
    int fd = this->_pfds[i + 1].fd;
    std::cout << "[-] Client ~" << user << " has been disconnected!" << std::endl;
    delete this->_clientList[i];
    this->_clientList.erase(this->_clientList.begin() + i);
    this->_pfds.erase(this->_pfds.begin() + i);
    close(fd);
    //std::cout << "0 >>" << this->_clientList[0]->_nickname << "\n";
    //std::cout << "1 >>" << this->_clientList[1]->_nickname << "\n";
    // removeclient from chqnnel
    std::cout << " └─ CLIENT DELETED" << std::endl;
}

/*
joined this channel before
*/