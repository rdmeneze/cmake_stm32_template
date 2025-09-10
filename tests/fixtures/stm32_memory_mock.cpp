#include "stm32_memory_mock.h"
#include <stdexcept>

STM32MemoryMock& STM32MemoryMock::instance() {
    static STM32MemoryMock instance;
    return instance;
}

void STM32MemoryMock::write_register(uint32_t address, uint32_t value) {
    memory_[address] = value;
    written_flags_[address] = true;
}

uint32_t STM32MemoryMock::read_register(uint32_t address) {
    auto it = memory_.find(address);
    return (it != memory_.end()) ? it->second : 0;
}

void STM32MemoryMock::set_bit(uint32_t address, uint32_t bit_mask) {
    uint32_t current_value = read_register(address);
    write_register(address, current_value | bit_mask);
}

void STM32MemoryMock::clear_bit(uint32_t address, uint32_t bit_mask) {
    uint32_t current_value = read_register(address);
    write_register(address, current_value & ~bit_mask);
}

bool STM32MemoryMock::was_register_written(uint32_t address) {
    auto it = written_flags_.find(address);
    return (it != written_flags_.end()) ? it->second : false;
}

bool STM32MemoryMock::is_bit_set(uint32_t address, uint32_t bit_mask) {
    uint32_t value = read_register(address);
    return (value & bit_mask) == bit_mask;
}

uint32_t STM32MemoryMock::get_register_value(uint32_t address) {
    return read_register(address);
}

void STM32MemoryMock::reset() {
    memory_.clear();
    written_flags_.clear();
}

// C interface implementation
extern "C" {
    void mock_memory_write(uint32_t address, uint32_t value) {
        STM32MemoryMock::instance().write_register(address, value);
    }

    uint32_t mock_memory_read(uint32_t address) {
        return STM32MemoryMock::instance().read_register(address);
    }

    void mock_memory_set_bit(uint32_t address, uint32_t bit_mask) {
        STM32MemoryMock::instance().set_bit(address, bit_mask);
    }
}
