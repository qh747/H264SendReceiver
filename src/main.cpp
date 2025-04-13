#include <string>
#include <iostream>
#include "h264FileSendReceiver.h"
using namespace SENDER_RECEIVER;

int main(int argc, char* argv[]) 
{
    // 检查参数数量是否正确
    if (argc < 7) {
        std::cerr << "Usage: <mode> <local_ip> <local_port> <remote_ip> <remote_port> <file_path/file_name>" << std::endl;
        std::cerr << "Example (sender): ./program sender 127.0.0.1 8080 127.0.0.1 8081 path/file" << std::endl;
        std::cerr << "Example (receiver): ./program receiver 127.0.0.1 8080 127.0.0.1 8081 path/file" << std::endl;
        return EXIT_FAILURE;
    }

    // 解析输入参数
    std::string mode       = argv[1];
    std::string ipAddr     = argv[2];
    uint16_t    port       = std::stoi(argv[3]);
    std::string remoteIp   = argv[4];
    uint16_t    remotePort = std::stoi(argv[5]);
    std::string fileInfo   = argv[6];

    auto pos = fileInfo.find_last_of("/\\");
    if (std::string::npos == pos) {
        std::cerr << "Invalid file path: " << fileInfo << std::endl;
        return EXIT_FAILURE;
    }

    std::string filePath = fileInfo.substr(0, pos);
    std::string fileName = fileInfo.substr(pos + 1);

    // 初始化h264文件发送接收
    auto h264FileSendReceiver = std::make_shared<H264FileSendReceiver>();
    if (!h264FileSendReceiver->initialize(ipAddr, port, remoteIp, remotePort)) {
        std::cerr << "Failed to initialize H264FileSendReceiver" << std::endl;
        return EXIT_FAILURE;
    }

    // 启动h264文件发送接收
    if ("sender" == mode) {
        h264FileSendReceiver->sendFile(filePath, fileName);
    }
    else {
        h264FileSendReceiver->recvFile(filePath, fileName);
    }

    return 0;
}