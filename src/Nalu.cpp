#include <map>
#include <iomanip>
#include <sstream>
#include "AnnexB/Nalu.h"
using Common::Nalu_t;

namespace AnnexB {

// nalu类型map
static std::map<uint8_t, std::string> NaluTypeMap = {
    {0x01, "Non-IDR slice"},
    {0x02, "Partition A"},
    {0x03, "Partition B"},
    {0x04, "Partition C"},
    {0x05, "IDR slice"},
    {0x06, "SEI"},
    {0x07, "SPS"},
    {0x08, "PPS"},
    {0x09, "AUD"},
    {0x0a, "End of sequence"},
    {0x0b, "End of stream"},
    {0x0c, "Filler data"},
    {0x0d, "SPSExt"},
    {0x0e, "Prefix NAL unit"},
    {0x0f, "Subset SPS"}
};

static std::string GetNaluTypeStr(uint8_t naluType) {
    auto it = NaluTypeMap.find(naluType);
    return it == NaluTypeMap.end() ? std::to_string(naluType) : it->second;
}

void Nalu::setStartCodeLen(std::size_t startCodeLen) {
    m_startCodeLen = startCodeLen;
}

void Nalu::setBuffer(const uint8_t* buffer, size_t bufferLen) {
    m_buffer.assign(buffer, buffer + bufferLen);
}

void Nalu::clear() {
    m_startCodeLen = 0;
    m_buffer.clear();
}

bool Nalu::isValid() const {
    if (m_buffer.empty() || 0 == m_startCodeLen) {
        return false;
    }

    if (m_buffer.size() <= m_startCodeLen) {
        return false;
    }

    return true;
}

uint8_t Nalu::getHead() const {
    return !this->isValid() ? 0 : m_buffer[m_startCodeLen];
}

const uint8_t* Nalu::getBody(std::size_t& size) const {
    if (!isValid()) {
        size = 0;
        return nullptr;
    }

    size = m_buffer.size() - m_startCodeLen - 1;
    return &m_buffer[m_startCodeLen + 1];
}

Nalu_t Nalu::getNaluType() const {
    uint8_t naluHead = this->getHead();
    return static_cast<Nalu_t>(naluHead & 0x1f);
}

std::size_t Nalu::getStartCodeLen() const {
    return m_startCodeLen;
}

std::string Nalu::dumpString() const {
    if (!this->isValid()) {
        return "";
    }

    std::stringstream ss;
    ss << "NALU ========================" << std::endl;
    ss << "len: " << m_buffer.size() << std::endl;

    ss << "START CODE: -----------------" << std::endl;
    for (auto idx = 0; idx < m_startCodeLen; ++idx) {
        ss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(m_buffer[idx]) << " ";
    }
    ss << std::endl;

    ss << "NALU HEAD: ------------------" << std::endl;
    uint8_t naluHead = this->getHead();
    ss << "forbidden_bit: " << static_cast<int>((naluHead >> 7) & 1) << std::endl;
    ss << "nal_ref_idc: " << static_cast<int>((naluHead >> 5) & 3) << std::endl;
    ss << "nal_unit_type: " << GetNaluTypeStr((naluHead >> 0) & 0x1f) << std::endl;

    ss << "NALU BODY: ------------------" << std::endl;
    for (auto idx = m_startCodeLen + 1; idx < m_buffer.size(); ++idx) {
        ss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(m_buffer[idx]) << " ";
    }
    ss << std::endl;

    return ss.str();
}

} // namespace TestAnnexBFileReader