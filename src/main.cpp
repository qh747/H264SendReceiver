#include <iomanip>
#include <iostream>
#include "Common/BitStream.h"
#include "AnnexB/Ebsp.h"
#include "AnnexB/FileReader.h"
#include "AnnexB/NaluStream.h"
using Common::BitStream;
using AnnexB::Nalu;
using AnnexB::Ebsp;
using AnnexB::FileReader;
using AnnexB::NaluStream;

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

// 测试EBSP读取函数
void FuncTestEbsp() {
    FileReader reader("/home/quhan/04_myCode/H264SendReceiver/source/demo_video_176x144_baseline.h264");
    if (!reader.open()) {
        std::cerr << "Open file error." << std::endl;
        return;
    }

    Nalu nalu;
    while (reader.readNalu(nalu)) {
        std::cout << nalu.dumpString();

        Ebsp ebsp;
        if (nalu.getEbsp(ebsp)) {
            std::cout << ebsp.dumpString();
        }

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
    std::cout << "readOneByte: " << std::hex << std::showbase << bs.readNBits(8) << std::endl;
    std::cout << "readTwoBytes: " << std::hex << std::showbase << bs.readNBits(16) << std::endl;

    // 测试数据越界读取
    while (!bs.isEnd()) {
        std::cout << "readFiveBits: " << std::hex << std::showbase << bs.readNBits(5) << std::endl;
    }
}

// 测试NaluStream类
void FuncTestNaluStream() {
    {
        // 测试数据，0x67的二进制为：0110 0111 -> 011 00111
        uint8_t buffer[] = {0x67};
        std::size_t size = sizeof(buffer) / sizeof(uint8_t);

        // 测试读取无符号哥伦布熵编码
        NaluStream ns(buffer, size);
        std::cout << "readUe(v): " << ns.readUev() << std::endl;
        std::cout << "readUe(v): " << ns.readUev() << std::endl;
    }

    {
        // 测试数据，0x67的二进制为：0110 0111 -> 011 00111
        uint8_t buffer[] = {0x67};
        std::size_t size = sizeof(buffer) / sizeof(uint8_t);

        // 测试读取有符号哥伦布熵编码
        NaluStream ns(buffer, size);
        std::cout << "readSe(v): "  << ns.readSev() << std::endl;
        std::cout << "readSe(v): "  << ns.readSev() << std::endl;
    }
}

int main()
{
    // 测试H264文件读取函数
    // FuncTestFileReader();

    // 测试BitStream类
    // FuncTestBitStream();

    // 测试NaluStream类
    // FuncTestNaluStream();

    // 测试EBSP读取函数
    FuncTestEbsp();

    return 0;
}