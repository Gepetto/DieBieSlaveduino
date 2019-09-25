/*
Basic test to verify the programation of the stm32 
*/

#define ledPin PB15 //STATUS LED


void setup() {
 pinMode(ledPin,OUTPUT);
}

void loop() {
  
  digitalWrite(ledPin, HIGH);   
  delay(1000);                
  digitalWrite(ledPin, LOW);  
  delay(1000);                

}
