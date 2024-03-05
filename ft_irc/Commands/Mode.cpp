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

// static void    sendMsg(int fd, std::string msg)
// {
//     const char *_msg = msg.c_str();
//     size_t msgSize = strlen(_msg);
//     if (send(fd, _msg, msgSize, 0) < 0) {
//         std::cout << "send failed" << std::endl;
//     }
// }

void    Server::Mode(std::string line, int i) {
    (void)i;
    (void)line;
    int args = countArguments(line);
    if (args > 1 && args < 4) {
        std::stringstream iss(line);
        std::string cmd, arg1, mode;

        // loop on arguments
        int j = 0;
        while (iss && j <= args)
        {
            if (j == 1)
                iss >> cmd;     // Command
            else if (j == 2)
                iss >> arg1;    // Channel, or Nickname
            else if (j == 3)
                iss >> mode;    // Mode
            j++;
        }

        // if channel
        if (arg1[0] == '#') {
            //
        }

        // if nickname
        else {
            //
        }
    }
}
