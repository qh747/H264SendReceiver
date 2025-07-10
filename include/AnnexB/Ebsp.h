#pragma once
#include <vector>
#include <cstdint>

namespace AnnexB {

/**
 * @brief Ebsp类
 * @note  Ebsp的数据内容为Nalu去除起始码后的数据
 * +---------------+---------------+---------------+---------------+
 * | 1             | 2             | 3             | 4             |
 * +---------------+---------------+---------------+---------------+
 * |F|NRI|  Type   |               Nalu Body                       |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+---------------+---------------+
 *
 */
class Ebsp {
public:
    /**
     * @brief 设置ebsp数据
     * @param buffer ebsp数据
     * @param bufferLen ebsp数据长度
     */
    void setBuffer(const uint8_t* buffer, size_t bufferLen);

    /**
     * @brief 清空ebsp数据
     */
    void clear();

public:
    /**
     * @brief  判断ebsp数据是否有效
     * @return 是否有效
     */
    bool isValid() const;

    /**
     * @brief  获取ebsp数据
     * @return ebsp数据
     * @param  size ebsp数据长度
     */
    const uint8_t* getBuffer(std::size_t& size) const;

    /**
     * @brief  输出nalu数据
     * @return nalu数据
     */
    std::string dumpString() const;

private:
    // 存储EBSP数据
    std::vector<uint8_t> m_buffer;
};

}; // namespace AnnexB