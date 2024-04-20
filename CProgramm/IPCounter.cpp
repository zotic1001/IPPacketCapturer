#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

struct IPData {
    int receivedPackets;
    int receivedBytes;
    int transmittedPackets;
    int transmittedBytes;
};

class IPStreamCounter {
private:
    std::unordered_map<std::string, IPData> ipData;
    std::unordered_map<std::string, IPData> processData(const std::string& input_filename) {
        std::unordered_map<std::string, IPData> ipDataMap;
        std::ifstream file(input_filename);
        if (!file.is_open()) {
            throw std::runtime_error("Couldn't open input file");
        }
        std::string line;
        std::getline(file, line); //Skip Header of file
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string ipAddress, receivedPacketsStr, receivedBytesStr, transmittedPacketsStr, transmittedBytesStr;

            if (std::getline(iss, ipAddress, ',') &&
                std::getline(iss, receivedPacketsStr, ',') &&
                std::getline(iss, receivedBytesStr, ',') &&
                std::getline(iss, transmittedPacketsStr, ',') &&
                std::getline(iss, transmittedBytesStr)) {
                try {
                    int receivedPackets = std::stoi(receivedPacketsStr);
                    int receivedBytes = std::stoi(receivedBytesStr);
                    int transmittedPackets = std::stoi(transmittedPacketsStr);
                    int transmittedBytes = std::stoi(transmittedBytesStr);

                    if (ipDataMap.find(ipAddress) == ipDataMap.end()) {
                        IPData newData;
                        newData.receivedPackets = receivedPackets;
                        newData.receivedBytes = receivedBytes;
                        newData.transmittedPackets = transmittedPackets;
                        newData.transmittedBytes = transmittedBytes;
                        ipDataMap[ipAddress] = newData;
                    }
                    else {
                        IPData& existingData = ipDataMap[ipAddress];
                        existingData.receivedPackets += receivedPackets;
                        existingData.receivedBytes += receivedBytes;
                        existingData.transmittedPackets += transmittedPackets;
                        existingData.transmittedBytes += transmittedBytes;
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Error parsing line: " << line << std::endl;
                }
            }
            else {
                std::cout << "Invalid line format: " << line << std::endl;
            }
        }

        file.close();
        return ipDataMap;
    }
public:
    IPStreamCounter(const std::string& input_filename) {
        this->ipData = processData(input_filename);
    }
    void SetInputFileName(const std::string& input_filename) {
        this->ipData = processData(input_filename);
    }
    void SaveDataToCSV(const std::string& output_filename) {
        std::ofstream file(output_filename);

        if (!file.is_open()) {
            std::cout << "Failed to open file: " << output_filename << std::endl;
            return;
        }

        file << "IP address,received packets,received bytes,transmitted packets,transmitted bytes" << std::endl;
        for (const auto& pair : ipData) {
            const std::string& ipAddress = pair.first;
            const IPData& ipData = pair.second;
            file << ipAddress << ","
                << ipData.receivedPackets << ","
                << ipData.receivedBytes << ","
                << ipData.transmittedPackets << ","
                << ipData.transmittedBytes << std::endl;
        }
        file.close();
        std::cout << "Data written to file: " << output_filename << std::endl;
    }
};





int main() {
    std::string inputFilename = "input.csv";
    std::string outputFilename = "output.csv";
    std::cout << "Path to input file (.csv) (can be entered with or without a file extension): ";
    std::getline(std::cin, inputFilename);
    if (inputFilename.length() >= 4 && inputFilename.substr(inputFilename.length() - 4) != ".csv" || inputFilename.length() <= 4) {
        inputFilename += ".csv";
    }
    std::cout << "Path to output file (.csv) (can be entered with or without a file extension)  : ";
    std::getline(std::cin, outputFilename);
    if (outputFilename.length() >= 4 && outputFilename.substr(outputFilename.length() - 4) != ".csv" || outputFilename.length() <= 4) {
        outputFilename += ".csv";
    }
    IPStreamCounter* counter = nullptr;
    try { 
        IPStreamCounter* counter = new IPStreamCounter(inputFilename);
        counter->SaveDataToCSV(outputFilename);
    }
    catch (std::runtime_error& ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }
    delete counter;
    return 0;
}