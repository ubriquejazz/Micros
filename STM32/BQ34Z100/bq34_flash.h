


bool bq34fl_update_design_capacity(int16_t capacity);
bool bq34fl_update_q_max(int16_t capacity)
bool bq34fl_update_design_energy(int16_t energy) 
bool bq34fl_update_cell_charge_voltage_range(uint16_t t1_t2,);
bool bq34fl_update_number_of_series_cells(uint8_t cells);
bool bq34fl_update_pack_configuration(uint16_t config)
bool bq34fl_update_charge_termination_parameters(int16_t taper_current);


void bq34fl_calibrate_cc_offset() 
void bq34fl_calibrate_board_offset()
void bq34fl_calibrate_voltage_divider(uint16_t applied_voltage, );
void bq34fl_calibrate_sense_resistor(int16_t applied_current)