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
        //handleDisconnection(i + 1);
        std::string cmd = "quit";
        int start = cmd.length() + 1;
        this->_clientList[i]->_leaveMsg = line.substr(start); // delete / freeing the buffer
        /* Example:
        :username!user@host QUIT :bye guys! */
        std::string messageToSend = ":" + _clientList[i]->_username + "!" + _clientList[i]->_username + "@" + this->_hostname + " QUIT " + this->_clientList[i]->_leaveMsg + "\n";
        // loop on all clients and anounce them that client x has been disconnected
        size_t j = 0;
        while (j < _clientList.size()) { // && _clientList[i]-> joined to #channel
            if (_clientList[j]->_authenticated == 1)
                sendMsg(_clientList[j]->_clientFd, messageToSend);
            j++;
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
    _clientList[i]->_joined = false;
}
