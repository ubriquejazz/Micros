/*!\name     types.c
 *
 * \brief     Checking memory usage of data types
 *			  Demo for uint32_t and uint8_t
 *
 *
 * \author    Juan Gago
 *
 */
 
uint32_t x;
uint8_t sensor_data[1000];

void setup(){
}

void loop(){
  sensor_data[x++]=x;
}
