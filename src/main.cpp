#include <iostream>
#include "AnnexB/FileReader.h"
using AnnexB::Nalu;
using AnnexB::FileReader;

int main()
{
    FileReader reader("/home/quhan/04_myCode/H264SendReceiver/source/demo_video_176x144_baseline.h264");
    if (!reader.open()) {
        std::cerr << "Open file error." << std::endl;
        return -1;
    }

    Nalu nalu;
    while (reader.readNalu(nalu)) {
        std::cout << nalu.dumpString();
        nalu.clear();
    }

    reader.close();
    return 0;
}