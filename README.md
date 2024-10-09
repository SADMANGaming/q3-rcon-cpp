# cod-rcon-cpp
Header to send rcon commands to Quake 3 based games.

# How to use

- Simple
```cpp
#include "sadman/rcon.h"

int main() {
    rcon.connect("123.123.1.12", 28961, "rcon password")) {
    rcon.send("status");
    return 0;
}
```

- Sends response
```cpp
#include "sadman/rcon.h"

int main() {
    if (rcon.connect("123.123.1.12", 28961, "rcon password")) {
        std::cout << "Connected successfully." << std::endl;
        std::string response = rcon.send("status");
        std::cout << "Server response: " << response << std::endl;
    } else {
        std::cerr << "Failed to connect." << std::endl;
    }

    return 0;
}
```

# Credits
- Sadman
- ChatGPT (creating sockets help)
