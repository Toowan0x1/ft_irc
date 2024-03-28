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
    std::string cmd, channel, nickname, reason;
    int flag, j;
    
    j = 0;
    if (args >= 4)
    {
        while (iss && j <= args)
        {
            // KICK <channel> <user> [<reason>]
            if (j == 1)             // Command
                iss >> cmd;
            else if (j == 2)        // Channel
                iss >> channel;
            else if (j == 3)        // User Nickname
                iss >> nickname;
            else if (j >= 4)        // Reason
            {
                std::string tmp;

                if (reason.empty())
                    iss >> reason;
                else
                {
                    reason += " ";
                    iss >> tmp;
                    reason += tmp;
                }
            }
            j++;
        }

        // check if channel exist
        flag = 0;
        for (size_t k = 0; k < _channels.size(); ++k) {
            if (_channels[k]->_name.compare(channel) == 0)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            std::string messageToSend = "Channel '" + channel + "' doesn't exist in the server!\n";
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

        // check if the user nickname exists (if the user is an admin, then the admin will r)
        int flag = 0;
        Client *client;
        std::vector<Client *>::iterator it;
        for (it = _clientList.begin(); it != _clientList.end(); ++it)
        {
            client = *it;
            if (client->_nickname.compare(nickname) == 0 && client->_joinedChannel.compare(channel) == 0)
            {
                // if true, notify the admin
                if (client->_userMode.find('o') != std::string::npos)
                {
                    std::string msg = "A user with nickname '~" + _clientList[i]->_nickname + "' is trying to kick you from the channel '" + channel + "'\n";
                    sendMsg(client->_clientFd, msg);
                }
                // flag 1 and quit
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            std::string messageToSend = "This nickname doesn't exist in the channel!\n";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
            return;
        }

        // check auths
        if (!_clientList[i]->_authenticated) {
            std::string messageToSend = "Not authenticated yet! Use '/PASS <Password>' to authenticate to the server.\n";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
            return;
        }

        // check op mode
        if (!containsChar(_clientList[i]->_userMode, 'o')) {
            std::string messageToSend = "You haven't enough permissions to kick users on this channel '" + channel + "' !\n";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
            return;
        }

        // message to send
        std::string messageToSend;
        messageToSend = "";
        messageToSend += "[Channel " + channel + "] ' ~" + client->_nickname + "' kicked from the channel by operator '~" + _clientList[i]->_nickname + "'\n";
        messageToSend +=  "  └─── Reason: '" + reason + "'\n";
        // for loop on clients on channel
        for (std::vector<Client *>::iterator it = _clientList.begin(); it != _clientList.end(); ++it)
        {
            Client *user= *it;
            if (user->_joinedChannel.compare(channel) == 0 && user->_nickname.compare(nickname))
                sendMsg(user->_clientFd, messageToSend);
        }
        // send msg to the kicked user and close connection
        messageToSend = ":" + _hostname + "KICK " + channel + " :";
        messageToSend += "You have been kicked from the channel ";
        messageToSend += "'" + channel + "'";
        messageToSend += " by operator [~" + _clientList[i]->_nickname + "]\n";
        messageToSend += "  └── [Reason]: " + reason + "\n";
        sendMsg(client->_clientFd, messageToSend);

        // remove client from channel
        client->_joined = false;
        client->_joinedChannel = "\0";
        for (std::vector<Client *>::iterator it = _clientList.begin(); it != _clientList.end(); ++it)
        {
            Client *client = *it;
            if (client->_nickname == nickname)
                break;
        }
        
        // remove client from channel
        removeClientFromChannels(client);
        {
            for (size_t x = 0; x < _channels.size(); ++x) {
                if (_channels[x]->_name == channel)
                {
                    // Find the client in the channel's list of clients
                    for (size_t y = 0; y < _channels[x]->_members.size(); ++y)
                    {
                        if (_channels[x]->_members[y] == client)
                        {
                            // Remove the client from the channel's list of clients
                            _channels[x]->_members.erase(_channels[x]->_members.begin() + y);
                            // Optionally notify other clients about the removal
                            break;
                        }
                    }
                    break; // Stop searching for the channel once the client is found and removed
                }
            }
        }
        std::cout << "[CHANNEL] '~" + client->_nickname + "' kicked from channel '" + channel + "' by the operator '~" + _clientList[i]->_nickname + "'\n";
        std::cout << "    └───── Reason: '" + reason + "'\n";
    }
}
