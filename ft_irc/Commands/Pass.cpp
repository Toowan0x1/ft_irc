/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 17:52:54 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/25 17:52:55 by oel-houm         ###   ########.fr       */
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
    const char *msssg = msg.c_str();
    size_t msgSize = strlen(msssg);
    if (send(fd, msssg, msgSize, 0) < 0) {
        std::cout << "send failed" << std::endl;
    }
}

void    Server::Pass(std::string line, int i)
{
    int args = countArguments(line);
    if (args > 1)
    {
        std::string cmd = "pass";
        int start = cmd.length() + 1;
        this->_clientList[i]->_authenticated = false;
        this->_clientList[i]->_password = line.substr(start);
        if (this->_clientList[i]->_password.compare(Server::_password) == 0)
        {
            this->_clientList[i]->_authenticated = true;
            std::string msg = ":" + this->_hostname + " 001 " + this->_clientList[i]->_username + " :Welcome to the IRC Network, " + this->_clientList[i]->_username + "!\n";
            sendMsg(_clientList[i]->_clientFd, msg);
        }
        else
        {
            std::string msg = ":" + this->_hostname + " 464 * :Password incorrect\n";
            sendMsg(_clientList[i]->_clientFd, msg);
        }
    }
}

/*

// Valid Password
:irc.example.com 001 username :Welcome to the IRC Network, username!

// Invalid Password
:irc.example.com 464 * :Password incorrect

*/