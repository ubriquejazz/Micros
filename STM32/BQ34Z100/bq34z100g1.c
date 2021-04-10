#include "bq34z100g1.h"

void bq34_unsealed () {
    Wire.beginTransmission(BQ34Z100_G1_ADDRESS);
    Wire.write(0x00); // Control
    Wire.write(0x14);
    Wire.write(0x04);
    Wire.endTransmission();
    
    Wire.beginTransmission(BQ34Z100_G1_ADDRESS);
    Wire.write(0x00); // Control
    Wire.write(0x72);
    Wire.write(0x36);
    Wire.endTransmission();
}

uint16_t bq34_read_register(uint8_t address, uint8_t length) {
    Wire.beginTransmission(BQ34Z100_G1_ADDRESS);
    Wire.write(address);
    Wire.endTransmission(false);
    Wire.requestFrom(BQ34Z100_G1_ADDRESS, length, true);
    uint16_t temp = 0;
    for (uint8_t i = 0; i < length; i++) {
        temp |= Wire.read() << (8 * i);
    }
    return temp;
}

uint16_t bq34_read_control(uint8_t address_lsb, uint8_t address_msb) {
    Wire.beginTransmission(BQ34Z100_G1_ADDRESS);
    Wire.write(0x00);
    Wire.write(address_lsb);
    Wire.write(address_msb);
    Wire.endTransmission(true);
    return read_register(0x00, 2);
}

void bq34_read_flash_block(uint8_t sub_class, uint8_t offset) {
    write_reg(0x61, 0x00); // Block control
    write_reg(0x3e, sub_class); // Flash class
    write_reg(0x3f, offset / 32); // Flash block
    
    Wire.beginTransmission(BQ34Z100_G1_ADDRESS);
    Wire.write(0x40); // Block data
    Wire.requestFrom(BQ34Z100_G1_ADDRESS, 32, true);
    for (uint8_t i = 0; i < 32; i++) {
        flash_block_data[i] = Wire.read(); // Data
    }
}

void bq34_write_reg(uint8_t addr, uint8_t val) {
    Wire.beginTransmission(BQ34Z100_G1_ADDRESS);
    Wire.write(addr);
    Wire.write(val);
    Wire.endTransmission(true);
}

void bq34_write_flash_block(uint8_t sub_class, uint8_t offset) {
    write_reg(0x61, 0x00); // Block control
    write_reg(0x3e, sub_class); // Flash class
    write_reg(0x3f, offset / 32); // Flash block
    
    Wire.beginTransmission(BQ34Z100_G1_ADDRESS);
    Wire.write(0x40); // Block data
    for (uint8_t i = 0; i < 32; i++) {
        Wire.write(flash_block_data[i]); // Data
    }
    Wire.endTransmission(true);
}

uint8_t bq34_flash_block_checksum() {
    uint8_t temp = 0;
    for (uint8_t i = 0; i < 32; i++) {
        temp += flash_block_data[i];
    }
    return 255 - temp;
}

double bq34_xemics_to_double(uint32_t value) {
    bool is_negetive = false;
    if (value & 0x800000) {
        is_negetive = true;
    }
    int16_t exp_gain = (value >> 24) - 128 - 24;
    double exponent = pow(2, exp_gain);
    double mantissa = (int32_t)((value & 0xffffff) | 0x800000);
    if (is_negetive) {
        return mantissa * exponent * -1;
    }
    return mantissa * exponent;
}

uint32_t bq34_double_to_xemics(double value) {
    bool is_negetive = false;
    if (value < 0) {
        is_negetive = true;
        value *= -1;
    }
    int8_t exponent;
    if (value > 1) {
        exponent = (log(value) / log(2)) + 1;
    } else {
        exponent = (log(value) / log(2));
    }
    double mantissa = value / (pow(2, (double)exponent) * pow(2, -24));
    if (is_negetive) {
        return (((exponent + 128) << 24) | (uint32_t)mantissa) | 0x800000;
    }
    return ((exponent + 128) << 24) | ((uint32_t)mantissa & 0x7fffff);
}
