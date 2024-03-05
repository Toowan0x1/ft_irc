/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:45:37 by oel-houm          #+#    #+#             */
/*   Updated: 2024/03/05 16:45:38 by oel-houm         ###   ########.fr       */
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

// static void    sendMsg(int fd, std::string msg)
// {
//     const char *_msg = msg.c_str();
//     size_t msgSize = strlen(_msg);
//     if (send(fd, _msg, msgSize, 0) < 0) {
//         std::cout << "send failed" << std::endl;
//     }
// }

void    Server::Who(std::string line, int i) {
    (void)i;
    int args = countArguments(line);
    if (args > 1) {
        std::stringstream iss(line);
        std::string cmd, channel;

        // loop on arguments
        int j = 0;
        while (iss && j <= args)
        {
            if (j == 1)
                iss >> cmd;     // Command
            else if (j == 2)
                iss >> channel; // Channel
            j++;
        }
        for (size_t i = 0; i < _channels.size(); ++i) {
            if (channel == _channels[i]->_name)
            {
                std::vector<Client *> members = _channels[i]->_members;
                for (size_t j = 0; j < members.size(); ++j) {
                    // Do something with each member
                    std::cout << "Channel: " << _channels[i]->_name << ", Member: " << members[j]->_username << std::endl;
                }
            }
        }
    }
}
