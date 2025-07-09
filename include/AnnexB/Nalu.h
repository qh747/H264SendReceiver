#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "Common/DataDef.h"

namespace AnnexB {

/**
 * @brief NALU类
 */
class Nalu {
public:
    /**
     * @brief 设置起始码长度
     * @param startCodeLen 起始码长度
     */
    void setStartCodeLen(std::size_t startCodeLen);

    /**
     * @brief 设置nalu数据
     * @param buffer nalu数据
     * @param bufferLen nalu数据长度
     */
    void setBuffer(const uint8_t* buffer, size_t bufferLen);

    /**
     * @brief 清空nalu数据
     */
    void clear();

public:
    /**
     * @brief  判断nalu数据是否有效
     * @return 是否有效
     */
    bool isValid() const;

    /**
     * @brief  获取nalu数据首部
     * @return nalu数据首部
     */
    uint8_t getHead() const;

    /**
     * @brief  获取nalu数据
     * @return nalu数据
     * @param  size nalu数据长度
     */
    const uint8_t* getBody(std::size_t& size) const;

    /**
     * @brief  获取nalu类型
     * @return nalu类型
     */
    Common::Nalu_t getNaluType() const;

    /**
     * @brief  获取起始码长度
     * @return 起始码长度
     */
    std::size_t getStartCodeLen() const;

public:
    /**
     * @brief  输出nalu数据
     * @return nalu数据
     */
    std::string dumpString() const;

private:
    // 起始码长度
    std::size_t m_startCodeLen { 0 };

    // nalu数据
    std::vector<uint8_t> m_buffer;
};

}; // namespace TestAnnexBFileReader