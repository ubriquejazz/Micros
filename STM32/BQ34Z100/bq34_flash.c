#include "bq34_flash.h"



bool bq34fl_update_design_capacity(int16_t capacity) {
    bq34_unsealed ();
    read_flash_block(48, 0);
    
    flash_block_data[6] = 0; // Cycle Count
    flash_block_data[7] = 0;
    
    flash_block_data[8] = capacity >> 8; // CC Threshold
    flash_block_data[9] = capacity & 0xff;
    
    flash_block_data[11] = capacity >> 8; // Design Capacity
    flash_block_data[12] = capacity & 0xff;
    
    for (uint8_t i = 6; i <= 9; i++) {
        write_reg(0x40 + i, flash_block_data[i]);
    }
    
    for (uint8_t i = 11; i <= 12; i++) {
        write_reg(0x40 + i, flash_block_data[i]);
    }
    bq34_checksum(150);
    
    bq34_unsealed ();
    read_flash_block(48, 0);
    int16_t updated_cc_threshold = flash_block_data[8] << 8;
    updated_cc_threshold |= flash_block_data[9];
    
    int16_t updated_capacity = flash_block_data[11] << 8;
    updated_capacity |= flash_block_data[12];
    
    if (flash_block_data[6] != 0 || flash_block_data[7] != 0) {
        return false;
    }
    if (capacity != updated_cc_threshold) {
        return false;
    }
    if (capacity != updated_capacity) {
        return false;
    }
    return true;
}

bool bq34fl_update_q_max(int16_t capacity) {
    bq34_unsealed ();
    read_flash_block(82, 0);
    flash_block_data[0] = capacity >> 8; // Q Max
    flash_block_data[1] = capacity & 0xff;
    
    flash_block_data[2] = 0; // Cycle Count
    flash_block_data[3] = 0;
    
    for (uint8_t i = 0; i <= 3; i++) {
        write_reg(0x40 + i, flash_block_data[i]);
    }
    bq34_checksum(150);
    
    bq34_unsealed ();
    read_flash_block(82, 0);
    int16_t updated_q_max = flash_block_data[0] << 8;
    updated_q_max |= flash_block_data[1];
    
    if (capacity != updated_q_max) {
        return false;
    }
    return true;
}

bool bq34fl_update_design_energy(int16_t energy) {
    bq34_unsealed ();
    read_flash_block(48, 0);
    flash_block_data[13] = energy >> 8; // Design Energy
    flash_block_data[14] = energy & 0xff;
    
    for (uint8_t i = 13; i <= 14; i++) {
        write_reg(0x40 + i, flash_block_data[i]);
    }
    bq34_checksum(150);
    
    bq34_unsealed ();
    read_flash_block(48, 0);
    int16_t updated_energy = flash_block_data[13] << 8;
    updated_energy |= flash_block_data[14];
    
    if (energy != updated_energy) {
        return false;
    }
    return true;
}

bool bq34fl_update_cell_charge_voltage_range(uint16_t t1_t2, uint16_t t2_t3, uint16_t t3_t4) {
    bq34_unsealed ();
    read_flash_block(48, 0);
    
    flash_block_data[17] = t1_t2 >> 8; // Cell Charge Voltage T1-T2
    flash_block_data[18] = t1_t2 & 0xff;
    
    flash_block_data[19] = t2_t3 >> 8; // Cell Charge Voltage T2-T3
    flash_block_data[20] = t2_t3 & 0xff;
    
    flash_block_data[21] = t3_t4 >> 8; // Cell Charge Voltage T3-T4
    flash_block_data[22] = t3_t4 & 0xff;
    
    for (uint8_t i = 17; i <= 22; i++) {
        write_reg(0x40 + i, flash_block_data[i]);
    }
    bq34_checksum(150);
    
    bq34_unsealed ();
    read_flash_block(48, 0);
    uint16_t updated_t1_t2 = flash_block_data[17] << 8;
    updated_t1_t2 |= flash_block_data[18];
    
    uint16_t updated_t2_t3 = flash_block_data[19] << 8;
    updated_t2_t3 |= flash_block_data[20];
    
    uint16_t updated_t3_t4 = flash_block_data[21] << 8;
    updated_t3_t4 |= flash_block_data[22];
    
    if (t1_t2 != updated_t1_t2 || t2_t3 != updated_t2_t3 || t3_t4 != updated_t3_t4) {
        return false;
    }
    return true;
}

