/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:22:36 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/17 11:22:39 by oel-houm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Include/Server.hpp"
#include "./Include/Client.hpp"
#include "./Include/Channel.hpp"

Server::Server(std::string port, std::string password) {
    this->_port = atoi(port.c_str());
    if (this->_port == 0 || (this->_port < 0 && this->_port > 65535))
        throw std::runtime_error("Invalid port");
    this->_password = password;
    if (!(this->_password.length() >= 8 && this->_password.length() <= 16))
        throw std::runtime_error("Invalid password");
}

 int     main(int ac, char **av)
{
    if (ac != 3) {
        std::cerr << "Invalid number of arguments." << std::endl;
        std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return 1;
    }
    try {
        Server ircServer(av[1], av[2]);
        ircServer.start();
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}
