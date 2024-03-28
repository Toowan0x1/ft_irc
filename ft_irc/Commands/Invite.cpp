/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:11:21 by oel-houm          #+#    #+#             */
/*   Updated: 2024/03/04 15:11:22 by oel-houm         ###   ########.fr       */
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

void    Server::Invite(std::string line, int i)
{
    int args = countArguments(line);
    if (args == 3)
    {
        std::stringstream iss(line);
        std::string cmd, nickname, channelName;
        int flag, j;
        
        j = 0;
        while (iss && j <= args)
        {
            if (j == 1)
                iss >> cmd;
            else if (j == 2)
                iss >> nickname;
            else if (j == 3)
                iss >> channelName;
            j++;
        }

        // 1. check user 
        std::vector<Client *>::iterator it;
        Client *client;
        flag = 0;
        for (it = _clientList.begin(); it != _clientList.end(); ++it)
        {
            client = *it;
            if (client->_nickname.compare(nickname) == 0)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            sendMsg(_clientList[i]->_clientFd, "ERROR: User not found\n");
            return;
        }

        // 2. check channel
        flag = 0;
        for (size_t k = 0; k < _channels.size(); ++k)
        {
            if (_channels[k]->_name == channelName)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            sendMsg(_clientList[i]->_clientFd, "ERROR: Channel not found\n");
            return;
        }

        // 3. invite 
        std::string successMsg = "SUCCESS: User '~"+ client->_nickname + "' invited to join channel space '" + channelName + "'.";
        sendMsg(_clientList[i]->_clientFd, successMsg + "\n");
        std::string inviteMsg = "INVITE: '~" + _clientList[i]->_nickname + "' invited you to join channel '" + channelName + "' chat room.";
        sendMsg(client->_clientFd, inviteMsg + "\n");
    }
}