bool bq34fl_update_number_of_series_cells(uint8_t cells) {
    bq34_unsealed ();
    read_flash_block(64, 0);
    
    flash_block_data[7] = cells; // Number of Series Cell
    
    write_reg(0x40 + 7, flash_block_data[7]);
    
    bq34_checksum(150);
    
    bq34_unsealed ();
    read_flash_block(64, 0);
    
    if (cells != flash_block_data[7]) {
        return false;
    }
    return true;
}

bool bq34fl_update_pack_configuration(uint16_t config) {
    bq34_unsealed ();
    read_flash_block(64, 0);
    
    flash_block_data[0] = config >> 8; // Pack Configuration
    flash_block_data[1] = config & 0xff;
    
    for (uint8_t i = 0; i <= 1; i++) {
        write_reg(0x40 + i, flash_block_data[i]);
    }
    bq34_checksum(150);
    
    bq34_unsealed ();
    read_flash_block(64, 0);
    uint16_t updated_config = flash_block_data[0] << 8;
    updated_config |= flash_block_data[1];
    if (config != updated_config) {
        return false;
    }
    return true;
}

bool bq34fl_update_charge_termination_parameters(int16_t taper_current, int16_t min_taper_capacity, int16_t cell_taper_voltage, uint8_t taper_window, int8_t tca_set, int8_t tca_clear, int8_t fc_set, int8_t fc_clear) {
    bq34_unsealed ();
    read_flash_block(36, 0);
    
    flash_block_data[0] = taper_current >> 8; // Taper Current
    flash_block_data[1] = taper_current & 0xff;
    
    flash_block_data[2] = min_taper_capacity >> 8; // Min Taper Capacity
    flash_block_data[3] = min_taper_capacity & 0xff;
    
    flash_block_data[4] = cell_taper_voltage >> 8; // Cell Taper Voltage
    flash_block_data[5] = cell_taper_voltage & 0xff;
    
    flash_block_data[6] = taper_window; // Current Taper Window
    
    flash_block_data[7] = tca_set & 0xff; // TCA Set %
    
    flash_block_data[8] = tca_clear & 0xff; // TCA Clear %
    
    flash_block_data[9] = fc_set & 0xff; // FC Set %
    
    flash_block_data[10] = fc_clear & 0xff; // FC Clear %
    
    for (uint8_t i = 0; i <= 10; i++) {
        write_reg(0x40 + i, flash_block_data[i]);
    }
    bq34_checksum(150);
    
    bq34_unsealed ();
    read_flash_block(36, 0);
    int16_t updated_taper_current, updated_min_taper_capacity, updated_cell_taper_voltage;
    uint8_t updated_taper_window;
    int8_t updated_tca_set, updated_tca_clear, updated_fc_set, updated_fc_clear;
    
    updated_taper_current = flash_block_data[0] << 8;
    updated_taper_current |= flash_block_data[1];
    
    updated_min_taper_capacity = flash_block_data[2] << 8;
    updated_min_taper_capacity |= flash_block_data[3];
    
    updated_cell_taper_voltage = flash_block_data[4] << 8;
    updated_cell_taper_voltage |= flash_block_data[5];
    
    updated_taper_window = flash_block_data[6];
    
    updated_tca_set = flash_block_data[7] & 0xff;
    
    updated_tca_clear = flash_block_data[8] & 0xff;
    
    updated_fc_set = flash_block_data[9] & 0xff;
    
    updated_fc_clear = flash_block_data[10] & 0xff;
    
    if (taper_current != updated_taper_current) {
        return false;
    }
    if (min_taper_capacity != updated_min_taper_capacity) {
        return false;
    }
    if (cell_taper_voltage != updated_cell_taper_voltage) {
        return false;
    }
    if (taper_window != updated_taper_window) {
        return false;
    }
    if (tca_set != updated_tca_set) {
        return false;
    }
    if (tca_clear != updated_tca_clear) {
        return false;
    }
    if (fc_set != updated_fc_set) {
        return false;
    }
    if (fc_clear != updated_fc_clear) {
        return false;
    }
    return true;
}

void bq34fl_calibrate_cc_offset() {
    enter_calibration();
    do {
        cc_offset();
        delay(1000);
    } while (!(control_status() & 0x0800)); // CCA
    
    do {
        delay(1000);
    } while (!(control_status() &~ 0x0800)); // CCA
    
    cc_offset_save();
    exit_calibration();
}

