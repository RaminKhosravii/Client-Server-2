#include <iostream>
#include <unordered_map>
#include "DeviceServer.hpp"

// Constructor to initialize the server and its parameters
DeviceServer::DeviceServer() {

}

// Destructor to clean up resources
DeviceServer::~DeviceServer() {

}

// Function to handle incoming JSON requests and generate a JSON response
nlohmann::json DeviceServer::handleRequest(const nlohmann::json& request) {
    nlohmann::json response;
    const std::string requestType = request.value("request_type", "");

    if (requestType == "connect") {
        response["result"] = "success";
        response["message"] = "Connected to the server.";
    }
    else if (requestType == "add_device") {
        const std::string deviceName = request.value("name", "");

        if (const std::string deviceStatus = request.value("status", ""); !deviceName.empty() && !deviceStatus.empty()) {
            deviceRegistry[deviceName] = Device(deviceName, deviceStatus);
            response["result"] = "success";
            response["message"] = "Device added.";
        } else {
            response["result"] = "error";
            response["message"] = "Device name or status cannot be empty.";
        }
    }
    else if (requestType == "control_device") {
        const std::string deviceName = request.value("name", "");
        const std::string newStatus = request.value("new_status", "");

        if (const auto it = deviceRegistry.find(deviceName); it != deviceRegistry.end()) {
            it->second.status = newStatus; // Update the device status
            response["result"] = "success";
            response["message"] = "Device status updated.";
        } else {
            response["result"] = "error";
            response["message"] = "Device not found.";
        }
    }
    else if (requestType == "get_devices") {
        nlohmann::json devicesJson = nlohmann::json::array();

        for (const auto& [name, device] : deviceRegistry) {
            nlohmann::json deviceJson;
            deviceJson["name"] = device.id;
            deviceJson["status"] = device.status;
            devicesJson.push_back(deviceJson);
        }

        response["result"] = "success";
        response["devices"] = devicesJson;
    }
    else {
        response["result"] = "error";
        response["message"] = "Invalid request.";
    }

    return response;
}

// Function to initialize and start the server
auto DeviceServer::startServer() -> void {

    std::cout << "Server started and waiting for incoming connections...\n";
}