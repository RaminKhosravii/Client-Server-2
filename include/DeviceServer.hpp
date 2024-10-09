#ifndef DEVICE_SERVER_HPP
#define DEVICE_SERVER_HPP

#include <winsock2.h>
#include <ws2tcpip.h>
#include <map>
#include <nlohmann/json.hpp>
#include "Device.hpp"

using json = nlohmann::json;

class DeviceServer {
private:
    std::map<std::string, Device> deviceRegistry;

public:
    DeviceServer();
    ~DeviceServer();

    static void startServer();

private:
    json handleRequest(const json& request);
};

#endif // DEVICE_SERVER_HPP