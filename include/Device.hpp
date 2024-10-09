#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <string>
#include <utility>
#include <sstream>

class Device {
public:
    std::string id;
    std::string status;


    Device(std::string deviceId, std::string deviceStatus)
        : id(std::move(deviceId)), status(std::move(deviceStatus)) {}


    Device() : id(""), status("") {}

    // متد برای نمایش اطلاعات دستگاه
    std::string toString() const {
        std::ostringstream oss;
        oss << "Device ID: " << id << ", Status: " << status;
        return oss.str();
    }


    std::string getId() const {
        return id;
    }

    std::string getStatus() const {
        return status;
    }

    void setId(const std::string& deviceId) {
        id = deviceId;
    }

    void setStatus(const std::string& deviceStatus) {
        status = deviceStatus;
    }

};

#endif // DEVICE_HPP