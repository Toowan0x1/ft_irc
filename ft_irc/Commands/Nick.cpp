/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 17:53:06 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/25 17:53:07 by oel-houm         ###   ########.fr       */
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

void    Server::Nick(std::string line, int i) {
    int args = countArguments(line);
    if (args > 1 && args == 2) {
        std::istringstream iss(line);
        size_t t = 0;
        std::string arg1, arg2;
        while (iss && t < 2) {
            if (t == 0)
                iss >> arg1;
            if (t == 1)
                iss >> arg2;
            t++;
        }
        t = 0;
        bool isDuplicated = false;
        while (t < _clientList.size()) {
            // what if ur nickname same as the one you are providing in the cmd
            if (_clientList[i]->_nickname.compare(arg2) == 0) { }
            if (_clientList[t]->_nickname.compare(arg2) == 0 && _clientList[t]->_clientFd != _clientList[i]->_clientFd) {
                isDuplicated = true;
                // handle msg: "nickname already used by client(*)"
                std::string messageToSend = "~" + arg2 + " nickname is registred. Please choose a different nickname.\n";
                sendMsg(_clientList[i]->_clientFd, messageToSend);
                break ;
            }
            t++;
        }
        if (!isDuplicated)
        {
            _clientList[i]->_nickname = arg2;
            std::string messageToSend = "Nickname set to: " + arg2 + "\n";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
            std::cout << "[SERVER] ~" << _clientList[i]->_nickname_tmp << " changed his nickname to ~" << _clientList[i]->_nickname << std::endl;
        }
    }
}
