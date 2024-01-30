/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 17:53:19 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/25 17:53:20 by oel-houm         ###   ########.fr       */
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

bool isValidString(const std::string& str) {
    // Check if the first character is a colon
    if (!str.empty() && str[0] == ':') {
        return true;  // Valid, don't perform further checks
    }

    // Check the remaining characters
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        char c = *it;
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
            return false;  // Character is not a letter or space
        }
    }
    return true;  // All characters are valid
}

// bool isValidString(const std::string& str)
// {
//     for (char c : str) {
//         if (!(std::isalpha(c) || c == ' ' || c == '\t' || c == '\v')) {
//             return false;  // Character is not a letter or space
//         }
//     }
//     return true;  // All characters are valid
// }

// Example: /user <username> <usermode> * :<realname>
void    Server::User(std::string line, int i)
{
    int args = countArguments(line);
    if (args >= 5)
    {
        std::stringstream iss(line);
        std::string cmd, username, usermode, hostname, realname, tmp;

        // loop on arguments
        int j = 0;
        while (iss && j <= args)
        {
            if (j == 1)
                iss >> cmd;
            else if (j == 2)
                iss >> username;
            else if (j == 3)
                iss >> usermode;
            else if (j == 4)
                iss >> hostname; // Placeholder for hostname or Ip.
            else if (j == 5)
            {
                iss >> tmp;
                realname = tmp;
            }
            else if (j > 5)
            {
                realname += " ";
                iss >> tmp;
                realname += tmp;
            }
            j++;
        }

        // check usermodes:
        if (usermode.length() == 1 && (usermode[0] == '0' || usermode[0] == 'i'
                || usermode[0] == 'o' || usermode[0] == 'w'
                || usermode[0] == 'a' || usermode[0] == 'r'))
        {
                this->_clientList[i]->_userMode = usermode;
                std::string msg = "Usermode set to: " + usermode + "\n";
                sendMsg(_clientList[i]->_clientFd, msg);
        }
        else {
            sendMsg(this->_clientList[i]->_clientFd,"Invalid mode. Please enter one of the valid modes (i, o, w, a, r).\n");
        }

        // check double username:
        std::vector<Client *> clientList;
        clientList = this->_clientList;
        size_t k = 0;
        int res = 0;
        while (k < clientList.size()) // or <=
        {
            if (clientList[k]->_username == username)
                res = 1;
            k++;
        }
        if (res == 0)
        {
            this->_clientList[i]->_username = username;
            std::string msg = "Username set to: " + username + "\n";
            sendMsg(_clientList[i]->_clientFd, msg);
        }
        else {
            if (this->_clientList[i]->_username != username)
            {
                std::string msg = "@" + username + " username is registred. Please choose a different username.\n";
                sendMsg(this->_clientList[i]->_clientFd, msg);
            }
            else
            { // anounce user that username set to ....
                std::string msg = "Username set to: " + username + "\n";
                sendMsg(_clientList[i]->_clientFd, msg);
            }
        }

        // check realname:
        if (realname.length() >= 35 || isValidString(realname) == 0)
        {
            std::string msg = "Invalid realname. Please ensure it has at most 35 characters and contains only letters.\n"; ///
            sendMsg(this->_clientList[i]->_clientFd, msg);
        }
        else
        {
            this->_clientList[i]->_realName = realname;
            std::string msg = "Realname set to: " + realname + "\n";
            sendMsg(_clientList[i]->_clientFd, msg);
        }
    }
}

/*
USER johnq * * :John Q. Hacker
NICK johnq

:irc.host.com NOTICE * :*** Looking up your hostname...
:irc.host.com NOTICE * :*** Could not resolve your hostname: Domain not found; using your IP address () instead.

CTRL+D
*/