void bq34fl_calibrate_board_offset() {
    enter_calibration();
    do {
        board_offset();
        delay(1000);
    } while (!(control_status() & 0x0c00)); // CCA + BCA
    
    do {
        delay(1000);
    } while (!(control_status() &~ 0x0c00)); // CCA + BCA
    
    cc_offset_save();
    exit_calibration();
}

void bq34fl_calibrate_voltage_divider(uint16_t applied_voltage, uint8_t cells_count) {
    double volt_array[50];
    for (uint8_t i = 0; i < 50; i++) {
        volt_array[i] = voltage();
        delay(150);
    }
    double volt_mean = 0;
    for (uint8_t i = 0; i < 50; i++) {
        volt_mean += volt_array[i];
    }
    volt_mean /= 50.0;
    
    double volt_sd = 0;
    for (uint8_t i = 0; i < 50; i++) {
        volt_sd += pow(volt_array[i] - volt_mean, 2);
    }
    volt_sd /= 50.0;
    volt_sd = sqrt(volt_sd);
    
    if (volt_sd > 100) {
        return;
    }

    bq34_unsealed ();
    read_flash_block(104, 0);
    
    uint16_t current_voltage_divider = flash_block_data[14] << 8;
    current_voltage_divider |= flash_block_data[15];
    
    uint16_t new_voltage_divider = ((double)applied_voltage / volt_mean) * (double)current_voltage_divider;
    
    flash_block_data[14] = new_voltage_divider >> 8;
    flash_block_data[15] = new_voltage_divider & 0xff;
    
    for (uint8_t i = 14; i <= 15; i++) {
        write_reg(0x40 + i, flash_block_data[i]);
    }
    
    write_reg(0x60, flash_block_checksum());
    delay(150);
    
    bq34_unsealed ();
    read_flash_block(68, 0);
    
    int16_t flash_update_of_cell_voltage = (double)(2800 * cells_count * 5000) / (double)new_voltage_divider;
    
    flash_block_data[0] = flash_update_of_cell_voltage << 8;
    flash_block_data[1] = flash_update_of_cell_voltage & 0xff;
    
    for (uint8_t i = 0; i <= 1; i++) {
        write_reg(0x40 + i, flash_block_data[i]);
    }
    bq34_checksum(150);
}

void bq34fl_calibrate_sense_resistor(int16_t applied_current) {
    double current_array[50];
    for (uint8_t i = 0; i < 50; i++) {
        current_array[i] = current();
        delay(150);
    }
    double current_mean = 0;
    for (uint8_t i = 0; i < 50; i++) {
        current_mean += current_array[i];
    }
    current_mean /= 50.0;

    double current_sd = 0;
    for (uint8_t i = 0; i < 50; i++) {
        current_sd += pow(current_array[i] - current_mean, 2);
    }
    current_sd /= 50.0;
    current_sd = sqrt(current_sd);

    if (current_sd > 100) {
        return;
    }

    bq34_unsealed ();
    read_flash_block(104, 0);

    uint32_t cc_gain = flash_block_data[0] << 24;
    cc_gain |= flash_block_data[1] << 16;
    cc_gain |= flash_block_data[2] << 8;
    cc_gain |= flash_block_data[3];
    
    double gain_resistence = 4.768 / xemics_to_double(cc_gain);

    double temp = (current_mean * gain_resistence) / (double)applied_current;

    uint32_t new_cc_gain = double_to_xemics(4.768 / temp);
    flash_block_data[0] = new_cc_gain >> 24;
    flash_block_data[1] = new_cc_gain >> 16;
    flash_block_data[2] = new_cc_gain >> 8;
    flash_block_data[3] = new_cc_gain & 0xff;

    new_cc_gain = double_to_xemics(5677445.6 / temp);
    flash_block_data[4] = new_cc_gain >> 24;
    flash_block_data[5] = new_cc_gain >> 16;
    flash_block_data[6] = new_cc_gain >> 8;
    flash_block_data[7] = new_cc_gain & 0xff;
    

    for (uint8_t i = 0; i <= 3; i++) {
        write_reg(0x40 + i, flash_block_data[i]);
    }

    for (uint8_t i = 4; i <= 7; i++) {
        write_reg(0x40 + i, flash_block_data[i]);
    }
    bq34_checksum(150);
}