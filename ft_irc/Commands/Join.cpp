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
    if (args > 1 && args == 2)
    {
        std::string arg1, arg2;
        std::istringstream iss(line);
        int k = 0;
        while (iss && k < args)
        {
            if (k == 0)
                iss >> arg1;
            if (k == 1)
                iss >> arg2;
            k++;
        }
        // channel names MUST start with #
        if (arg2[0] != '#') {
            /**/
        }
        // Creating new channel
        Channel *newChannel = new Channel(arg2);//arg2 channel
        this->_channels.push_back(newChannel);
        // add a user to channel members
        newChannel->_members.push_back(this->_clientList[i]); // clientlist i + 1 -1 
        /* inform everyone that user has joined
            ...
        */
        if (send(this->_clientList[i]->_clientFd, "joined general\n", 16, 0) < 0)
        {
            //
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
