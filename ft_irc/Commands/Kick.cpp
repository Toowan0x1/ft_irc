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
    if (args == 4)
    {
        while (iss && j <= args)
        {
            // KICK <channel> <user> [<reason>]
            if (j == 1)
                iss >> cmd;         // Command
            else if (j == 2)
                iss >> channel;     // Channel
            else if (j == 3)
                iss >> nickname;    // User Nickname
            else if (j == 4)
                iss >> reason;     // Reason
            j++;
        }
        std::cout << "cmd:\t" << cmd << std::endl;
        std::cout << "channel:\t" << channel << std::endl;
        std::cout << "nickname:\t" << nickname << std::endl;
        std::cout << "reason:\t" << reason << std::endl;

        // check if channel exist
        flag = 0;
        std::vector<Channel *> channels;
        for (size_t k = 0; k < channels.size(); ++k) {
            std::cout << "1. channel = " << channel << "=" << std::endl;
            std::cout << "2. channel = " << channels[k]->_name << "=" << std::endl;
            if (channels[k]->_name.compare(channel) == 0 || channels[k]->_name == channel)
            {
                flag = 1;
                std::cout << "1. channel = " << channel << "=" << std::endl;
                std::cout << "2. channel = " << channels[k]->_name << "=" << std::endl;
                break;
            }
        }
        if (flag == 0)
        {
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

        // check if the user nickname exists
        int flag = 0;
        Client *client;
        std::vector<Client *>::iterator it;
        for (it = _clientList.begin(); it != _clientList.end(); ++it)
        {
            client = *it;
            if (client->_nickname.compare(nickname) == 0 && client->_joinedChannel.compare(channel) == 0)
            {
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
        messageToSend = "User Kicked: ";
        messageToSend += reason;
        messageToSend += "\n";
        // for loop on clients on channel
        for (std::vector<Client *>::iterator it = _clientList.begin(); it != _clientList.end(); ++it)
        {
            Client *user= *it;
            if (user->_joinedChannel.compare(channel) == 0 && user->_nickname.compare(nickname))
                sendMsg(user->_clientFd, messageToSend);
        }
        // send msg to the kicked user and close connection
        messageToSend = "You have been kicked from the channel ";
        messageToSend += "'" + channel + "'";
        messageToSend += " by [" + _clientList[i]->_nickname + "]\n";
        messageToSend += "[Reason]: " + reason + "\n";
        sendMsg(client->_clientFd, messageToSend);

        // remove client from channel
        client->_joined = 0;
        client->_joinedChannel = "";
        for (std::vector<Client *>::iterator it = _clientList.begin(); it != _clientList.end(); ++it)
        {
            Client *client = *it;
            if (client->_nickname == nickname)
                break;
        }
        removeClientFromChannels(client);

        std::cout << "client kicked is:\t" << client->_nickname << std::endl;
        // ::
        // msg boot 'user' kicked from 'channel' by the operator 'nickname' <reason>:['']
        /*
        [user] has been kicked from [channel] by the operator [nickname]. Reason: '[reason]'
        */
    }
}

// kick invite leave quit