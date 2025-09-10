#pragma once

#include <cstdint>
#include <unordered_map>

#ifdef __cplusplus
extern "C" {
#endif

// STM32L4 Memory Map
#define RCC_BASE            0x40021000UL
#define RCC_AHB2ENR         (RCC_BASE + 0x4CUL)

// RCC AHB2 Enable Register bits
#define RCC_AHB2ENR_GPIOAEN 0x00000001UL
#define RCC_AHB2ENR_GPIOBEN 0x00000002UL
#define RCC_AHB2ENR_GPIOCEN 0x00000004UL

// Mock memory interface
class STM32MemoryMock {
public:
    static STM32MemoryMock& instance();
    
    void write_register(uint32_t address, uint32_t value);
    uint32_t read_register(uint32_t address);
    void set_bit(uint32_t address, uint32_t bit_mask);
    void clear_bit(uint32_t address, uint32_t bit_mask);
    
    // Test verification methods
    bool was_register_written(uint32_t address);
    bool is_bit_set(uint32_t address, uint32_t bit_mask);
    uint32_t get_register_value(uint32_t address);
    
    void reset();

private:
    std::unordered_map<uint32_t, uint32_t> memory_;
    std::unordered_map<uint32_t, bool> written_flags_;
};

// C interface for use in macros
void mock_memory_write(uint32_t address, uint32_t value);
uint32_t mock_memory_read(uint32_t address);
void mock_memory_set_bit(uint32_t address, uint32_t bit_mask);

#ifdef __cplusplus
}
#endif

// Redefine STM32 memory access for testing
#ifdef UNIT_TESTING

// Mock the memory-mapped register access
#undef RCC
#define RCC ((RCC_TypeDef*)0x40021000UL)

// Mock register structure
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t ICSCR;
    volatile uint32_t CFGR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t PLLSAI1CFGR;
    volatile uint32_t PLLSAI2CFGR;
    volatile uint32_t CIER;
    volatile uint32_t CIFR;
    volatile uint32_t CICR;
    uint32_t RESERVED0;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED1;
    volatile uint32_t APB1RSTR1;
    volatile uint32_t APB1RSTR2;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED2;
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;    // This is what we want to mock
    volatile uint32_t AHB3ENR;
    uint32_t RESERVED3;
    volatile uint32_t APB1ENR1;
    volatile uint32_t APB1ENR2;
    volatile uint32_t APB2ENR;
} RCC_TypeDef;

// Redefine the RCC macro to use our mock
#undef __HAL_RCC_GPIOB_CLK_ENABLE
#define __HAL_RCC_GPIOB_CLK_ENABLE() \
    do { \
        mock_memory_set_bit(RCC_AHB2ENR, RCC_AHB2ENR_GPIOBEN); \
    } while(0)

#undef __HAL_RCC_GPIOA_CLK_ENABLE
#define __HAL_RCC_GPIOA_CLK_ENABLE() \
    do { \
        mock_memory_set_bit(RCC_AHB2ENR, RCC_AHB2ENR_GPIOAEN); \
    } while(0)

#undef __HAL_RCC_GPIOC_CLK_ENABLE
#define __HAL_RCC_GPIOC_CLK_ENABLE() \
    do { \
        mock_memory_set_bit(RCC_AHB2ENR, RCC_AHB2ENR_GPIOCEN); \
    } while(0)

#endif // UNIT_TESTING
