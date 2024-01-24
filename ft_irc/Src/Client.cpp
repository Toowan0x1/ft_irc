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

// Client::Client() : _registred(false), _welcomed(false), _authenticated(false) { // _authorized
//     /**/
// }

// Client::Client(int &clientFD) :
//     //_clientFd(clientFD),
//     _addr(),
//     _username(""),
//     _nickname(""), // default nickname: "guest_"+${id}
//     _realName(""),
//     _password(""),
//     _buffer(""),
//     _authenticated(false),
//     _welcomed(false),
//     _registred(false),
//     _keepAlive(true) {
//         (void)clientFD;
//     // Additional constructor code if needed
//     // (void)clientFD; // Not needed as you are already using it in the initializer list
// }

Client::Client(int &clientFD) : _keepAlive(1) {
    this->_clientFd = clientFD;
}

Client::~Client() {
    /*
    - handle read operation
    */
}

// host port password nickname
// your host is oelhoum.1337,
// this server ha been started Wed Oct 12 2022
// There are 1 users and 1 bot on 1 servers
// end of MOTD command
// nc localhost 1337
// PASS IRC-1337
// NICK TOOWAN
// 464: Password Incorrect

/*
MAX_BUFFER_SIZE
buffer[MAX_BUFFER_SIZE]

/exit
/quit

solve this: client connected and(registred with a false password )
[*] Client try to connect to server, From 127.0.0.1
[+] New Client Connected, Client IP Address 127.0.0.1
*/
