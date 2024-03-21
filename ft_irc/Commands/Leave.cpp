/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Leave.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 20:48:14 by oel-houm          #+#    #+#             */
/*   Updated: 2024/03/20 20:48:15 by oel-houm         ###   ########.fr       */
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

void    Server::Leave(std::string line, int i)
{
    int args = countArguments(line);
    (void)i;
    (void)sendMsg;
    if (args == 1) {
        /**/
    }
    // check joined 
    // leave channel
    // anounce message to all channel members
    // anounce message to user after leaving
}
