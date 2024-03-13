/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:11:00 by oel-houm          #+#    #+#             */
/*   Updated: 2024/03/04 15:11:01 by oel-houm         ###   ########.fr       */
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

// kick channel nick msg
void    Server::Kick(std::string line, int i)
{
    int args = countArguments(line);
    std::stringstream iss(line);
    std::string cmd, channel, nick, msg;
    int flag, j;
    
    j = 0;
    if (args == 4)
    {
        while (iss && j <= args)
        {
            if (j == 1)
                iss >> cmd;     // Command
            else if (j == 2)
                iss >> channel; // Channel
            else if (j == 3)
                iss >> nick;    // Nick
            else if (j == 4)
                iss >> msg;     // Msg
            j++;
        }

        // check if channel exist
        flag = 0;
        std::vector<Channel *> channels;
        for (size_t k = 0; k < channels.size(); ++k) {
            if (channels[k]->_name == channel)
            {
                flag = 1;
                break;
            }
        } if (flag == 0) {
            std::string messageToSend = "This channel doesn't exist in the server!\n";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
            return;
        }

        // check if the op is on that channel
        if (_clientList[i]->_joinedChannel != channel)
        {
            std::string messageToSend = "You are not a member on that channel!\n";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
            return;
        }

        // check if nick exists
        int flag = 0;
        std::vector<Client *>::iterator it;
        for (it = _clientList.begin(); it != _clientList.end(); ++it) {
            Client *client = *it;
            if (client->_nickname == nick && client->_joinedChannel == channel) {
                flag = 1;
                break;
            }
        } if (flag == 0) {
            std::string messageToSend = "This nickname doesn't exist in the channel!\n";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
            return;
        }

        // check auths
        if (!_clientList[i]->_authenticated) {
            std::string messageToSend = "\n";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
            return;
        }

        // check op mode
        if (!containsChar(_clientList[i]->_userMode, 'o')) {
            std::string messageToSend = "You don't have operator permission to kick users in that channel!\n";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
            return;
        }

        //
    }
}
