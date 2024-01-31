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
    /* banana */
    /*
    [client->server]
    [client->server]:toowan@0 JOIN #seclab
    */
    /* #hackforums has been created! */
    //check if authenticated and have nick, user, realname etc
    // if user disconnected o dar connect next time, kaytra mochkil dyal double userNIckname
    //
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
        // channel names MUST start with #
        if (channelName[0] == '#') {
            /* Check if the channel already exists */
            Channel *joinedChannel = nullptr;
            /*
            // syntax not aalowed in std-98
            for (Channel *channel : _channels) {
                if (channel->_name == channelName) {
                    joinedChannel = channel;
                    break;
                }
            }
            */
           // nullptr problem
           // 
           int x = 0;
            std::vector<Channel *>::iterator it;
            for (it = _channels.begin(); it != _channels.end(); ++it) {
                Channel *channel = *it;
                x++;
                //std::cout << "Checking channel: " << channel->_name << std::endl;
                if (channel->_name == channelName) {
                    joinedChannel = channel;
                    //std::cout << "Channel found: " << channel->_name << std::endl;
                    //x++;
                    //std::cout << "CHANNEL ALREADY EXSITS:" << channel->_name << std::endl;
                    break;
                }
            }
            //std::cout << "x=" << x << std::endl;
            if (joinedChannel != nullptr) // if exist
            // if (x == 3)
            {
                // check if user doesnt belongs to the channel if no pushback him
                // else, tell him that already joined
                // newChannel->_members.push_back(_clientList[i]);

                //
                std::cout << "Members in the existing " << channelName << ":\n";
                std::vector<Client *>::iterator it;
                for (it = joinedChannel->_members.begin(); it != joinedChannel->_members.end(); ++it) {
                    std::cout << " - " << (*it)->_nickname << "\n";
                }
                //std::cout << std::endl;
            }
            //std::cout << "CHANNEL " << channelName << std::endl;
            /* if channel doesn't exists, create a new one */
            else if (joinedChannel == nullptr) {
                //std::cout << "joined #X" << std::endl;
                /* Creating new channel */
                Channel *newChannel = new Channel(channelName);
                this->_channels.push_back(newChannel);
                /* add user to channel members  */
                // if new channel give the client elevated mode/permission on that channel
                newChannel->_members.push_back(_clientList[i]); // clientList i + 1 - 1
                /* inform everyone that user has joined */

                // Count the number of members in the channel
                int memberCount = newChannel->_members.size();
                std::cout << "[*] Total member in channel " << channelName << " is: "<< memberCount << std::endl; 
                
                /**/
                std::cout << "[*] " << channelName << " Members:\n";
                std::vector<Client *>::iterator it;
                for (it = newChannel->_members.begin(); it != newChannel->_members.end(); ++it) {
                    std::cout << " - " << (*it)->_nickname << "\n";
                }
                //std::cout << std::endl;
            }
            // /* inform everyone that user has joined */
            // size_t j = 0;
            // size_t m = newChannel->_members.size();
            // // while (j < Channel::_members)
            // // {
            // //     //
            // // }
        }
        else
        {
            /**/
        }
    }
    else
    {
        /* Example:
        ("461 " + _nickName + " JOIN :Not enough parameters") */
        std::string msg = "461 " + _clientList[i]->_nickname + "JOIN :Not enough parameters";
        sendMsg(_clientList[i]->_clientFd, msg);
        //std::cout << "" << std::endl;
    }
}
