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

/*
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
            // do nothing
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
}*/

int isValidMode(const std::string& mode, int)
{
    if (mode[0] == '-' || mode[0] == '+')
    {
        int i = 1; // Start from index 1, assuming index 0 is already checked
        while (mode[i])
        {
            // Your loop logic here
            ++i; // Increment index
        }
    }
    return 0; // Return something appropriate
}


static std::string removeChars(const std::string& original, const std::string& removeChars) {
    std::string result;

    for (size_t i = 0; i < original.length(); ++i) {
        // Check if the current character exists in removeChars
        if (removeChars.find(original[i]) == std::string::npos) {
            // If not found, append it to the result string
            result += original[i];
        }
    }

    return result;
}

static  std::string removeDuplicated(std::string& str)
{
    std::string result;
    for (std::string::size_type i = 0; i < str.length(); ++i) {
        if (result.find(str[i]) == std::string::npos) /////////*****
            result += str[i];
    }
    return result;
}

void    Server::Mode(std::string line, int i)
{
    if (_clientList[i]->_joinedChannel.empty())
    {
        std::string messageToSend = "You cannot use MODE command\nJoin a channel to use MODE\n";
        sendMsg(_clientList[i]->_clientFd, messageToSend);
        return ;
    }

    int args = countArguments(line);
    if (args == 2 || args == 3) {
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
            std::vector<Client *>::iterator it;
            for (it = _clientList.begin(); it != _clientList.end(); ++it)
            {
                Client *client = *it;
                if (client->_nickname == arg1)
                {
                    if (mode[0] == '-')
                    {
                        std::string tmp;
                        std::cout << "-<mode>\n";
                        // check mode str (improve this part later)
                        if (isValidMode(mode, _clientList[i]->_clientFd))
                        {
                            client->_userMode = removeChars(client->_userMode, mode.substr(1));
                            std::cout << "tmp=> " << client->_userMode << "\n";
                            // solve duplicated issue
                            client->_userMode = removeDuplicated(client->_userMode);
                        }
                    }
                    else if (mode[0] == '+')
                    {
                        std::string tmp;
                        if (isValidMode(mode, _clientList[i]->_clientFd))
                        {
                            int j = 1;
                            while (mode[j])
                                client->_userMode += mode[j++];
                            client->_userMode = removeDuplicated(client->_userMode);
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
                    std::string messageToSend;
                    if (client->_joinedChannel != _clientList[i]->_joinedChannel)
                    {
                        messageToSend = "You cannot query user mode of a user who's not in the same channel as you.\n";
                        sendMsg(_clientList[i]->_clientFd, messageToSend);
                        return;
                    }
                    if (client->_userMode.empty())
                        messageToSend = "MODE " + arg1 + " " + "0" + "\n";
                    else
                        messageToSend = "MODE " + arg1 + " " + client->_userMode + "\n";
                    sendMsg(_clientList[i]->_clientFd, messageToSend);
                    flag = 1;
                    break;
                }
            }
            if (!flag)
            {
                std::string messageToSend = "user ot found !\n";
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