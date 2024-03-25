/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:09:21 by oel-houm          #+#    #+#             */
/*   Updated: 2024/03/25 12:40:50 by oel-houm         ###   ########.fr       */
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

void    Server::Privmsg(std::string line, int i)
{
    int args = countArguments(line);
    std::stringstream iss(line);
    std::string cmd, arg1, msg;
    std::string messageToSend;
    int j = 0;
    while (iss && j <= args) {
        std::string word;
        iss >> word;
        if (j == 0)
            cmd = word;
        else if (j == 1)
            arg1 = word;
        else if (j >= 2)
        {
            if (!msg.empty())
                msg += " ";
            msg += word;
        }
        j++;
    }
    // in case arg1 is a channel
    if (arg1[0] == '#' || arg1 == _clientList[i]->_joinedChannel)
    //if (arg1 == _clientList[i]->_joinedChannel || arg1.compare(_clientList[i]->_joinedChannel) == 0)
    {
        // while loop on memebers and send msg to all of them (from -> to)
        bool channelFound = false;
        for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
        {
            Channel *channel = *it;
            if (channel->_name == arg1)
            {
                channelFound = true;
                size_t l = 0;
                while (l < channel->_members.size())
                {
                    messageToSend = ":" + _clientList[i]->_nickname + " MSG " + channel->_name + " :" + msg + "\n";
                    //std::cout << channel->_members[l]->_nickname << "\n";
                    sendMsg(channel->_members[l]->_clientFd, messageToSend);
                    l++;
                }
                break;
            }
        }
        if (!channelFound)
        {
            messageToSend = "Error: No channel found with the name '" + arg1 + "'!";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
        }
    }

    // in case arg1 is a client member
    else if (!arg1.empty())
    {
        // check that user and check if is in the channel
        bool clientFound = false;
        std::vector<Client *>::iterator it;
        for (it = _clientList.begin(); it != _clientList.end(); ++it) {
            Client *client = *it;
            if (client->_nickname == arg1) {
                clientFound = true;
                messageToSend = ":" + _clientList[i]->_nickname + " MSG " + client->_nickname + " :" + msg + "\n";
                sendMsg(client->_clientFd, messageToSend);
                break;
            }
        }
        if (!clientFound)
        {
            messageToSend = "Error: No user found with the name '" + arg1 + "'!";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
        }
    }
}
