#include "bq34_standard.h"

uint16_t BQ34STD_control_status() {
    return bq34_read_control(0x00, 0x00);
}

uint16_t BQ34STD_device_type() {
    return bq34_read_control(0x01, 0x00);
}

uint16_t BQ34STD_fw_version() {
    return bq34_read_control(0x02, 0x00);
}

uint16_t BQ34STD_hw_version() {
    return bq34_read_control(0x03, 0x00);
}

uint16_t BQ34STD_reset_data() {
    return bq34_read_control(0x05, 0x00);
}

uint16_t BQ34STD_prev_macwrite() {
    return bq34_read_control(0x07, 0x00);
}

uint16_t BQ34STD_chem_id() {
    return bq34_read_control(0x08, 0x00);
}

uint16_t BQ34STD_board_offset() {
    return bq34_read_control(0x09, 0x00);
}

uint16_t BQ34STD_cc_offset() {
    return bq34_read_control(0x0a, 0x00);
}

uint16_t BQ34STD_cc_offset_save() {
    return bq34_read_control(0x0b, 0x00);
}

uint16_t BQ34STD_df_version() {
    return bq34_read_control(0x0c, 0x00);
}

uint16_t BQ34STD_set_fullsleep() {
    return bq34_read_control(0x10, 0x00);
}

uint16_t BQ34STD_static_chem_chksum() {
    return bq34_read_control(0x17, 0x00);
}

uint16_t BQ34STD_sealed() {
    return bq34_read_control(0x20, 0x00);
}

uint16_t BQ34STD_it_enable() {
    return bq34_read_control(0x21, 0x00);
}

uint16_t BQ34STD_cal_enable() {
    return bq34_read_control(0x2d, 0x00);
}

uint16_t BQ34STD_reset() {
    return bq34_read_control(0x41, 0x00);
}

uint16_t BQ34STD_exit_cal() {
    return bq34_read_control(0x80, 0x00);
}

uint16_t BQ34STD_enter_cal() {
    return bq34_read_control(0x81, 0x00);
}

uint16_t BQ34STD_offset_cal() {
    return bq34_read_control(0x82, 0x00);
}

uint8_t BQ34STD_state_of_charge() {
    return (uint8_t)bq34_read_register(0x02, 1);
}

uint8_t BQ34STD_state_of_charge_max_error() {
    return (uint8_t)bq34_read_register(0x03, 1);
}

uint16_t BQ34STD_remaining_capacity() {
    return bq34_read_register(0x04, 2);
}

uint16_t BQ34STD_full_charge_capacity() {
    return bq34_read_register(0x06, 2);
}

uint16_t BQ34STD_voltage() {
    return bq34_read_register(0x08, 2);
}

int16_t BQ34STD_average_current() {
    return (int16_t)bq34_read_register(0x0a, 2);
}

uint16_t BQ34STD_temperature() {
    return bq34_read_register(0x0c, 2);
}

uint16_t BQ34STD_flags() {
    return bq34_read_register(0x0e, 2);
}

uint16_t BQ34STD_flags_b() {
    return bq34_read_register(0x12, 2);
}

int16_t BQ34STD_current() {
    return (int16_t)bq34_read_register(0x10, 2);
}

uint16_t BQ34STD_average_time_to_empty() {
    return bq34_read_register(0x18, 2);
}

uint16_t BQ34STD_average_time_to_full() {
    return bq34_read_register(0x1a, 2);
}

int16_t BQ34STD_passed_charge() {
    return bq34_read_register(0x1c, 2);
}

uint16_t BQ34STD_do_d0_time() {
    return bq34_read_register(0x1e, 2);
}

uint16_t BQ34STD_available_energy() {
    return bq34_read_register(0x24, 2);
}

uint16_t BQ34STD_average_power() {
    return bq34_read_register(0x26, 2);
}

uint16_t BQ34STD_serial_number() {
    return bq34_read_register(0x28, 2);
}

uint16_t BQ34STD_internal_temperature() {
    return bq34_read_register(0x2a, 2);
}

uint16_t BQ34STD_cycle_count() {
    return bq34_read_register(0x2c, 2);
}

uint16_t BQ34STD_state_of_health() {
    return bq34_read_register(0x2e, 2);
}

uint16_t BQ34STD_charge_voltage() {
    return bq34_read_register(0x30, 2);
}

uint16_t BQ34STD_charge_current() {
    return bq34_read_register(0x32, 2);
}

uint16_t BQ34STD_pack_configuration() {
    return bq34_read_register(0x3a, 2);
}

uint16_t BQ34STD_design_capacity() {
    return bq34_read_register(0x3c, 2);
}

uint8_t BQ34STD_grid_number() {
    return (uint8_t)bq34_read_register(0x62, 1);
}

uint8_t BQ34STD_learned_status() {
    return (uint8_t)bq34_read_register(0x63, 1);
}

uint16_t BQ34STD_dod_at_eoc() {
    return bq34_read_register(0x64, 2);
}

uint16_t BQ34STD_q_start() {
    return bq34_read_register(0x66, 2);
}

uint16_t BQ34STD_true_fcc() {
    return bq34_read_register(0x6a, 2);
}

uint16_t BQ34STD_state_time() {
    return bq34_read_register(0x6c, 2);
}

uint16_t BQ34STD_q_max_passed_q() {
    return bq34_read_register(0x6e, 2);
}

uint16_t BQ34STD_dod_0() {
    return bq34_read_register(0x70, 2);
}

uint16_t BQ34STD_q_max_dod_0() {
    return bq34_read_register(0x72, 2);
}

uint16_t BQ34STD_q_max_time() {
    return bq34_read_register(0x74, 2);
}
