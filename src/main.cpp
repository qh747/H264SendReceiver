#include <iostream>
#include "Common/BitStream.h"
#include "AnnexB/FileReader.h"

using Common::BitStream;
using AnnexB::Nalu;
using AnnexB::FileReader;

// 测试H264文件读取函数
void FuncTestFileReader() {
    FileReader reader("/home/quhan/04_myCode/H264SendReceiver/source/demo_video_176x144_baseline.h264");
    if (!reader.open()) {
        std::cerr << "Open file error." << std::endl;
        return;
    }

    Nalu nalu;
    while (reader.readNalu(nalu)) {
        std::cout << nalu.dumpString();
        nalu.clear();
    }

    reader.close();
}

// 测试BitStream类
void FuncTestBitStream() {
    // 测试数据
    uint8_t buffer[] = {0x01, 0x02, 0x03, 0x06};
    std::size_t size = sizeof(buffer) / sizeof(uint8_t);

    // 测试数据正常读取
    BitStream bs(buffer, size);
    std::cout << "readOneByte: " << bs.readNBits(8) << std::endl;
    std::cout << "readTwoBytes: " << bs.readNBits(16) << std::endl;

    // 测试数据越界读取
    while (!bs.isEnd()) {
        std::cout << "readFiveBits: " << bs.readNBits(5) << std::endl;
    }
}

int main()
{
    // 测试H264文件读取函数
    // FuncTestFileReader();

    // 测试BitStream类
    FuncTestBitStream();

    return 0;
}