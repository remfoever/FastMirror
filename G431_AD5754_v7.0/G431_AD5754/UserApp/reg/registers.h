#ifndef __REGISTERS_H_
#define __REGISTERS_H_

#include <stdint.h>

#define WRITABLE 		0
#define READ_ONLY		1

#define REG_LEN_UINT8			1
#define REG_LEN_UINT16		2
#define REG_LEN_UINT32		4
#define REG_LEN_FLOAT			4


// Register name enumeration
typedef enum {
    REG_DAC_A,          // DAC_A_Register
    REG_DAC_B,          // DAC_B_Register
    REG_DAC_C,          // DAC_C_Register
    REG_DAC_D,          // DAC_D_Register
//    REG_PowerSetting,   // PowerSettingRegister
} RegisterName;

// Register type enumeration
typedef enum {
    REG_TYPE_UINT8,  // 8-bit unsigned integer
    REG_TYPE_UINT16, // 16-bit unsigned integer
    REG_TYPE_UINT32, // 32-bit unsigned integer
    REG_TYPE_FLOAT,  // 32-bit floating point
} RegType;

// Register descriptor structure
typedef struct {
    RegisterName name;  // Register name (enumerated type)
    uint16_t address;   // Starting address of the register
    RegType type;       // Data type of the register
    uint16_t length;    // Length of the register data in bytes
    uint8_t readonly;   // Read-only flag: 0 for writable, 1 for read-only
} RegisterDescriptor;


// Function to write to a register
void write_register(uint16_t address, uint32_t value);

// Function to read from a register
uint32_t read_register(uint16_t address);

#endif // REGISTERS_H










