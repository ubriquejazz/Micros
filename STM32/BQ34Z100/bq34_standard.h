#include "bq34z100g1.h"

uint16_t BQ34STD_control_status(); 
uint16_t BQ34STD_device_type();
uint16_t BQ34STD_fw_version(); 
uint16_t BQ34STD_hw_version();
uint16_t BQ34STD_reset_data(); 
uint16_t BQ34STD_prev_macwrite(); 
uint16_t BQ34STD_chem_id(); 
uint16_t BQ34STD_board_offset(); 
uint16_t BQ34STD_cc_offset(); 
uint16_t BQ34STD_cc_offset_save(); 
uint16_t BQ34STD_df_version(); 
uint16_t BQ34STD_set_fullsleep(); 
uint16_t BQ34STD_static_chem_chksum(); 
uint16_t BQ34STD_sealed(); 
uint16_t BQ34STD_it_enable(); 
uint16_t BQ34STD_cal_enable(); 
uint16_t BQ34STD_reset(); 
uint16_t BQ34STD_exit_cal(); 
uint16_t BQ34STD_enter_cal(); 
uint16_t BQ34STD_offset_cal(); 
uint8_t  BQ34STD_state_of_charge(); 
uint8_t  BQ34STD_state_of_charge_max_error();
uint16_t BQ34STD_remaining_capacity(); 
uint16_t BQ34STD_full_charge_capacity(); 
uint16_t BQ34STD_voltage(); 
int16_t  BQ34STD_average_current(); 
uint16_t BQ34STD_temperature(); 
uint16_t BQ34STD_flags(); 
uint16_t BQ34STD_flags_b(); 
int16_t  BQ34STD_current(); 
uint16_t BQ34STD_average_time_to_empty();
uint16_t BQ34STD_average_time_to_full(); 
int16_t  BQ34STD_passed_charge(); 
uint16_t BQ34STD_do_d0_time(); 
uint16_t BQ34STD_available_energy();
uint16_t BQ34STD_average_power(); 
uint16_t BQ34STD_serial_number(); 
uint16_t BQ34STD_internal_temperature(); 
uint16_t BQ34STD_cycle_count(); 
uint16_t BQ34STD_state_of_health(); 
uint16_t BQ34STD_charge_voltage(); 
uint16_t BQ34STD_charge_current(); 
uint16_t BQ34STD_pack_configuration();
uint16_t BQ34STD_design_capacity();
uint8_t  BQ34STD_grid_number();
uint8_t  BQ34STD_learned_status();
uint16_t BQ34STD_dod_at_eoc(); 
uint16_t BQ34STD_q_start(); 
uint16_t BQ34STD_true_fcc(); 
uint16_t BQ34STD_state_time(); ;
uint16_t BQ34STD_q_max_passed_q(); ;
uint16_t BQ34STD_dod_0(); ;
uint16_t BQ34STD_q_max_dod_0();
uint16_t BQ34STD_q_max_time();

static inline bq34_checksum(uint8_t  delay)
{
	bq34_write_reg(0x60, bq34_flash_block_checksum(););
    OsDelay(delay);
    BQ34STD_reset();
    OsDelay(delay);
}

static inline bq34_enter_calibration() {
    bq34_unsealed ();
    do {
        BQ34STD_cal_enable();
        BQ34STD_enter_cal();
        delay(1000);
    } while (!(BQ34STD_control_status() & 0x1000)); // CALEN
}

static inline bq34_exit_calibration() {
    do {
        BQ34STD_exit_cal();
        delay(1000);
    } while (!(BQ34STD_control_status() &~ 0x1000)); // CALEN
    bq34_checksum(150);
}