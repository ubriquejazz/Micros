/*!\name     volatile.c
 *
 * \brief    Blink regular vs volatile Benchmark
 *           Calls a delay function repetedy
 *
 *
 * \author    Juan Gago
 *
 */

#define MEASURE

#define MASK(x) ((unsigned char)(1<<x))

#ifdef MEASURE
uint32_t StartTime, CurrentTime;
#endif

void setup() {                
  DDRB |= MASK(5);   // The LED pin is output. Mask: 00100000 
  Serial.begin(9600);  
}

void wait(){
  uint32_t x=100000;
  while(x)
    x--;
}
  
void loop() {
  PORTB ^= MASK(5);  // Toggle LED 
  
  #ifdef MEASURE
    StartTime = millis();
  #endif
  
  wait();
  wait();
  wait();
  wait();
  wait();

  #ifdef MEASURE
    CurrentTime = millis();
    Serial.print("Took ");
    Serial.print((float)(CurrentTime - StartTime)*0.001,3);
    Serial.print(" seconds\n");
  #endif
}

