#include "registers.h"
#include "log.h"
#include <stdio.h>
#include <string.h>

// Register value storage (50 registers)
uint8_t modbus_registers[50] = {0};

// Register descriptor table
RegisterDescriptor register_table[] = {
    {REG_DAC_A,          0x0000, REG_TYPE_UINT16, REG_LEN_UINT16, WRITABLE},    // Register address 0x0000, 16-bit data, writable
    {REG_DAC_B,          0x0002, REG_TYPE_UINT16, REG_LEN_UINT16, WRITABLE},    // Register address 0x0002, 16-bit data, writable
    {REG_DAC_C,          0x0004, REG_TYPE_UINT16, REG_LEN_UINT16, WRITABLE},    // Register address 0x0004, 16-bit data, writable
    {REG_DAC_D,          0x0006, REG_TYPE_UINT16, REG_LEN_UINT16, WRITABLE},    // Register address 0x0006, 16-bit data, writable
//    {REG_PowerSetting,   0x000A, REG_TYPE_FLOAT , REG_LEN_FLOAT , WRITABLE},    // Register address 0x000A, 32-bit float, writable

};

// Helper macro to get the size of register_table
#define REGISTER_COUNT (sizeof(register_table) / sizeof(RegisterDescriptor))

// Function to write to a register
void write_register(uint16_t address, uint32_t value) {
    for (int i = 0; i < REGISTER_COUNT; i++) {
        if (register_table[i].address == address) {
            RegisterDescriptor *reg = &register_table[i];

            // Check if register is writable
            if (reg->readonly) {
                EMLOG(LOG_ERROR, "Attempt to write to a read-only register.");
                return;
            }

            // Boundary check
            if (address + reg->length > sizeof(modbus_registers)) {
                EMLOG(LOG_ERROR, "Register address out of bounds.");
                return;
            }

            switch (reg->type) {
                case REG_TYPE_UINT8:
                    modbus_registers[address] = (uint8_t)value;
                    break;
                case REG_TYPE_UINT16:
                    modbus_registers[address] = (uint8_t)((value >> 8) & 0xFF);
                    modbus_registers[address + 1] = (uint8_t)(value & 0xFF);
                    break;
                case REG_TYPE_UINT32:
                    modbus_registers[address] = (uint8_t)((value >> 24) & 0xFF);
                    modbus_registers[address + 1] = (uint8_t)((value >> 16) & 0xFF);
                    modbus_registers[address + 2] = (uint8_t)((value >> 8) & 0xFF);
                    modbus_registers[address + 3] = (uint8_t)(value & 0xFF);
                    break;
                case REG_TYPE_FLOAT:
                    memcpy(&modbus_registers[address], &value, sizeof(float));
                    break;
                default:
                    EMLOG(LOG_ERROR, "Unsupported register type.");
                    break;
            }
            return;
        }
    }
    EMLOG(LOG_ERROR, "Register not found.");
}

// Function to read from a register
uint32_t read_register(uint16_t address) {
    for (int i = 0; i < REGISTER_COUNT; i++) {
        if (register_table[i].address == address) {
            RegisterDescriptor *reg = &register_table[i];

            // Boundary check
            if (address + reg->length > sizeof(modbus_registers)) {
                EMLOG(LOG_ERROR, "Register address out of bounds.");
                return 0xFFFFFFFF; // Indicate error
            }

            uint32_t value = 0;
            switch (reg->type) {
                case REG_TYPE_UINT8:
                    value = modbus_registers[address];
                    break;
                case REG_TYPE_UINT16:
                    value = (modbus_registers[address] << 8) | modbus_registers[address + 1];
                    break;
                case REG_TYPE_UINT32:
                    value = (modbus_registers[address] << 24) |
                            (modbus_registers[address + 1] << 16) |
                            (modbus_registers[address + 2] << 8) |
                            modbus_registers[address + 3];
                    break;
                case REG_TYPE_FLOAT:
                    memcpy(&value, &modbus_registers[address], sizeof(float));
                    break;
                default:
                    EMLOG(LOG_ERROR, "Unsupported register type.");
                    break;
            }
            return value;
        }
    }
    EMLOG(LOG_ERROR, "Register not found.");
    return 0xFFFFFFFF; // Indicate error
}





























