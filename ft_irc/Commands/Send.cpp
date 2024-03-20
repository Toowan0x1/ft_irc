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
    std::cout << "SEND [ " << line << " ]\n";
	int args = countArguments(line);
    std::string cmd, nickname, fileToSend;
    int j = 0;
    std::stringstream iss(line);
    (void)i;
    while (iss && j <= args)
    {
        // SEND <*> <file>
        if (j == 1)
            iss >> cmd;     // Command
        else if (j == 2)
            iss >> nickname;   // nickname
        else if (j == 3)
            iss >> fileToSend;   // fileToSend
        j++;
    }
    //fileToSend = "Makefile";
    std::string filePath = "/home/toowan/Desktop/ft_irc/" + fileToSend;

    std::ifstream file(filePath.c_str(), std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file for reading." << std::endl;
        return;
    }

    // Read the file content
    std::ostringstream fileContent;
    fileContent << file.rdbuf();
    file.close();

    // Convert file content to string
    std::string fileContentStr = fileContent.str();

    // Encode file content in Base64
    //std::string base64EncodedContent = base64Encode(fileContentStr);

    // Construct the IRC message
    std::string message = "PRIVMSG nickname :" + fileContentStr;

    // Send the file content to the client
    ssize_t bytesSent = send(_clientList[0]->_clientFd, fileContentStr.c_str(), fileContentStr.size(), 0);
    if (bytesSent == -1) {
        std::cerr << "Error: Failed to send file to client." << std::endl;
        return;
    }

    sendMsg(_clientList[i]->_clientFd, "File sent successfully.\n");
}

