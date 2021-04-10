

#define BQ34Z100_G1_ADDRESS 0xAA

void 	 bq34_unsealed ();
uint16_t bq34_read_register(uint8_t address, uint8_t length);
uint16_t bq34_read_control(uint8_t address_lsb, uint8_t address_msb);
void 	 bq34_read_flash_block(uint8_t sub_class, uint8_t offset);
void 	 bq34_write_reg(uint8_t addr, uint8_t val);
void 	 bq34_write_flash_block(uint8_t sub_class, uint8_t offset);
uint8_t  bq34_flash_block_checksum();
double 	 bq34_xemics_to_double(uint32_t value);
uint32_t bq34_double_to_xemics(double value);
    
