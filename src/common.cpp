#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <jrtplib3/rtperrors.h>
#include "common.h"
using namespace jrtplib;

namespace COMMON {

bool Checkerror(int rtperr) 
{
    if (rtperr < 0) {
        std::cerr << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
        return false;
    }

    return true;
}

bool FindFirstStartCode(uint8_t* buf) 
{
    // buf前四字节为0x00000001为起始码
    return (buf[0] == START_CODE_FIRST[0] && buf[1] == START_CODE_FIRST[1] && 
        buf[2] == START_CODE_FIRST[2] && buf[3] == START_CODE_FIRST[3]) ? true : false;
}

bool FindIntervalStartCode(uint8_t* buf)
{
    // buf前三字节为0x000001为起始码
    return (buf[0] == START_CODE_INTERVAL[0] || buf[1] == START_CODE_INTERVAL[1] || buf[2] == START_CODE_INTERVAL[2]) ? true : false;
}

void NaluPrintf(uint8_t* buf, uint32_t len, uint8_t type, uint32_t count) 
{
    std::cout << "NALU type: " << (uint32_t)type << " num: " << count << " len: " << len << std::endl;

    for(uint32_t i = 0; i < len; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)buf[i] << " ";
        if(i % 32 == 31)
            std::cout << std::endl;
    }
    std::cout << std::endl;
}

void PacketPrint(uint8_t* buf, uint32_t len, uint8_t type, uint32_t count)
{
    std::cout << "RTP PRINT: ----------" << std::endl;
    RtpPrintf(buf, len);
    std::cout << std::endl;

    std::cout << "NALU PRINT: ----------" << std::endl;
    NaluPrintf(buf, len, type, count);
    std::cout << std::endl;
}

void RtpPrintf(uint8_t* buf, uint32_t len) 
{
    std::cout << "RTP len: " << len << std::endl;

    for(uint32_t i = 0; i < len; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)buf[i] << " ";

        if(i % 32 == 31) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

void NaluToRtp(std::shared_ptr<RTPSession> sessionPtr, uint8_t* buf, uint32_t len) 
{
    // 帧间隔 = 1秒 / 帧率
    double frameInterval = 1.0 / FRAME_RATE;
    // 时间戳增加间隔 = 帧间隔 * 90000
    uint32_t timestampIncrement = (uint32_t)(frameInterval * 90000);

    uint32_t rtplen = len-1;
    if (rtplen <= MAXLEN) {
        PacketPrint(buf, len, H264_PAYLOAD_TYPE, 1);
        Checkerror(sessionPtr->SendPacket(buf, len, 96, true, timestampIncrement)); 		
    }
    else {
        // 数据包数量
        uint32_t pktCount = rtplen / MAXLEN;

        // 最后一个数据包长度
        uint32_t lastPktLength = rtplen % MAXLEN;
        if(lastPktLength == 0) {
            pktCount -= 1;
            lastPktLength = MAXLEN;
        }

        // 数据首字节为nalu首部
        uint8_t naluHeader = buf[0];

        /**
         * @brief: 取bit位: 6、5
         * 
         * bit:  7 6 5 4 3 2 1 0  &
         * 0x60: 0 1 1 0 0 0 0 0
         */
        uint8_t fuIndicator = naluHeader & 0x60;   
        // 置bit为1: 4 3 2     28    FU-A

        /**
         * @brief: 置bit为1: 4、3、2
         * 
         * bit:  7 6 5 4 3 2 1 0  |
         * 0x1C: 0 0 0 1 1 1 0 0
         */
        fuIndicator |= 0x1C;
        
        /**
         * @brief: 取bit位: 4、3、2、1、0
         * 
         * bit:  7 6 5 4 3 2 1 0  &
         * 0x60: 0 0 0 1 1 1 1 1 
         */
        uint8_t fuHeader = naluHeader & 0x1F;      
		
        uint32_t pktIndex = 0;
        uint8_t rtpbuf[2000] = {0};

        while(pktIndex++ <= pktCount) {
            if(pktIndex == 0) {	            // 第一包
                fuHeader |= 0x80;   // bit 7  S
                fuHeader &= ~0x40;  // bit 6  E 
                fuHeader &= ~0x20;  // bit 5  R

                rtpbuf[0] = fuIndicator;
                rtpbuf[1] = fuHeader;
                memcpy(&rtpbuf[2], &buf[1], MAXLEN);				
                
                PacketPrint(buf, MAXLEN + 2, H264_PAYLOAD_TYPE, pktIndex);
                Checkerror(sessionPtr->SendPacket(rtpbuf, MAXLEN+2, 96, true, 0)); 
            }
            else if(pktIndex == pktCount) { // 最后一包
                fuHeader &= ~0x80;    // bit 7  S
                fuHeader |= 0x40;     // bit 6  E 
                fuHeader &= ~0x20;    // bit 5  R

                rtpbuf[0] = fuIndicator;
                rtpbuf[1] = fuHeader;
                memcpy(&rtpbuf[2], &buf[1 + pktIndex * MAXLEN], lastPktLength);

                PacketPrint(buf, lastPktLength + 2, H264_PAYLOAD_TYPE, pktIndex);
                Checkerror(sessionPtr->SendPacket(rtpbuf, lastPktLength+2, 96, true, timestampIncrement)); 	
            }
            else {                    // 中间包
                fuHeader &= ~0x80;    // bit 7  S
                fuHeader &= ~0x40;    // bit 6  E 
                fuHeader &= ~0x20;    // bit 5  R

                rtpbuf[0] = fuIndicator;
                rtpbuf[1] = fuHeader;
                memcpy(&rtpbuf[2], &buf[1 + pktIndex * MAXLEN], MAXLEN);				
                
                PacketPrint(buf, MAXLEN + 2, H264_PAYLOAD_TYPE, pktIndex);
                Checkerror(sessionPtr->SendPacket(rtpbuf, MAXLEN+2, 96, true, 0));
            }
        }		
    }
}

bool IpAddrStrToUint32(const std::string& ipStr, uint32_t& ip)
{
    std::vector<std::string> parts;
    std::stringstream ss(ipStr);
    std::string part;
    
    // 分割字符串
    while (std::getline(ss, part, '.')) {
        parts.push_back(part);
    }
    
    if (parts.size() != 4) {
        std::cerr << "Ip addr convert error. invalid IP address format. ip: " << ipStr << std::endl;
        return false;
    }
    
    for (int i = 0; i < 4; ++i) {
        int num = std::stoi(parts[i]);
        if (num < 0 || num > 255) {
            std::cerr << "Ip addr convert error. ip octet out of range. ip: " << ipStr << std::endl;
        }
        ip |= (num << (24 - 8 * i));
    }
    
    return true;
}

} // namespace COMMON