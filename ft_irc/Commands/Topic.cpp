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
        for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
            Channel *channel = *it;
            if (channel->_name == _clientList[i]->_joinedChannel)
            {
                std::string messageToSend;
                if (channel->_topic.empty())
                    channel->_topic = "*";
                messageToSend = ":" + _hostname + " TOPIC " + _clientList[i]->_joinedChannel + " : " + channel->_topic + "\n";
                sendMsg(_clientList[i]->_clientFd, messageToSend);
                break;
            }
        }
        return;
    }
    else if (args >= 2)
    {
        std::stringstream iss(line);
        std::string cmd, topic, tmp;
        std::string messageToSend;
        int j = 0;

        while (iss && j <= args)
        {
            if (j == 1)
                iss >> cmd;     // Command
            else if (j >= 2)
            {
                if (topic.empty())  // Topic
                    iss >> topic;
                else
                {
                    iss >> tmp;
                    topic += " ";
                    topic += tmp;
                }
            }
            j++;
        }

        // check op mode
        if (!containsChar(_clientList[i]->_userMode, 'o')) {
            messageToSend = ":" + _hostname + " TOPIC ERROR " + _clientList[i]->_joinedChannel + " : " + "You're not channel operator\n";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
            return;
        }

        // loop on channels
        std::vector<Channel *>::iterator it;
        for (it = _channels.begin(); it != _channels.end(); ++it)
        {
            Channel *channel = *it;
            if (channel->_name == _clientList[i]->_joinedChannel)
            {
                channel->_topic = topic;
                // server output :
                std::cout << "[" + channel->_name + "] Topic changed -> " + topic + "\n";
                messageToSend = ":" + _hostname + " TOPIC CHANGED " + _clientList[i]->_joinedChannel + " : " + topic + "\n";
                sendMsg(_clientList[i]->_clientFd, messageToSend);
                // inform channel members
                for (size_t l = 0; l < channel->_members.size(); l++)
                {
                    messageToSend = ":" + this->_hostname + " 332 " + this->_clientList[i]->_nickname + " " + channel->_name + " :TOPIC CHANGED: " + topic + "\n";
                    sendMsg(channel->_members[l]->_clientFd, messageToSend);
                }
                break;
            }
        }
    }
}