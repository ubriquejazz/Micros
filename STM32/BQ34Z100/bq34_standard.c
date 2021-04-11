#include "bq34_standard.h"

uint8_t bq34std_state_of_charge() {
    return (uint8_t)bq34_read_register(0x02, 1);
}

uint8_t bq34std_state_of_charge_max_error() {
    return (uint8_t)bq34_read_register(0x03, 1);
}

uint16_t bq34std_remaining_capacity() {
    return bq34_read_register(0x04, 2);
}

uint16_t bq34std_full_charge_capacity() {
    return bq34_read_register(0x06, 2);
}

uint16_t bq34std_voltage() {
    return bq34_read_register(0x08, 2);
}

int16_t bq34std_average_current() {
    return (int16_t)bq34_read_register(0x0a, 2);
}

uint16_t bq34std_temperature() {
    return bq34_read_register(0x0c, 2);
}

uint16_t bq34std_flags() {
    return bq34_read_register(0x0e, 2);
}

uint16_t bq34std_flags_b() {
    return bq34_read_register(0x12, 2);
}

int16_t bq34std_current() {
    return (int16_t)bq34_read_register(0x10, 2);
}
