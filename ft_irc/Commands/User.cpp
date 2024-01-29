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
    int count;

    while (iss >> word)
        count++;
    return (count);
}

void    Server::User(std::string line, int clientFd)
{
    int args = countArguments(line);
    if (args >= 5)
    {
        std::stringstream iss(line);
        std::string arg1, arg2, arg3, arg4, arg5, tmp;
        int i = 0;
        while (iss && i <= args)
        {
            if (i == 0) {
                //donothing//
            }
            else if (i == 1)
                iss >> arg1;
            else if (i == 2)
                iss >> arg2;
            else if (i == 3)
                iss >> arg3;
            else if (i == 4)
                iss >> arg4;
            else if (i == 5)
            {
                iss >> tmp;
                arg5 = tmp;
            }
            else if (i > 5)
            {
                arg5 += " ";
                iss >> tmp;
                arg5 += tmp;
            }
            i++;
        }
        i = clientFd;
        // Pick a nick:
        // -<nickname>- This nickname is registred. Please choose a different nickname.
        if (arg3.length() == 1 && (arg3[0] == '0' || arg3[0] == 'i'
                || arg3[0] == 'o' || arg3[0] == 'w'
                || arg3[0] == 'a' || arg3[0] == 'r')) {
                    this->_clientList[i]->_userMode = arg3;
                /* ... */
        } else {
            std::cout << "[syntax error]: error in setting user mode!" << std::endl;
            /* usage: i o w a r */
        }
        /* check double username */
        std::vector<Client *> clientList;
        clientList = this->_clientList;
        size_t t = 0;
        int res = 0;
        while (t < clientList.size()) // > or <=
        {
            if (clientList[t]->_username == arg2)
                res = 1;
            t++;
        }
        if (res == 0)
            this->_clientList[i]->_username = arg2;
        else {
            std::cout << "-" << arg2 << "- This username is registred. Please choose a different username." << std::endl;
        }

        this->_clientList[i]->_username = arg2;
        this->_clientList[i]->_userMode = arg3;
        this->_clientList[i]->_realName = arg5;
    }
}

