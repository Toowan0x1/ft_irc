/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 19:20:46 by oel-houm          #+#    #+#             */
/*   Updated: 2024/03/20 19:22:26 by oel-houm         ###   ########.fr       */
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
//#include <hexchat-plugin.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>

// Pseudocode to send a file to HexChat
// void sendFileToHexChat(const std::string& targetUser, const std::string& filePath) {
//     // Read the file contents
//     std::ifstream file(filePath, std::ios::binary);
//     if (!file.is_open()) {
//         // Handle file not found error
//         return;
//     }
//     std::ostringstream oss;
//     oss << file.rdbuf();
//     std::string fileContents = oss.str();

//     // Send a message to HexChat with the file contents or file path
//     std::string message = "PRIVMSG " + targetUser + " :" + filePath; // Or use fileContents if you want to send the file directly
//     // Send 'message' to HexChat via IRC protocol
// }

void	Server::Send(std::string line, int i)
{
	int args = countArguments(line);
    std::string cmd, arg1, fileToSend;
    std::string messageToSend;
    int j = 0;
    std::stringstream iss(line);
    (void)i;
    while (iss && j <= args)
    {
        // SEND <*> <file>
        if (j == 1)
            iss >> cmd;     // Command
        else if (j == 2)
            iss >> arg1;   // nickname or channel
        else if (j == 3)
            iss >> fileToSend;   // fileToSend
        j++;
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        sendMsg(_clientList[i]->_clientFd, "Error: Unable to get current working directory.\n");
        return;
    }
    std::string currentPath(cwd);
    std::string filePath = currentPath + "/" + fileToSend;
    std::cout << filePath << std::endl;
    std::ifstream file(filePath.c_str(), std::ios::binary);
    if (!file.is_open()) {
        sendMsg(_clientList[i]->_clientFd, "Error: Failed to open file for reading: " + filePath + "/" + ".\n");
        return;
    }

    // Read the file content
    std::ostringstream fileContent;
    fileContent << file.rdbuf();
    file.close();

    // Convert file content to string
    std::string fileContentStr = fileContent.str();

    // in case arg1 is a channel
    if (arg1[0] == '#' || arg1 == _clientList[i]->_joinedChannel)
    {
        //
        bool channelFound = false;
        for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
        {
            Channel *channel = *it;
            if (channel->_name == arg1)
            {
                channelFound = true;
                size_t l = 0;
                while (l < channel->_members.size())
                {
                    messageToSend = ":" + _clientList[i]->_nickname + " MSG " + channel->_name + " :" + fileContentStr + "\n";
                    ssize_t bytesSent = send(channel->_members[l]->_clientFd, fileContentStr.c_str(), fileContentStr.size(), 0);
                    if (bytesSent == -1) {
                        sendMsg(_clientList[i]->_clientFd, "Error: Failed to send file to client.\n");
                        return;
                    }
                    l++;
                }
                sendMsg(_clientList[i]->_clientFd, "File sent successfully.\n");
                break;
            }
        }
        if (!channelFound)
        {
            messageToSend = "Error: No channel found with the name '" + arg1 + "'!";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
        }
    }

    // in case arg1 is a client member
    else if (!arg1.empty())
    {
        // check that user and check if is in the channel
        bool clientFound = false;
        std::vector<Client *>::iterator it;
        for (it = _clientList.begin(); it != _clientList.end(); ++it) {
            Client *client = *it;
            if (client->_nickname == arg1) {
                clientFound = true;
                messageToSend = ":" + _clientList[i]->_nickname + " MSG " + client->_nickname + " :" + fileContentStr + "\n";
                ssize_t bytesSent = send(client->_clientFd, fileContentStr.c_str(), fileContentStr.size(), 0);
                if (bytesSent == -1) {
                    sendMsg(_clientList[i]->_clientFd, "Error: Failed to send file to client.\n");
                    return;
                }
                sendMsg(_clientList[i]->_clientFd, "File sent successfully.\n");
                break;
            }
        }
        if (!clientFound)
        {
            messageToSend = "Error: No user found with the name '" + arg1 + "'!";
            sendMsg(_clientList[i]->_clientFd, messageToSend);
        }
    }


    /////////

    

    
}

