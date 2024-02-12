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

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class   Client {
    public:
        int _clientFd;
        //struct pollfd       _fd;
        struct sockaddr_in  _addr;
        std::string _username;
        std::string _nickname;
        std::string _realName;
        std::string _userMode;
        std::string _IPAddress;
        std::string _hostname;
        std::string _password;
        std::string _buffer; // _msgBuffer  + leave_msg
        bool _authenticated;
        bool _joined;
        bool _keepAlive;

        std::string _leaveMsg;

        // Map to store channel modes for each channel the user is in
        std::map<std::string, std::string> _channelModes;

        Client(int &clientFD); // clientSocketFd
        ~Client();
};

#endif

/*
#include <iostream>

class Client {
public:
    // Your existing member variables...

    // Map to store channel modes for each channel the user is in
    std::map<std::string, std::string> _channelModes;

    // Constructor and destructor...

    // Function to set channel modes for a channel
    void setChannelMode(const std::string& channel, const std::string& mode) {
        _channelModes[channel] = mode;
    }

    // Function to get channel modes for a channel
    std::string getChannelMode(const std::string& channel) {
        if (_channelModes.find(channel) != _channelModes.end()) {
            return _channelModes[channel];
        }
        return "";
    }

    // Function to remove channel modes for a channel
    void removeChannelMode(const std::string& channel) {
        _channelModes.erase(channel);
    }
};

Client client;
client.setChannelMode("#channel1", "+o"); // Set operator mode for channel #channel1
std::cout << "Mode for #channel1: " << client.getChannelMode("#channel1") << std::endl; // Print mode for #channel1
client.removeChannelMode("#channel1"); // Remove mode for channel #channel1


*/