cmake_minimum_required(VERSION 3.10)


project(Client-Server1)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include_directories(include)


include_directories(include/nlohmann)


add_executable(ClientExecutable src/main_client.cpp src/DeviceClient.cpp)
add_executable(ServerExecutable src/main_server.cpp src/DeviceServer.cpp)


target_link_libraries(ClientExecutable ws2_32)
# target_link_libraries(ClientExecutable PRIVATE your_additional_library)
# target_link_libraries(ServerExecutable PRIVATE your_additional_library)


# target_link_libraries(ClientExecutable PUBLIC YourLibrary)
# target_link_libraries(ServerExecutable PUBLIC YourLibrary)