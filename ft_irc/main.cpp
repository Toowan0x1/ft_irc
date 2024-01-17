/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 13:00:04 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/15 09:44:38 by oel-houm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>	
#include <string.h>	
#include <unistd.h>	
#include <stdlib.h>	
#include <sys/types.h>	// For various data types used in system calls
#include <sys/socket.h>	// For socket-related functions
#include <netinet/in.h>	// For Internet address structures
#include <arpa/inet.h>	// For functions to manipulate IP addresses
#include <netdb.h>	// For functions to query DNS (e.g., gethostbyname)
#include <fcntl.h>	// For file control (e.g., fcntl)
#include <errno.h>	// For error handling (errno variable)

class Server {
	private:
		std::string		_hostname;
		int			_socketfd;
		int			_port;
		struct sockaddr_in	_addr;
		_fds[];
		std::string		_password;
		_list;
		_channels;
	public:
		Server(std::string port, std::string password);
		~Server();
}

Server::Server(std::string port, std::string password) {
	//
}

using namespace std;

int	main(int ac, char **av)
{
	if (ac != 3) {
		cout << "Invalid number of arguments!" << endl;
		// usage:
		return 1;
	}
	try {
		Server server(av[1], av[2]);
		(void)av;
	} catch (const runtime_error& e) {cout << e.what() << endl;}
	return 0;
}
