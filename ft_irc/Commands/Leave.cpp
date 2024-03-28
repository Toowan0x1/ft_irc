/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Leave.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 20:48:14 by oel-houm          #+#    #+#             */
/*   Updated: 2024/03/20 20:48:15 by oel-houm         ###   ########.fr       */
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

// leave <channelName> <leaveMsg>

void    Server::Leave(std::string line, int i)
{
    int args = countArguments(line);
    std::string cmd, channelName, leaveMsg;
    std::stringstream iss(line);
    
    int k = 0;
    while (iss && k <= args) {
        std::string word;
        iss >> word;
        if (k == 0)
            cmd = word;
        else if (k == 1)
            channelName = word;
        else if (k >= 2)
        {
            if (!leaveMsg.empty())
                leaveMsg += " ";
            leaveMsg += word;
        }
        k++;
    }

    // get channel first
    Channel *channel;
    for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        channel = *it;
        if (channel->_name == channelName)
            break;
    }

    // check joined 
    if (_clientList[i]->_joined == 1 && _clientList[i]->_joinedChannel.empty() == false) {
        _clientList[i]->_joined = 0;
        _clientList[i]->_joinedChannel = "";
    } else {
        sendMsg(_clientList[i]->_clientFd, "Sorry, you are not currently in any channels to leave.\n");
    }
    
    // leave channel
    removeClientFromChannels(_clientList[i]);

    // anounce message to all channel members
    for (size_t j = 0; j < channel->_members.size(); ++j)
    {
        if (channel->_members[j]->_nickname != _clientList[i]->_nickname)
        {
            std::string msg1 = "~" + _clientList[i]->_nickname + " has left the chat.\n";
            sendMsg(channel->_members[j]->_clientFd, msg1);
            std::string msg2 = ":" + _clientList[i]->_nickname + "!" + _clientList[i]->_nickname + "@" + _hostname + " LEAVE :" + leaveMsg + "\n";
            sendMsg(channel->_members[j]->_clientFd, msg2);
        }
    }

    // anounce message to user after leaving
    sendMsg(_clientList[i]->_clientFd, "You have successfully left the chat room " + channelName + ".\n");
}
