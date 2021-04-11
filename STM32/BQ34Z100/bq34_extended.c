#include "bq34_extended.h"

uint16_t bq34ext_average_time_to_empty() {
    return bq34_read_register(0x18, 2);
}

uint16_t bq34ext_average_time_to_full() {
    return bq34_read_register(0x1a, 2);
}

int16_t bq34ext_passed_charge() {
    return bq34_read_register(0x1c, 2);
}

uint16_t bq34ext_do_d0_time() {
    return bq34_read_register(0x1e, 2);
}

uint16_t bq34ext_available_energy() {
    return bq34_read_register(0x24, 2);
}

uint16_t bq34ext_average_power() {
    return bq34_read_register(0x26, 2);
}

uint16_t bq34ext_serial_number() {
    return bq34_read_register(0x28, 2);
}

uint16_t bq34ext_internal_temperature() {
    return bq34_read_register(0x2a, 2);
}

uint16_t bq34ext_cycle_count() {
    return bq34_read_register(0x2c, 2);
}

uint16_t bq34ext_state_of_health() {
    return bq34_read_register(0x2e, 2);
}

uint16_t bq34ext_charge_voltage() {
    return bq34_read_register(0x30, 2);
}

uint16_t bq34ext_charge_current() {
    return bq34_read_register(0x32, 2);
}

uint16_t bq34ext_pack_configuration() {
    return bq34_read_register(0x3a, 2);
}

uint16_t bq34ext_design_capacity() {
    return bq34_read_register(0x3c, 2);
}


/* bq34fl */


uint8_t bq34ext_grid_number() {
    return (uint8_t)bq34_read_register(0x62, 1);
}

uint8_t bq34ext_learned_status() {
    return (uint8_t)bq34_read_register(0x63, 1);
}

uint16_t bq34ext_dod_at_eoc() {
    return bq34_read_register(0x64, 2);
}

uint16_t bq34ext_q_start() {
    return bq34_read_register(0x66, 2);
}

uint16_t bq34ext_true_fcc() {
    return bq34_read_register(0x6a, 2);
}

uint16_t bq34ext_state_time() {
    return bq34_read_register(0x6c, 2);
}

uint16_t bq34ext_q_max_passed_q() {
    return bq34_read_register(0x6e, 2);
}

uint16_t bq34ext_dod_0() {
    return bq34_read_register(0x70, 2);
}

uint16_t bq34ext_q_max_dod_0() {
    return bq34_read_register(0x72, 2);
}

uint16_t bq34ext_q_max_time() {
    return bq34_read_register(0x74, 2);
}
