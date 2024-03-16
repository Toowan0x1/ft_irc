/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 07:03:41 by oel-houm          #+#    #+#             */
/*   Updated: 2024/03/14 07:03:42 by oel-houm         ###   ########.fr       */
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

static bool containsChar(const std::string& str, char ch) {
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == ch) {
            return true;
        }
    }
    return false;
}

void    Server::Topic(std::string line, int i)
{
    int args = countArguments(line);

    if (_clientList[i]->_authenticated == 0 || _clientList[i]->_nickname.empty()
        || _clientList[i]->_username.empty())
    {
        std::string messageToSend = "Not authenticated yet!\n";
        sendMsg(_clientList[i]->_clientFd, messageToSend);
        return;
    }

    if (_clientList[i]->_joined == 0)
    {
        std::string messageToSend = "Not joined a channel yet!\n";
        sendMsg(_clientList[i]->_clientFd, messageToSend);
        return;
    }

    if (args == 1)
    {
        // loop on channels
        std::vector<Channel *>channels;
        size_t k;
        for (k = 0; k < channels.size(); ++k)
        {
            if (channels[k]->_name == _clientList[i]->_joinedChannel)
                {std::cout << "found\n";break;}
        }
        std::cout << _clientList[i]->_joinedChannel << "\n";
        // std::string messageToSend;
        // if (channels[k]->_topic.empty())
        //     channels[k]->_topic = "(NULL)";
        // messageToSend = _clientList[i]->_joinedChannel + " TOPIC: " + channels[k]->_topic + "\n";
        // std::cout << messageToSend;
        // sendMsg(_clientList[i]->_clientFd, messageToSend);
    }
    else if (args == 2)
    {
        std::stringstream iss(line);
        std::string cmd, topic;
        std::string messageToSend;
        int j = 0;

        while (iss && j <= args)
        {
            if (j == 1)
                iss >> cmd;     // Command
            else if (j == 2)
                iss >> topic;   // Topic
            j++;
        }

        // check op mode
        if (!containsChar(_clientList[i]->_userMode, 'o')) {
            messageToSend = "You don't have the privileges permits you to change this channel's topic!\n";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
            return;
        }
        // loop on channels
        std::vector<Channel *> channels;
        for (size_t k = 0; k < channels.size(); ++k) {
            if (channels[k]->_name == _clientList[i]->_joinedChannel)
            {
                channels[k]->_topic = topic;
                // server output :
                std::cout << "[" + channels[k]->_name + "] Topic changed -> " + topic + "\n";
                sendMsg(_clientList[i]->_clientFd, messageToSend);
                // inform channel members
                size_t l = 0;
                while (channels[k]->_members[l])
                {
                    // :<server> 332 <nick> <channel> :<new_topic>
                    messageToSend = ":" + this->_hostname + " 332 " + this->_clientList[i]->_nickname + " " + channels[k]->_name + " :" + topic + "\n";
                    sendMsg(channels[k]->_members[l]->_clientFd, messageToSend);
                    l++;
                }
            }
        }
    }
}
