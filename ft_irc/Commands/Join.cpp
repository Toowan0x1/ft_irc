/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 05:15:26 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/30 05:15:28 by oel-houm         ###   ########.fr       */
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

void    Server::Join(std::string line, int i) {
    int args = countArguments(line);
    if (args == 2 || args == 3)
    {
        std::string cmd, channelName, channelPass;
        std::istringstream iss(line);
        int k = 0;
        while (iss && k < args)
        {
            if (k == 0)
                iss >> cmd;
            if (k == 1)
                iss >> channelName;
            if (k == 2)
                iss >> channelPass;
            k++;
        }

        // ability to join just 1 channel
        {
            if (_clientList[i]->_joined == true)
            {
                std::string message;
                message = _hostname + " NOTICE " + _clientList[i]->_nickname + " You are already in a channel. You cannot join another one.\n";
                sendMsg(_clientList[i]->_clientFd, message);
                message = _hostname + " NOTICE " + _clientList[i]->_nickname + " Leave this channel in order to join another one.\n";
                sendMsg(_clientList[i]->_clientFd, message);
                return;
            }
        }

        // check if the user authenticated with valid pass, have user and nick
        {
            if (_clientList[i]->_authenticated == false)
            {
                std::string ss, sr;
                if (_clientList[i]->_nickname.empty())
                {
                    ss = this->_hostname + " 451 * :You have not registred\n";
                    sr = this->_hostname + " NOTICE * :Please enter your IRC server password using the /PASS command before joining channels.\n";
                }
                else
                {
                    ss = this->_hostname + " 451 " + _clientList[i]->_nickname + " :You have not registred\n";
                    sr = this->_hostname + " NOTICE " + _clientList[i]->_nickname + " :Please enter your IRC server password using the /PASS command before joining channels.\n";
                }
                sendMsg(_clientList[i]->_clientFd, ss);
                sendMsg(_clientList[i]->_clientFd, sr);
            }
            if (_clientList[i]->_username.empty())
            {
                std::string ss, sr;
                ss = this->_hostname + " 432 * :Erroneous nickname\n";
                sendMsg(_clientList[i]->_clientFd, ss);
                if (_clientList[i]->_username.empty())
                    sr = this->_hostname + " NOTICE * :Please set a username, mode, and realname before joining channels.\n";
                else
                sr = this->_hostname + " NOTICE " + _clientList[i]->_nickname + " NOTICE * :Please set a username, mode, and realname before joining channels.\n";
                sendMsg(_clientList[i]->_clientFd, sr);
            }
            if (_clientList[i]->_nickname.empty())
            {
                std::string ss, sr;
                ss = this->_hostname + " 431 * :No nickname given\n";
                sendMsg(_clientList[i]->_clientFd, ss);
                if (_clientList[i]->_nickname.empty())
                    sr = this->_hostname + " NOTICE * :Please set a nickname using the /NICK command before joining channels.\n";
                else
                    sr = this->_hostname + " NOTICE " + _clientList[i]->_nickname + " :Please set a nickname using the /NICK command before joining channels.\n";
                sendMsg(_clientList[i]->_clientFd, sr);
            }
            return;
        }

        // channel names MUST start with #
        if (channelName[0] == '#') {
            // Check if the channel already exists
            // check the channel if already exists
            Channel *joinedChannel = NULL;
            std::vector<Channel *>::iterator it;
            for (it = _channels.begin(); it != _channels.end(); ++it) {
                Channel *channel = *it;
                if (channel->_name == channelName) {
                    joinedChannel = channel;
                    break;
                }
            }
            // in case the channel already exists
            if (joinedChannel != NULL)
            {
                // check hasPass
                if (joinedChannel->hasPassword == 1)
                {
                    if (joinedChannel->_pass != channelPass)
                    {
                        std::string message;
                        if (channelPass.empty())
                        {
                            message = _hostname + " NOTICE * :The channel " + channelName + " requires a password to join.\n";
                            sendMsg(_clientList[i]->_clientFd, message);
                        }
                        else if (!channelPass.empty() && joinedChannel->_pass != channelPass)
                        {
                            message = this->_hostname + " 475 " + _clientList[i]->_nickname + " " + channelName + " :Cannot join channel (+k)\n";
                            sendMsg(_clientList[i]->_clientFd, message);
                        }
                        return;
                    }
                }
                // check if user doesnt belongs to the channel if no pushback him
                int userJoined = 0; // ///// // // change this
                std::vector<Client *>::iterator iter;
                for (iter = joinedChannel->_members.begin(); iter != joinedChannel->_members.end(); ++iter) {
                    if ((*iter)->_nickname == _clientList[i]->_nickname) {
                        userJoined = 1;
                        break;
                    }
                }
                if (userJoined == 0)
                {
                    joinedChannel->_members.push_back(_clientList[i]);
                    _clientList[i]->_joined = true;
                    std::cout << "~" << _clientList[i]->_nickname << " has been joined " << channelName << std::endl;

                    std::string tmp = _hostname + " 001 " + _clientList[i]->_nickname + " :Welcome to the channel! Enjoy your stay.\n";
                    sendMsg(_clientList[i]->_clientFd, tmp);

                    /* inform everyone that user has joined */
                    std::vector<Client *>::iterator it;
                    for (it = joinedChannel->_members.begin(); it != joinedChannel->_members.end(); it++)
                    {
                        std::string message = channelName + " :New user " + _clientList[i]->_username + " has joined the channel.\n";
                        if ((*it)->_clientFd != _clientList[i]->_clientFd)
                            sendMsg((*it)->_clientFd, message);
                    }
                }
                else
                    sendMsg(_clientList[i]->_clientFd, "you are already joined this channel before\n");
            }
            // In case the channel doesn't exists before, create a new one
            else if (joinedChannel == NULL) {
                /* Creating new channel */
                Channel *newChannel = new Channel(channelName);
                this->_channels.push_back(newChannel);
                _clientList[i]->_joined = true;
                if (!channelPass.empty())
                {
                    newChannel->hasPassword = 1;
                    newChannel->_pass = channelPass;
                }
                std::cout << channelName << " has been created!" << std::endl;
                /* add user to channel members  */
                // if new channel give the client elevated mode/permission on that channel
                newChannel->_members.push_back(_clientList[i]); // clientList i + 1 - 1
                std::cout << "~" << _clientList[i]->_nickname << " has been joined " << channelName << std::endl;
                
                std::string tmp = _hostname + " 001 " + _clientList[i]->_nickname + " :Welcome to the channel! Enjoy your stay.\n";
                sendMsg(_clientList[i]->_clientFd, tmp);
                
                /* inform channel members that a new user joined */
                std::vector<Client *>::iterator it;
                for (it = newChannel->_members.begin(); it != newChannel->_members.end(); it++)
                {
                    std::string message = channelName + " :New user " + _clientList[i]->_username + " has joined the channel.\n";
                    if ((*it)->_clientFd != _clientList[i]->_clientFd)
                        sendMsg((*it)->_clientFd, message);
                }
            }
        }
    }
    else
    {
        std::string message = "461 " + _clientList[i]->_nickname + " JOIN :Not enough parameters.\n";
        sendMsg(_clientList[i]->_clientFd, message);
    }
}

/*
// msg #channel msg
// msg ~nickname msg
// quit or exit a channel (quit :Leaving)
-------
    banana
    [client->server]
    [client->server]:toowan@0 JOIN #seclab
    // if user disconnected o dar connect next time, kaytra mochkil dyal double userNIckname
*/


/* 
                    * represents the user's nickname (or in the case of 431,
                    a wildcard to indicate that no nickname was given),
                    and #channel represents the channel the user attempted to join.
                */