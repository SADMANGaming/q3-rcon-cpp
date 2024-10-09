# cod-rcon-cpp
Header to send rcon commands to Quake 3 based games.

# How to use

```cpp
#include "sadman/rcon.h"

int main() {
    rcon.connect("123.123.1.12", 28961, "rcon password")) {
    rcon.send("status");
    return 0;
}
```

# Credits
- Sadman
- ChatGPT (creating sockets help)
