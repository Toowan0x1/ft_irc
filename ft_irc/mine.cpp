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
#include <sys/poll.h>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <cstring>

#define SIZE 512

class   Client {
    private:
        int     _socket_fd;
        struct pollfd fd;
        std::string IPAddr;
        struct socketaddr_in _addr;
        std::string _username;
        std::string _nickname;
        std::string  _buffer;
        bool _authenticate;
        bool _welcomed;
        bool _registred;
    public:
        Client();
};

class   Channel {
    private:
        std::string _name;
    public:
        Channel();
};

class Server {
    private:
        std::string          _hostname;
        int                  _socketfd;
        int                     _port;
        struct soketaddr_in     _addr;
        struct pollfd           _fds[SIZE];
        std::string             _password;
        std::vector<Client *>   _all_client; // clientList
        std::vector<Channel *>  _channels;
    public:
        Server(std::string port, std::string password);
        void    start();
        std::string    getHostname();
        //Server();
};

Server::Server(std::string port, std::string password) {
    this->_port = atoi(port.c_str());
    if (this->_port == 0 || (this->_port < 0 && this->_port > 65535))
        throw std::runtime_error("Invalid port");
    this->_password = password;
    if (this->_password.empty()) // pass policy from 6 to 14 character
    {
        throw std::runtime_error("Invalid password");
    }
}

std::string    Server::getHostname() {
}

void    Server::start() {
    /*
    1. Set socket
    2. Set addr
    3. Binding
    4. Listening
    */
    int value = 1;
    this->_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socketfd < 0) // == -1
        throw std::runtime_error("Failed to create server socket");
    if (setsockopt(this->_socketfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) < 0)
        throw std::runtime_error("Failed to set socket reuse option");
    if (fcntl(this->_socketfd, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Failed to set socket to non-blocking mode");
    
    //
    this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = htons(this_port);
    this->_hostname = this->getHostname();
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
