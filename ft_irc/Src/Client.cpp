/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:27:13 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/18 13:27:14 by oel-houm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"
#include "../Include/Client.hpp"
#include "../Include/Channel.hpp"

//     _nickname(""), // default nickname: "guest_"+${id}
Client::Client(int &clientFD) :
    _clientFd(clientFD),
    _userMode("0"),
    _authenticated(0),
    _welcomed(0),
    _registred(0),
    _keepAlive(1)
{
    /**/
}

Client::~Client() {
    /*
    - handle read operation
    */
}

// host port password nickname
// your host is oelhoum.1337,
// Bot: There are 1 users and 1 bot on 1 servers
// end of MOTD command

/*
/exit
/quit

solve this: client connected and(registred with a false password )
[*] Client try to connect to server, From 127.0.0.1
[+] New Client Connected, Client IP Address 127.0.0.1
*/
