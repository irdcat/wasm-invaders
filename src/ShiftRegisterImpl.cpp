#include "ShiftRegisterImpl.hpp"

ShiftRegisterImpl::ShiftRegisterImpl()
    : shift_low(0x00)
    , shift_high(0x00)
    , shift_offset(0x00)
{
}

u8 ShiftRegisterImpl::readShiftResult() const
{
    const u16 cached_value = (shift_high << 8) | shift_low;
    return (cached_value >> (8 - shift_offset)) & 0xFF;
}

void ShiftRegisterImpl::writeShiftOffset(u8 value)
{
    shift_offset = value & 7;
}

void ShiftRegisterImpl::writeShiftData(u8 value)
{
    shift_low = shift_high;
    shift_high = value;
}