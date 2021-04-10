#include "bq34_standard.h"

uint16_t bq34std_control_status() {
    return bq34_read_control(0x00, 0x00);
}

uint16_t bq34std_device_type() {
    return bq34_read_control(0x01, 0x00);
}

uint16_t bq34std_fw_version() {
    return bq34_read_control(0x02, 0x00);
}

uint16_t bq34std_hw_version() {
    return bq34_read_control(0x03, 0x00);
}

uint16_t bq34std_reset_data() {
    return bq34_read_control(0x05, 0x00);
}

uint16_t bq34std_prev_macwrite() {
    return bq34_read_control(0x07, 0x00);
}

uint16_t bq34std_chem_id() {
    return bq34_read_control(0x08, 0x00);
}

uint16_t bq34std_board_offset() {
    return bq34_read_control(0x09, 0x00);
}

uint16_t bq34std_cc_offset() {
    return bq34_read_control(0x0a, 0x00);
}

uint16_t bq34std_cc_offset_save() {
    return bq34_read_control(0x0b, 0x00);
}

uint16_t bq34std_df_version() {
    return bq34_read_control(0x0c, 0x00);
}

uint16_t bq34std_set_fullsleep() {
    return bq34_read_control(0x10, 0x00);
}

uint16_t bq34std_static_chem_chksum() {
    return bq34_read_control(0x17, 0x00);
}

uint16_t bq34std_sealed() {
    return bq34_read_control(0x20, 0x00);
}

uint16_t bq34std_it_enable() {
    return bq34_read_control(0x21, 0x00);
}

uint16_t bq34std_cal_enable() {
    return bq34_read_control(0x2d, 0x00);
}

uint16_t bq34std_reset() {
    return bq34_read_control(0x41, 0x00);
}

uint16_t bq34std_exit_cal() {
    return bq34_read_control(0x80, 0x00);
}

uint16_t bq34std_enter_cal() {
    return bq34_read_control(0x81, 0x00);
}

uint16_t bq34std_offset_cal() {
    return bq34_read_control(0x82, 0x00);
}

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

uint16_t bq34std_average_time_to_empty() {
    return bq34_read_register(0x18, 2);
}

uint16_t bq34std_average_time_to_full() {
    return bq34_read_register(0x1a, 2);
}

int16_t bq34std_passed_charge() {
    return bq34_read_register(0x1c, 2);
}

uint16_t bq34std_do_d0_time() {
    return bq34_read_register(0x1e, 2);
}

uint16_t bq34std_available_energy() {
    return bq34_read_register(0x24, 2);
}

uint16_t bq34std_average_power() {
    return bq34_read_register(0x26, 2);
}

uint16_t bq34std_serial_number() {
    return bq34_read_register(0x28, 2);
}

uint16_t bq34std_internal_temperature() {
    return bq34_read_register(0x2a, 2);
}

uint16_t bq34std_cycle_count() {
    return bq34_read_register(0x2c, 2);
}

uint16_t bq34std_state_of_health() {
    return bq34_read_register(0x2e, 2);
}

uint16_t bq34std_charge_voltage() {
    return bq34_read_register(0x30, 2);
}

uint16_t bq34std_charge_current() {
    return bq34_read_register(0x32, 2);
}

uint16_t bq34std_pack_configuration() {
    return bq34_read_register(0x3a, 2);
}

uint16_t bq34std_design_capacity() {
    return bq34_read_register(0x3c, 2);
}

uint8_t bq34std_grid_number() {
    return (uint8_t)bq34_read_register(0x62, 1);
}

uint8_t bq34std_learned_status() {
    return (uint8_t)bq34_read_register(0x63, 1);
}

uint16_t bq34std_dod_at_eoc() {
    return bq34_read_register(0x64, 2);
}

uint16_t bq34std_q_start() {
    return bq34_read_register(0x66, 2);
}

uint16_t bq34std_true_fcc() {
    return bq34_read_register(0x6a, 2);
}

uint16_t bq34std_state_time() {
    return bq34_read_register(0x6c, 2);
}

uint16_t bq34std_q_max_passed_q() {
    return bq34_read_register(0x6e, 2);
}

uint16_t bq34std_dod_0() {
    return bq34_read_register(0x70, 2);
}

uint16_t bq34std_q_max_dod_0() {
    return bq34_read_register(0x72, 2);
}

uint16_t bq34std_q_max_time() {
    return bq34_read_register(0x74, 2);
}

void bq34std_enter_calibration() {
    bq34_unsealed ();
    do {
        bq34std_cal_enable();
        bq34std_enter_cal();
        OsDelay(1000);
    } while (!(bq34_control_status() & 0x1000)); // CALEN
}

void bq34std_exit_calibration() {
    do {
        bq34std_exit_cal();
        OsDelay(1000);
    } while (!(bq34_control_status() &~ 0x1000)); // CALEN
    bq34_checksum(150);
}

void bq34std_calibrate_cc_offset() {
    bq34std_enter_calibration();
    do {
        bq34std_cc_offset();
        OsDelay(1000);
    } while (!(bq34_control_status() & 0x0800)); // CCA
    
    do {
        OsDelay(1000);
    } while (!(bq34_control_status() &~ 0x0800)); // CCA
    
    bq34std_cc_offset_save();
    bq34std_exit_calibration();
}

void bq34std_calibrate_board_offset() {
    bq34std_enter_calibration();
    do {
        bq34std_board_offset();
        OsDelay(1000);
    } while (!(bq34_control_status() & 0x0c00)); // CCA + BCA
    
    do {
        OsDelay(1000);
    } while (!(bq34_control_status() &~ 0x0c00)); // CCA + BCA
    
    bq34std_cc_offset_save();
    bq34std_exit_calibration();
}