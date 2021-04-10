#include "bq34_flash.h"

uint8_t flash_block_data[32];

bool bq34fl_update_design_capacity(int16_t capacity) {
    bq34_unsealed ();
    bq34_read_flash_block(48, 0);    
    flash_block_data[6] = 0; // Cycle Count
    flash_block_data[7] = 0;
    
    flash_block_data[8] = capacity >> 8; // CC Threshold
    flash_block_data[9] = capacity & 0xff;
    
    flash_block_data[11] = capacity >> 8; // Design Capacity
    flash_block_data[12] = capacity & 0xff;
    
    for (uint8_t i = 6; i <= 9; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    
    for (uint8_t i = 11; i <= 12; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    return bq34_checksum(150);
}

bool bq34fl_update_q_max(int16_t capacity) {
    bq34_unsealed ();
    bq34_read_flash_block(82, 0);
    flash_block_data[0] = capacity >> 8; // Q Max
    flash_block_data[1] = capacity & 0xff;
    
    flash_block_data[2] = 0; // Cycle Count
    flash_block_data[3] = 0;
    
    for (uint8_t i = 0; i <= 3; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    return bq34_checksum(150);
}

bool bq34fl_update_design_energy(int16_t energy) {
    bq34_unsealed ();
    bq34_read_flash_block(48, 0);
    flash_block_data[13] = energy >> 8; // Design Energy
    flash_block_data[14] = energy & 0xff;
    
    for (uint8_t i = 13; i <= 14; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    return bq34_checksum(150);
}

bool bq34fl_update_cell_charge_voltage_range(uint16_t t1_t2, uint16_t t2_t3, uint16_t t3_t4) {
    bq34_unsealed ();
    bq34_read_flash_block(48, 0);    
    flash_block_data[17] = t1_t2 >> 8; // Cell Charge Voltage T1-T2
    flash_block_data[18] = t1_t2 & 0xff;
    
    flash_block_data[19] = t2_t3 >> 8; // Cell Charge Voltage T2-T3
    flash_block_data[20] = t2_t3 & 0xff;
    
    flash_block_data[21] = t3_t4 >> 8; // Cell Charge Voltage T3-T4
    flash_block_data[22] = t3_t4 & 0xff;
    
    for (uint8_t i = 17; i <= 22; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    return bq34_checksum(150);
}

bool bq34fl_update_number_of_series_cells(uint8_t cells) {
    bq34_unsealed ();
    bq34_read_flash_block(64, 0);    
    flash_block_data[7] = cells; // Number of Series Cell
    
    bq34_write_reg(0x40 + 7, flash_block_data[7]);    
    return bq34_checksum(150);
}

bool bq34fl_update_pack_configuration(uint16_t config) {
    bq34_unsealed ();
    read_flash_block(64, 0);    
    flash_block_data[0] = config >> 8; // Pack Configuration
    flash_block_data[1] = config & 0xff;
    
    for (uint8_t i = 0; i <= 1; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    return bq34_checksum(150);
}

bool bq34fl_update_charge_termination_parameters(taper_t taper) 
{
    taper_t updated;
    bq34_unsealed ();
    bq34_read_flash_block(36, 0);
    
    flash_block_data[0] = taper.current >> 8; // Taper Current
    flash_block_data[1] = taper.current & 0xff;
    
    flash_block_data[2] = taper.min_capacity >> 8; // Min Taper Capacity
    flash_block_data[3] = taper.min_capacity & 0xff;
    
    flash_block_data[4] = taper.cell_voltage >> 8; // Cell Taper Voltage
    flash_block_data[5] = taper.cell_voltage & 0xff;
    
    flash_block_data[6] = taper.window; // Current Taper Window
    
    flash_block_data[7] = taper.tca.set & 0xff; // TCA Set %
    flash_block_data[8] = taper.tca.clear & 0xff; // TCA Clear %
    
    flash_block_data[9] = taper.fc.set & 0xff; // FC Set %
    flash_block_data[10] = taper.fc.clear & 0xff; // FC Clear %
    
    for (uint8_t i = 0; i <= 10; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    return bq34_checksum(150);
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
    bq34_read_flash_block(104, 0);
    
    uint16_t current_voltage_divider = flash_block_data[14] << 8;
    current_voltage_divider |= flash_block_data[15];
    
    uint16_t new_voltage_divider = ((double)applied_voltage / volt_mean) * (double)current_voltage_divider;
    
    flash_block_data[14] = new_voltage_divider >> 8;
    flash_block_data[15] = new_voltage_divider & 0xff;
    
    for (uint8_t i = 14; i <= 15; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    bq34_checksum(150);
    
    bq34_unsealed ();
    bq34_read_flash_block(68, 0);
    
    int16_t flash_update_of_cell_voltage = (double)(2800 * cells_count * 5000) / (double)new_voltage_divider;
    
    flash_block_data[0] = flash_update_of_cell_voltage << 8;
    flash_block_data[1] = flash_update_of_cell_voltage & 0xff;
    
    for (uint8_t i = 0; i <= 1; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
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
    bq34_read_flash_block(104, 0);

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
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }

    for (uint8_t i = 4; i <= 7; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    bq34_checksum(150);
}