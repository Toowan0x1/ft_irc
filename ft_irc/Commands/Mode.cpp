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

// static int hasCharacter(const std::string& str, char ch) {
//     return (str.find(ch) != std::string::npos) ? 1 : 0;
// }

static int	isValidMode(const std::string& mode, int clientFd)
{
	int i = 0;
    int flag = 1;
    if (mode[0] == '-' || mode[0] == '+')
        i++;
	while (mode[i])
	{
		if (mode[i] == 'i' || mode[i] == 'o' || mode[i] == 'w'
				|| mode[i] == 'a' || mode[i] == 'r')
		{
            /* do nothing */
        }
		else {
			std::string messageToSend = "incorrect user mode !\n";
			sendMsg(clientFd, messageToSend);
            flag = 0;
			break;
		}
		i++;
	}
    return flag;
}

void    Server::Mode(std::string line, int i)
{
    int args = countArguments(line);
    if (args > 1 && args < 4) {
        std::stringstream iss(line);
        std::string cmd, arg1, mode;

        // loop on arguments
        int j = 0;
        if (args == 3)
        {
            while (iss && j <= args)
            {
                if (j == 1)
                    iss >> cmd;     // Command
                else if (j == 2)
                    iss >> arg1;    // User Nickname
                else if (j == 3)
                    iss >> mode;    // Mode
                j++;
            }
        }
        else if (args == 2)
        {
            while (iss && j <= args)
            {
                if (j == 1)
                    iss >> cmd;     // Command
                else if (j == 2)
                    iss >> arg1;    // User Nickname
                j++;
            }
        }

        // Setting user modes
        int flag = 0;
        if (args == 3)
        {
            std::cout << "cmd = " << cmd << " ,arg1 = " << arg1 << " ,mode = " << mode << "\n";
            //
            std::vector<Client *>::iterator it;
            for (it = _clientList.begin(); it != _clientList.end(); ++it)
            {
                Client *client = *it;
                if (client->_nickname == arg1)
                {
                    // check the mode if begin with - or + or one of the user modes iowar
                    int j = 1;
                    if (mode[0] == '-')
                    {
                        std::string tmp;
                        std::cout << "-<mode>\n";
                        // check mode str (improve this part later)
                        if (isValidMode(mode, _clientList[i]->_clientFd))
                        {
                            while (mode[j])
                            {
                                if (mode[j] == 'i' || mode[j] == 'o' || mode[j] == 'w'
                                    || mode[j] == 'a' || mode[j] == 'r')
                                {
                                    // -io
                                    // wairo
                                    int k = 0;
                                    while (client->_userMode[k])
                                    {
                                        if (client->_userMode[k] != mode[k])
                                        {
                                            tmp += client->_userMode[k];
                                        }
                                        k++;
                                    }
                                    client->_userMode = tmp;
                                }
                                j++;
                            }
                        }
                    }
                    else if (mode[0] == '+')
                    {
                        std::cout << "+<mode>\n";
                        std::string tmp;
                        if (isValidMode(mode, _clientList[i]->_clientFd))
                        {
                            while (mode[j]) {
                                if (mode[j] == 'i' || mode[j] == 'o' || mode[j] == 'w'
                                    || mode[j] == 'a' || mode[j] == 'r')
                                {
                                    // +wa
                                    // iro
                                    int k = 0;
                                    while (client->_userMode[k]) {
                                        if (client->_userMode[k] != mode[j]) {
                                            tmp += mode[j];
                                        }
                                        k++;
                                    }
                                    client->_userMode += tmp;
                                }
                                j++;
                            }
                        }
                    }
                    else
                    {
                        // invalid user mode to set
                        std::string messageToSend = "incorrect user mode !\n";
                        sendMsg(_clientList[i]->_clientFd, messageToSend);
                    }
                    flag = 1;
                    break;
                }
            }
            if (!flag)
            {
                // nickname username not found 
                std::string messageToSend = "user ot found !\n";
                sendMsg(_clientList[i]->_clientFd, messageToSend);
            }
        }

        // Querying User Modes
        else if (args == 2)
        {
            std::vector<Client *>::iterator it;
            for (it = _clientList.begin(); it != _clientList.end(); ++it) {
                Client *client = *it;
                if (client->_nickname == arg1) {
                    std::string messageToSend = "MODE " + arg1 + " " + client->_userMode + "\n";
                    std::cout << messageToSend;
                    sendMsg(_clientList[i]->_clientFd, messageToSend);
                    flag = 1;
                    break;
                }
            }
            if (!flag)
            {
                std::string messageToSend = "";
                sendMsg(_clientList[i]->_clientFd, messageToSend);
            }
        }
    }
}
/*
MODE nickname +i    // Set invisible mode
MODE nickname -a    // Unset away mode

MODE nickname       // Query user modes
*/