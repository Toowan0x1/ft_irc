/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:08:54 by oel-houm          #+#    #+#             */
/*   Updated: 2024/03/04 15:08:55 by oel-houm         ###   ########.fr       */
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

void    Server::Mode(std::string line, int i) {
    (void)i;
    (void)line;
    int args = countArguments(line);
    if (args > 1 && args < 4) {
        std::stringstream iss(line);
        std::string cmd, arg1, mode;

        // loop on arguments
        int j = 0;
        while (iss && j <= args)
        {
            if (j == 1)
                iss >> cmd;     // Command
            else if (j == 2)
                iss >> arg1;    // Channel, or Nickname
            else if (j == 3)
                iss >> mode;    // Mode
            j++;
        }

        // Setting both channel, and user modes
        int flag = 0;
        if (args == 3)
        {
            // Setting Channel Modes
            // if (arg1[0] == '#') {
            //     std::vector<Channel *>::iterator it;
            //     for (it = _channels.begin(); it != _channels.end(); ++it)
            //     {
            //         Channel *channel = *it;
            //         if (channel->_name == arg1)
            //         {
            //             flag = 1;
            //             break;
            //         }
            //     }
            //     if (flag == 1)
            //     {
            //         // mode channel 
            //     }
            //     else
            //     {
            //         //
            //     }
            // }

            // Setting User Modes
            if (1) {
                std::vector<Client *>::iterator it;
                for (it = _clientList.begin(); it != _clientList.end(); ++it)
                {
                    Client *client = *it;
                    if (client->_nickname == arg1)
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 1)
                {
                    if (_clientList[i]->_userMode)
                }
                else
                {
                    std::string messageToSend = "";
                    sendMsg(_clientList[i]->_clientFd, messageToSend);
                }
            }
        }
        // Querying User or Channel Modes
        else if (args == 2)
        {
            //
        }
    }
}

/*

MODE #channel +p    // Set private mode
MODE #channel -m    // Unset moderated mode

MODE nickname +i    // Set invisible mode
MODE nickname -a    // Unset away mode

MODE nickname       // Query user modes
MODE #channel       // Query channel modes

*/