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
    // in case arg1 is a channel
    if (arg1[0] == '#' || arg1 == _clientList[i]->_joinedChannel)
    {
        // while loop on memebers and send msg to all of them (from -> to)
        std::vector<Channel *>channels;
        size_t k;
        for (k = 0; k < channels.size(); ++k)
        {
            if (channels[k]->_name == arg1)
                break;
        }
        size_t l;
        std::cout << "channel: " << channels[k] << "\n";
        std::cout << "cmd: " << cmd << "\n";
        std::cout << "arg: " << arg1 << "\n";
        std::cout << "msg: " << msg << "\n";
        for (l = 0; channels[k]->_members.size(); ++l)
        {
            messageToSend = ":" + _clientList[i]->_nickname + " MSG " + channels[k]->_name + ":" + msg + "\n";
            std::cout << channels[k]->_members[l]->_nickname << "\n";
            sendMsg(channels[k]->_members[l]->_clientFd, messageToSend);
        }
    }
    // in case arg1 is a member
    else if (arg1[0] != '#')
    {
        // check that user and check if is in the channel
        
        //messageToSend = ":" + _clientList[i]->_nickname + " MSG " + channels[k]->_name + ":" + msg + "\n";
        //sendMsg(channels[k]->_members[l]->_clientFd, messageToSend);
    }
    else
    {
        // arg1: no channel and no user found with that name
    }
    // sendMsg(_clientList[i]->_clientFd, messageToSend);
}
