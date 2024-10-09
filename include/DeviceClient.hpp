#ifndef DEVICE_CLIENT_HPP
#define DEVICE_CLIENT_HPP

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// #define PORT 8080
// #define SERVER_IP "127.0.0.1"

class DeviceClient {
private:
    mutable SOCKET socket_;
    mutable bool isConnected_;
    std::vector<std::string> actionLogs_;
    static constexpr int BUFFER_SIZE = 1024;
    std::array<char, BUFFER_SIZE> buffer{};

public:
    DeviceClient();
    ~DeviceClient();
    void displayMainMenu();

private:
    void connectToServer() const;

    void sendDeviceInformation() const;

    void sendDeviceInformation();

    void receiveDeviceInformation() const;

    void createReport() const;

    void addDevice();
    void controlDevice();
    void receiveDeviceInformation();
    void createReport();
    void shutdownConnection();
    void processRequest(const json& request, json& response);
    void logAction(const std::string& action);
};

#endif // DEVICE_CLIENT_HPP