/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:09:21 by oel-houm          #+#    #+#             */
/*   Updated: 2024/03/04 15:09:22 by oel-houm         ###   ########.fr       */
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

void    Server::Msg(std::string line, int i)
{
    int args = countArguments(line);
    std::stringstream iss;
    std::string cmd, arg1, msg;
    std::string messageToSend;
    int j = 0;
    while (iss && j <= args) {
        if (j == 1)
            iss >> cmd;
        else if (j == 2)
            iss >> arg1;
        else if (j == 3)
            iss >> msg;
        j++;
    }
    if (arg1[0] == '#' && arg1 == _clientList[i]->_joinedChannel)
    {
        //
    }
    else if (arg1[0] != '#')
    {
        //
    }
    else
    {
        // arg1: no channel and no user found with that name
    }
    // sendMsg(_clientList[i]->_clientFd, messageToSend);
}
