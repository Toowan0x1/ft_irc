/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Whois.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:07:59 by oel-houm          #+#    #+#             */
/*   Updated: 2024/03/04 15:08:00 by oel-houm         ###   ########.fr       */
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

void    Server::Whois(std::string line, int i) {
    if (this->_clientList[i]->_authenticated == false) {
        const char *msg = "Not authenticated yet! Enter a valid password\n";
        sendMsg(this->_clientList[i]->_clientFd, msg);
    }
    else {
        int args = countArguments(line);
        if (args > 1) {
            std::string cmd = "whois";
            int start = cmd.length() + 1;
            std::string nickname = line.substr(start);
            if (this->_clientList[i]->_nickname == nickname)
            {
                /* if whois <my_nickname> */
            }
            /* else */

            std::vector<Client *> clientList;
            clientList = this->_clientList;
            int fddd = i;
            size_t i = 0; // change i to j or y or whatever
            bool found = false;
            while (i < clientList.size())
            {
                if (clientList[i]->_nickname == line.substr(start)) {
                    std::string messageToSend;;
                    messageToSend = "-----------------------------------\n";
                    messageToSend += "- Real Name:      "   + this->_clientList[i]->_realName  + "\n";
                    messageToSend += "- Nickname:       "   + this->_clientList[i]->_nickname  + "\n";
                    messageToSend += "- Username:       "   + this->_clientList[i]->_username  + "\n";
                    messageToSend += "- User Mode:      "   + this->_clientList[i]->_userMode  + "\n";
                    messageToSend += "- Hostname:       "   + this->_clientList[i]->_hostname  + "\n";
                    messageToSend += "- IP Address:     "   + this->_clientList[i]->_IPAddress + "\n";
                    std::string authenticated = "No";
                    if (this->_clientList[i]->_authenticated == true) {
                        authenticated = "Yes";
                    }
                    messageToSend += "- Authenticated:  " + authenticated + "\n";
                    messageToSend += "- Joined Channel: " + this->_clientList[i]->_joinedChannel + "\n";
                    //messageToSend += "|  leaveMsg:\t" + this->_clientList[i]->_leaveMsg + "\t\t\t\t|\n";
                    messageToSend += "-----------------------------------\n";
                    sendMsg(this->_clientList[fddd]->_clientFd, messageToSend);
                    found = true;
                    break ;
                }
                ++i; //i++;
            }
            if (found == false) {
                std::string msgS = "There is no user with nickname ~" + nickname + "\n";
                const char *msgS2 = msgS.c_str();
                sendMsg(this->_clientList[fddd]->_clientFd, msgS2);
            }
        }
    }
}

