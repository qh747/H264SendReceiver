#pragma once
#include <cstdint>

namespace Common {

/**
 * @brief NALU类型
 */
typedef enum class NALU_TYPE : uint8_t {
    Unspecified     = 0x00,
    NonIDRSlice     = 0x01,
    PartitionA      = 0x02,
    PartitionB      = 0x03,
    PartitionC      = 0x04,
    IDRSlice        = 0x05,
    SEI             = 0x06,
    SPS             = 0x07,
    PPS             = 0x08,
    AUD             = 0x09,
    EndOfSequence   = 0x0a,
    EndOfStream     = 0x0b,
    FillerData      = 0x0c,
    SPSExt          = 0x0d,
    PrefixNalUnit   = 0x0e,
    SubsetSPS       = 0x0f,

} Nalu_t;

}; // namespace Common
