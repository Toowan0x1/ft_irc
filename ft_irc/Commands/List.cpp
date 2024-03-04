/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:48:29 by oel-houm          #+#    #+#             */
/*   Updated: 2024/02/12 16:48:30 by oel-houm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"
#include "../Include/Client.hpp"
#include "../Include/Channel.hpp"

static void    sendMsg(int fd, std::string msg)
{
    const char *_msg = msg.c_str();
    size_t msgSize = strlen(_msg);
    if (send(fd, _msg, msgSize, 0) < 0) {
        std::cout << "send failed" << std::endl;
    }
}

// Function to convert an integer to a string
std::string intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void    Server::List(std::string line, int i) {
    (void)line;
    std::vector<Channel *>::iterator it;
    std::string message;
    message  = _hostname + " 321 " + _clientList[i]->_nickname + " Channel :Users Topic\n";
    sendMsg(_clientList[i]->_clientFd, message);
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        // #channel_name 50 :Channel_topic
        Channel *channel = *it;
        message = _hostname + " 322 " + _clientList[i]->_nickname + " " + channel->_name + " " + intToString(channel->_members.size()) + " :" + channel->_topic + "\n";
        sendMsg(_clientList[i]->_clientFd, message);
    }
    message = _hostname + " 323 " + _clientList[i]->_nickname + " End of channel list\n";
    sendMsg(_clientList[i]->_clientFd, message);
}


/*
// Define a function to send IRC numeric replies
void sendNumericReply(int code, const std::string& message) {
    std::cout << ":" << "irc.server.com" << " " << code << " " << "your_nick" << " :" << message << std::endl;
}

// Simulate sending the RPL_LISTSTART message
sendNumericReply(321, "Channel :Users Name");
*/