#include <iostream>
#include <unistd.h>

int main() {
    char hostname[256]; // Assuming the hostname won't exceed 255 characters

    if (gethostname(hostname, sizeof(hostname)) == 0) {
        std::cout << "Hostname: " << hostname << std::endl;
    } else {
        std::cerr << "Error getting hostname" << std::endl;
        // Handle error appropriately
        return 1;
    }

    return 0;
}

