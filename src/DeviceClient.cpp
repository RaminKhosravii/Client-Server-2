#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <limits>
#include <string>
#include <sstream>
#include "DeviceClient.hpp"

constexpr const char* SERVER_IP = "127.0.0.1";
constexpr int PORT = 8080;

// Initializing Winsock and setting up the client
DeviceClient::DeviceClient() : socket_(INVALID_SOCKET), isConnected_(false) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("Failed to initialize Winsock.");
    }
}

// Cleaning up resources when the client is destroyed
DeviceClient::~DeviceClient() {
    shutdownConnection();
    WSACleanup();
}

// Displays the main menu for user interaction
void DeviceClient::displayMainMenu() {
    int choice;
    do {
        std::cout << "\nMain Menu:\n"
                  << "1. Connect\n2. Send Device Info\n3. Receive Device Info\n"
                  << "4. Generate Report\n5. Disconnect\n0. Exit\nChoose an option: ";
        std::cin >> choice;

        if (std::cin.fail() || choice < 0 || choice > 5) {
            std::cin.clear(); // clear input stream
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            std::cout << "Invalid choice. Please try again.\n";
            continue;
        }

        switch (choice) {
            case 1: connectToServer(); break;
            //case 2: sendDeviceInformation(); break;
            //case 3: receiveDeviceInformation(); break;
            //شcase 4: createReport(); break;
            case 5: shutdownConnection(); break;
            case 0: std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid choice. Please try again.\n"; break;
        }
    } while (choice != 0);
}

// Connects to the server
void DeviceClient::connectToServer() const {
    if (isConnected_) {
        std::cout << "Already connected to server.\n";
        return;
    }

    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == INVALID_SOCKET) {
        throw std::runtime_error("Socket creation failed: " + std::to_string(WSAGetLastError()));
    }

    sockaddr_in serverAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddress.sin_addr);

    // Attempt to connect to the server
    if (connect(socket_, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        closesocket(socket_);
        socket_ = INVALID_SOCKET; // Clear the socket variable
        int err = WSAGetLastError();
        throw std::runtime_error("Connection failed with error code: " + std::to_string(err));
    }

    isConnected_ = true;
    std::cout << "Successfully connected to server.\n";
}

// Example of sending device information using JSON
void DeviceClient::sendDeviceInformation() const {
    if (!isConnected_) {
        std::cout << "Not connected to server.\n";
        return;
    }

    nlohmann::json deviceInfo;
    deviceInfo["name"] = "Device1";
    deviceInfo["status"] = "online";

    // Convert json to string and send to server
    const std::string message = deviceInfo.dump();
    const int sentBytes = send(socket_, message.c_str(), message.length(), 0);

    if (sentBytes == SOCKET_ERROR) {
        const int err = WSAGetLastError();
        std::cerr << "Failed to send data: " << err << std::endl;
        closesocket(socket_);
        socket_ = INVALID_SOCKET; // Reset socket on error
        isConnected_ = false; // Disconnect on failure
    } else {
        std::cout << "Sent " << sentBytes << " bytes to server.\n";
    }
}

// Function to receive device information from the server
void DeviceClient::receiveDeviceInformation() const {
    if (!isConnected_) {
        std::cout << "Not connected to server.\n";
        return;
    }

    char buffer[1024]; // Buffer to store received data
    memset(buffer, 0, sizeof(buffer)); // Clear buffer

    int bytesReceived = recv(socket_, buffer, sizeof(buffer) - 1, 0); // Receive data
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        nlohmann::json receivedData = nlohmann::json::parse(buffer);

        std::cout << "Received device information:\n";
        std::cout << receivedData.dump(4) << std::endl; // Pretty-printing JSON with indentation
    } else if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Failed to receive data: " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "Connection closed by the server.\n";
    }
}

// Function to create a simple report of device status
void DeviceClient::createReport() const {
    if (!isConnected_) {
        std::cout << "Not connected to server. Cannot create report.\n";
        return;
    }

    // Here, you can generate any report you want
    std::ostringstream report;
    report << "Device Report\n"
           << "--------------------\n"
           << "Device Name: Device1\n"
           << "Status: Online\n" // You can expand this based on actual device status
           << "Connection Status: " << (isConnected_ ? "Connected" : "Disconnected") << "\n";

    // Output the report to the console or save it to a file
    std::cout << report.str() << std::endl;
}

// Function to shutdown the connection
void DeviceClient::shutdownConnection() {
    if (isConnected_) {
        closesocket(socket_);
        socket_ = INVALID_SOCKET;
        isConnected_ = false;
        std::cout << "Disconnected from server.\n";
    }
}