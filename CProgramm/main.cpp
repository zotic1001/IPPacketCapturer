#include <iostream>

struct IPData {
    int receivedPackets;
    int receivedBytes;
    int transmittedPackets;
    int transmittedBytes;

    IPData() : receivedPackets(0), receivedBytes(0), transmittedPackets(0), transmittedBytes(0) {}
};

int main() {
    return 0;
}