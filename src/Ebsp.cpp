#include <iomanip>
#include <string>
#include <sstream>
#include "AnnexB/Ebsp.h"

namespace AnnexB {

void Ebsp::setBuffer(const uint8_t* buffer, size_t bufferLen) {
    m_buffer.assign(buffer, buffer + bufferLen);
}

void Ebsp::clear() {
    m_buffer.clear();
}

bool Ebsp::isValid() const {
    return !m_buffer.empty();
}

const uint8_t* Ebsp::getBuffer(std::size_t& size) const {
    size = m_buffer.size();
    return m_buffer.data();
}

std::string Ebsp::dumpString() const {
    std::stringstream ss;
    ss << "EBSP ========================" << std::endl;
    ss << "len: " << m_buffer.size() << std::endl;

    ss << "EBSP BODY: ------------------" << std::endl;
    for (const auto& val : m_buffer) {
        ss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(val) << " ";
    }
    ss << std::endl;

    return ss.str();
}

} // namespace AnnexB