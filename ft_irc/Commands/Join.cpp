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
        // channel names MUST start with #
        if (channelName[0] == '#') {
            /* Check if the channel already exists */
            Channel *joinedChannel = nullptr;
           // check channel if already exist !!!
            std::vector<Channel *>::iterator it;
            for (it = _channels.begin(); it != _channels.end(); ++it) {
                Channel *channel = *it;
                if (channel->_name == channelName) {
                    joinedChannel = channel;
                    break;
                }
            }
            if (joinedChannel != nullptr) // if exist
            {
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
                    std::cout << "~" << _clientList[i]->_nickname << " has been joined " << channelName << " channel" << std::endl;
                }
                else
                    sendMsg(_clientList[i]->_clientFd, "you are already joined this channel before\n");
            }
            /* if channel doesn't exists, create a new one */
            else if (joinedChannel == nullptr) {
                //std::cout << "joined #X" << std::endl;
                /* Creating new channel */
                Channel *newChannel = new Channel(channelName);
                this->_channels.push_back(newChannel);
                std::cout << "[+] " << channelName << " channel created" << std::endl;
                /* add user to channel members  */
                // if new channel give the client elevated mode/permission on that channel
                newChannel->_members.push_back(_clientList[i]); // clientList i + 1 - 1
                //std::cout << "[+] Client " << " has been joined " << channelName << "channel" << std::endl;
                std::cout << "~" << _clientList[i]->_nickname << " has been joined " << channelName << " channel" << std::endl;
                /* inform everyone that user has joined */

                // Count the number of members in the channel
                // int memberCount = newChannel->_members.size();
                // std::cout << "[*] Total member in channel " << channelName << " is: "<< memberCount << std::endl; 
                
                /**/
                // std::cout << "[*] " << channelName << " Channel members:\n";
                // std::vector<Client *>::iterator it;
                // for (it = newChannel->_members.begin(); it != newChannel->_members.end(); ++it) {
                //     std::cout << " [x] " << (*it)->_nickname << "\n";
                // }
                std::string ss;
                ss = this->_hostname + " 366 " + _clientList[i]->_nickname + " " + channelName + " :End of /Names list.\n";

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

/*
:server-name 366 your-nickname #channel :End of /NAMES list.
*/

/*
check if user have (nickname, username, authenticated with valid pass)
// iforming user joined the chnl in(rfc prototype)
// inform all mmbers of channel the new joined user
// manage channel password (if channel have password / ask new users for pass?)
// msg #channel msg
// msg ~nickname msg
// quit or exit a channel (quit :Leaving)

client with nickname ~toowan created #channel Channel
client joined channel #channel

-------
    banana
    
    [client->server]
    [client->server]:toowan@0 JOIN #seclab
    /
    #hackforums has been created! 
    //check if authenticated and have nick, user, realname etc
    // if user disconnected o dar connect next time, kaytra mochkil dyal double userNIckname
    //
*/