/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:19:43 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/17 11:21:44 by oel-houm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

class   Client {
    private:
        // std::string command
        int     _socket_fd; // clientfd clientsocket
        struct pollfd fd;
        struct socketaddr_in _addr;
        std::string _username;
        std::string _nickname;
        std::string _realName;
        std::string _IPAddress;
        std::string _password;
        std::string  _buffer; // _msgBuffer  + leave_msg
        bool _authenticate; //authorized // authorized means client entered correct password (only)
        bool _welcomed; // welcomed means this used was registered before and welcomed, used so that we dont send welcome messages everytime user changes nick
        bool _registred; // registered means client entered correct password, a valid nickname, username and realname 
        bool _keepAlive;
    public:
        Client();
};