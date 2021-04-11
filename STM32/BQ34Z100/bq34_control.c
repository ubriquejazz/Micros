#include "bq34_control.h"


uint16_t bq34ctl_read_control(uint8_t address_lsb, uint8_t address_msb) {
    Wire.beginTransmission(BQ34Z100_G1_ADDRESS);
    Wire.write(0x00);
    Wire.write(address_lsb);
    Wire.write(address_msb);
    Wire.endTransmission(true);
    return read_register(0x00, 2);
}

uint16_t bq34ctl_control_status() {
    return bq34ctl_read_control(0x00, 0x00);
}

uint16_t bq34ctl_device_type() {
    return bq34ctl_read_control(0x01, 0x00);
}

uint16_t bq34ctl_fw_version() {
    return bq34ctl_read_control(0x02, 0x00);
}

uint16_t bq34ctl_hw_version() {
    return bq34ctl_read_control(0x03, 0x00);
}

uint16_t bq34ctl_reset_data() {
    return bq34ctl_read_control(0x05, 0x00);
}

uint16_t bq34ctl_prev_macwrite() {
    return bq34ctl_read_control(0x07, 0x00);
}

uint16_t bq34ctl_chem_id() {
    return bq34ctl_read_control(0x08, 0x00);
}

uint16_t bq34ctl_board_offset() {
    return bq34ctl_read_control(0x09, 0x00);
}

uint16_t bq34ctl_cc_offset() {
    return bq34ctl_read_control(0x0a, 0x00);
}

uint16_t bq34ctl_cc_offset_save() {
    return bq34ctl_read_control(0x0b, 0x00);
}

uint16_t bq34ctl_df_version() {
    return bq34ctl_read_control(0x0c, 0x00);
}

uint16_t bq34ctl_set_fullsleep() {
    return bq34ctl_read_control(0x10, 0x00);
}

uint16_t bq34ctl_static_chem_chksum() {
    return bq34ctl_read_control(0x17, 0x00);
}

uint16_t bq34ctl_sealed() {
    return bq34ctl_read_control(0x20, 0x00);
}

uint16_t bq34ctl_it_enable() {
    return bq34ctl_read_control(0x21, 0x00);
}

uint16_t bq34ctl_cal_enable() {
    return bq34ctl_read_control(0x2d, 0x00);
}

uint16_t bq34ctl_reset() {
    return bq34ctl_read_control(0x41, 0x00);
}

uint16_t bq34ctl_exit_cal() {
    return bq34ctl_read_control(0x80, 0x00);
}

uint16_t bq34ctl_enter_cal() {
    return bq34ctl_read_control(0x81, 0x00);
}

uint16_t bq34ctl_offset_cal() {
    return bq34ctl_read_control(0x82, 0x00);
}

void bq34ctl_enter_calibration() {
    bq34_unsealed ();
    do {
        bq34ctl_cal_enable();
        bq34ctl_enter_cal();
        OsDelay(1000);
    } while (!(bq34_control_status() & 0x1000)); // CALEN
}

void bq34ctl_exit_calibration() {
    do {
        bq34ctl_exit_cal();
        OsDelay(1000);
    } while (!(bq34_control_status() &~ 0x1000)); // CALEN
    // bq34_checksum(150);
}

void bq34ctl_calibrate_cc_offset() {
    bq34ctl_enter_calibration();
    do {
        bq34ctl_cc_offset();
        OsDelay(1000);
    } while (!(bq34_control_status() & 0x0800)); // CCA
    
    do {
        OsDelay(1000);
    } while (!(bq34_control_status() &~ 0x0800)); // CCA
    
    bq34ctl_cc_offset_save();
    bq34ctl_exit_calibration();
}

void bq34ctl_calibrate_board_offset() {
    bq34ctl_enter_calibration();
    do {
        bq34ctl_board_offset();
        OsDelay(1000);
    } while (!(bq34_control_status() & 0x0c00)); // CCA + BCA
    
    do {
        OsDelay(1000);
    } while (!(bq34_control_status() &~ 0x0c00)); // CCA + BCA
    
    bq34ctl_cc_offset_save();
    bq34ctl_exit_calibration();
}