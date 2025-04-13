#pragma once
#include <memory>
#include <string>
#include <cstdio>
#include <cstdbool>
#include <cstdint>
#include <jrtplib3/rtpsession.h>
#include <jrtplib3/rtpsessionparams.h>
#include <jrtplib3/rtpudpv4transmitter.h>
using jrtplib::RTPPacket;
using jrtplib::RTPSession;
using jrtplib::RTPSessionParams;
using jrtplib::RTPUDPv4TransmissionParams;

namespace SENDER_RECEIVER {

/**
 * @brief H264文件发送接收类
 */
class H264FileSendReceiver
{
public:
    using Ptr = std::shared_ptr<H264FileSendReceiver>;
    using SessionPtr = std::shared_ptr<RTPSession>;
    using SessionParamPtr = std::shared_ptr<RTPSessionParams>;
    using TransmissionParamPtr = std::shared_ptr<RTPUDPv4TransmissionParams>;

public:
	H264FileSendReceiver() = default;
	~H264FileSendReceiver() = default;

public:
    /**
     * @brief               初始化发送接收类
     * @return              初始化结果，成功返回true，否则返回false
     * @param ipAddr        本地ip地址
     * @param port          本地端口
     * @param remoteIp      远端ip地址
     * @param remotePort    远端端口
     */
    bool                    initialize(const std::string& ipAddr, uint16_t port, const std::string& remoteIp, uint16_t remotePort);

    /**
     * @note                example: filePath(/home/user/filepath), fileName(file.h264)
     * @brief               发送H264文件
     * @return              发送结果，成功返回true，否则返回false
     * @param filePath      文件路径
     * @param fileName      文件名称
     */ 
    bool                    sendFile(const std::string& filePath, const std::string& fileName);

    /** 
     * @note                example: filePath(/home/user/filepath), fileName(file.h264)
     * @brief               接收H264文件
     * @return              接收结果，成功返回true，否则返回false
     * @param filePath      文件路径
     * @param fileName      文件名称
     */ 
    bool                    recvFile(const std::string& filePath, const std::string& fileName);

private:
    //                      RTP会话
    SessionPtr              m_sessionPtr;

    //                      RTP会话参数
    SessionParamPtr         m_sessionParamPtr;

    //                      RTP会话传输参数
    TransmissionParamPtr    m_transmissionParamPtr;
};

}; // namespace SENDER