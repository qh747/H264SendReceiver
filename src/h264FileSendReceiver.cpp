#include <iostream>
#include <jrtplib3/rtpconfig.h>
#include <jrtplib3/rtppacket.h>
#include <jrtplib3/rtpipv4address.h>
#include "common.h"
#include "h264FileSendReceiver.h"
using namespace jrtplib;
using namespace COMMON;

namespace SENDER_RECEIVER {

bool H264FileSendReceiver::initialize(const std::string& ipAddr, uint16_t port, const std::string& remoteIp, uint16_t remotePort)
{
    // 校验输入参数
    if (ipAddr.empty() || 0 == port || remoteIp.empty() || 0 == remotePort || port == remotePort) {
        std::cerr << "Initialize error. invalid input param." << std::endl;
        return false;
    }

    uint32_t localIpAddr = 0;
    uint32_t remoteIpAddr = 0;
    if (!IpAddrStrToUint32(ipAddr, localIpAddr) || !IpAddrStrToUint32(remoteIp, remoteIpAddr)) {
        std::cerr << "Initialize error. invalid ip addr." << std::endl;
        return false;
    }

    // 是否重复初始化
    if(nullptr != m_sessionPtr) {
        std::cout << "Inialize warning. session has been initialized." << std::endl;
        return true;
    }

    // 初始化RTP会话参数
    m_sessionParamPtr = std::make_shared<RTPSessionParams>();
    {
        // 设置使用预先定义的ssrc（非指定ssrc）
        m_sessionParamPtr->SetUsePredefinedSSRC(true);

        // 设置采样间隔（H264固定采样率为90000，采样间隔 = 1秒 / 采样率）
        m_sessionParamPtr->SetOwnTimestampUnit(1.0 / H264_SAMPLE_RATE);

        // 设置支持接收自身发送的数据
        m_sessionParamPtr->SetAcceptOwnPackets(true);
    }
    
    // 初始化RTP传输参数
    m_transmissionParamPtr = std::make_shared<RTPUDPv4TransmissionParams>();
    {
        // 设置本端IP地址
        m_transmissionParamPtr->SetBindIP(localIpAddr);

        // 设置本端端口
        m_transmissionParamPtr->SetPortbase(port);
    }

    // 初始化RTP会话
    m_sessionPtr = std::make_shared<RTPSession>();
    {
        // 设置会话参数和传输参数
        if (!Checkerror(m_sessionPtr->Create(*m_sessionParamPtr, m_transmissionParamPtr.get()))) {
            std::cerr << "Initialize error. create session failed." << std::endl;
            return false;
        }

        // 设置对端地址
        RTPIPv4Address remoteAddr(remoteIpAddr, remotePort);
        if(!Checkerror(m_sessionPtr->AddDestination(remoteAddr))) {
            std::cerr << "Initialize error. add destination failed." << std::endl;
            return false;
        }

        // 设置时间戳增加间隔（时间戳增加间隔 = （1秒 / 帧率）* 采样率）
        double frameInterval = 1.0 / FRAME_RATE;
        m_sessionPtr->SetDefaultTimestampIncrement((uint32_t)(frameInterval * H264_SAMPLE_RATE));

        // 设置载荷类型
        m_sessionPtr->SetDefaultPayloadType(H264_PAYLOAD_TYPE);

        // 设置标志位
        m_sessionPtr->SetDefaultMark(true);
    }
    
    return true;
}

bool H264FileSendReceiver::sendFile(const std::string& filePath, const std::string& fileName)
{
    // 校验输入参数
    if  (filePath.empty() || fileName.empty()) {
        std::cerr << "Send file error. invalid input param." << std::endl;
        return false;
    }

    // 校验session状态
    if (nullptr == m_sessionPtr) {
        std::cerr << "Send file error. session not initialized." << std::endl;
        return false;
    }

    // 打开文件
    std::string filePathName = filePath + "/" + fileName;
    std::cout << "Send file name: " << filePathName << std::endl;

    auto fd = fopen(filePathName.c_str(), "rb");
    if (nullptr == fd) {
        std::cerr << "Send file error. open file failed. file name: " << filePathName << std::endl;
        return false;
    }

    // 读取H264起始码
    uint8_t buffer[FILE_BUFFER_SIZE] = {0};
    fread(buffer, 1, 4, fd);

    uint32_t pos = 0;
    NaluStartCode_t naluType = NaluStartCode_t::NALU_START_CODE_UNKNOWN;
    if (FindFirstStartCode(buffer)) {
        pos = 4;
        naluType = NaluStartCode_t::NALU_START_CODE_FIRST;
    }
    else if (FindIntervalStartCode(buffer)) {
        pos = 3;
        naluType = NaluStartCode_t::NALU_START_CODE_INTERVAL;
    }
    else {
        std::cerr << "Send file error. file start code not exists. file name: " << filePathName << std::endl;
        return false;
    }
    
    // 读取文件剩余内容
    bool endFlag = false;
    while (!endFlag) {
        if(feof(fd)) {	
            NaluToRtp(m_sessionPtr, buffer + (uint8_t)naluType, pos - (uint32_t)naluType - 1);
            endFlag = true;
        }

        buffer[pos++] = fgetc(fd);

        if (FindFirstStartCode(&buffer[pos-4])) {
            NaluToRtp(m_sessionPtr, buffer + (uint8_t)naluType, pos - (uint32_t)naluType - 4);

            memcpy(buffer, START_CODE_FIRST, 4);
            pos = 4;
            naluType = NaluStartCode_t::NALU_START_CODE_FIRST;

            RTPTime::Wait(0.03);			
        }
        else if (FindIntervalStartCode(&buffer[pos-3])) {
            NaluToRtp(m_sessionPtr, buffer + (uint8_t)naluType, pos - (uint32_t)naluType - 3);

            memcpy(buffer, START_CODE_INTERVAL, 3);
            pos = 3;
            naluType = NaluStartCode_t::NALU_START_CODE_INTERVAL;

            RTPTime::Wait(0.03);
        }
    }

    std::cout << "Send file success. file name: " << fileName << std::endl;
    fclose(fd);
    return true;
}

bool H264FileSendReceiver::recvFile(const std::string& filePath, const std::string& fileName)
{
    // 校验输入参数
    if  (filePath.empty() || fileName.empty()) {
        std::cerr << "Recv file error. invalid input param." << std::endl;
        return false;
    }

    // 校验session状态
    if (nullptr == m_sessionPtr) {
        std::cerr << "Recv file error. session not initialized." << std::endl;
        return false;
    }

    // 打开文件
    std::string filePathName = filePath + "/" + fileName;
    std::cout << "Recv file name: " << filePathName << std::endl;

    auto fd = fopen(filePathName.c_str(), "rb");
    if (nullptr == fd) {
        std::cerr << "Recv file error. open file failed. file name: " << filePathName << std::endl;
        return false;
    }

    // 等待接收数据
    m_sessionPtr->BeginDataAccess();

    RTPTime delay(1);
	RTPTime startTime = RTPTime::CurrentTime();

    bool endFlag = false;
    while (!endFlag) {

#ifndef RTP_SUPPORT_THREAD
        // 接收网络数据错误(未使用jthread时使用)
        if (!Checkerror(m_sessionPtr->Poll())) {
            continue;
        }
#endif

        // 接收网络数据处理
        if (m_sessionPtr->GotoFirstSourceWithData()) {
            uint32_t pos = 0;
            uint8_t buffer[FILE_BUFFER_SIZE] = {0};
            do {
                auto pktPtr = m_sessionPtr->GetNextPacket();
                if (nullptr == pktPtr) {
                    std::cout << "Recv file warning. packet is null." << std::endl;
                    continue;
                }

                auto payloadType = pktPtr->GetPayloadType();
                if (H264_PAYLOAD_TYPE != pktPtr->GetPayloadType()) {
                    std::cout << "Recv file warning. payload type is not H264. payload type: " << (uint32_t)payloadType << std::endl;
                    m_sessionPtr->DeletePacket(pktPtr);
                    continue;
                }

                auto payloadLen = pktPtr->GetPayloadLength();
                auto payloadPtr = pktPtr->GetPayloadData();
                
                if (pktPtr->HasMarker()) {
                    memcpy(&buffer[pos], payloadPtr, payloadLen);	
					fwrite(buffer, 1, pos + payloadLen, fd);
					pos = 0;
                }
                else {
                    memcpy(&buffer[pos], payloadPtr, payloadLen);	
                    pos += payloadLen;
                }

                m_sessionPtr->DeletePacket(pktPtr);
            } while (m_sessionPtr->GotoNextSourceWithData());
        }

        // 判断是否退出
        RTPTime::Wait(delay);
		RTPTime curTime = RTPTime::CurrentTime();
		curTime -= startTime;

		if (curTime > RTPTime(40.0)) {
            endFlag = true;
        }
    }

    m_sessionPtr->EndDataAccess();
	m_sessionPtr->BYEDestroy(RTPTime(10.0), 0, 0);

    std::cout << "Recv file success. file name: " << fileName << std::endl;
    fclose(fd);
    return true;
} 

} // namespace SENDER_RECEIVER