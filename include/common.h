#pragma once
#include <memory>
#include <cstdbool>
#include <cstdint>
#include <jrtplib3/rtpdefines.h>
#include <jrtplib3/rtpsession.h>
using jrtplib::RTPSession;

namespace COMMON {

// 文件缓冲区大小
const uint32_t FILE_BUFFER_SIZE = 1024 * 1024;

// RTP包大小
const uint32_t MAXLEN = RTP_DEFAULTPACKETSIZE - 100;

// H264负载类型
const uint8_t H264_PAYLOAD_TYPE = 96;

// H264采样率
const uint32_t H264_SAMPLE_RATE = 90000;

// 帧率(FPS)
const uint32_t FRAME_RATE = 25;

// 起始码
const uint8_t START_CODE_FIRST[4] = {0, 0, 0, 1};
const uint8_t START_CODE_INTERVAL[3] = {0, 0, 1};

// NALU起始码类型
typedef enum class NALU_START_CODE_TYPE : uint8_t 
{
    NALU_START_CODE_UNKNOWN     = 0,
    NALU_START_CODE_FIRST       = 3,
    NALU_START_CODE_INTERVAL    = 4,

} NaluStartCode_t;

// 检查错误
bool Checkerror(int rtperr);

// 寻找起始码
bool FindFirstStartCode(uint8_t* buf);
bool FindIntervalStartCode(uint8_t* buf);

// 打印信息
void PacketPrint(uint8_t* buf, uint32_t len, uint8_t type, uint32_t count);
void NaluPrintf(uint8_t* buf, uint32_t len, uint8_t type, uint32_t count);
void RtpPrintf(uint8_t* buf, uint32_t len);

// NALU转RTP包
void NaluToRtp(std::shared_ptr<RTPSession> sessionPtr, uint8_t* buf, uint32_t len);

// ip地址格式转换
bool IpAddrStrToUint32(const std::string& ipStr, uint32_t& ip);

}; // namespace COMMON