#include <iostream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>

class YourIRCServer {
private:
    int _socketfd;  // Assuming _socketfd is an integer member of YourIRCServer

public:
    // ... other members and methods ...

    void startListening() {
        // Assuming _socketfd has been properly initialized before calling startListening

        // Start listening on the socket
        if (listen(this->_socketfd, 0) < 0) {
            //std::cerr << "Error on listen: " << strerror(errno) << std::endl;

            // Optionally throw an exception with the error message
            throw std::runtime_error("Error on listen: " + std::string(strerror(errno)));
	    exit(1);
        }

        // Rest of your code if listen is successful
    }

    // ... other members and methods ...
};

int main() {
    YourIRCServer server;

    // Assuming you initialize _socketfd before calling startListening
    server.startListening();

    // Rest of your main code

    return 0;
}

