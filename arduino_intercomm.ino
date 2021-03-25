//Arduino code
#include <SoftwareSerial.h>
SoftwareSerial s(5,6);

int sensorPin = A0;
float volt;
float ntu;

void setup() {
s.begin(9600);
}
 
void loop() {
  volt=0;
  for(int i=0; i<800; i++)
    {
        volt += ((float)analogRead(sensorPin)/1023)*5;
    }
    volt = volt/800;
    volt = round_to_dp(volt,2);
    Serial.println("Volt: ");
    Serial.println(volt);
    if(volt < 2.5){
      ntu = 3000;
    }else{
      ntu = -1120.4*square(volt)+5742.3*volt-4353.8; 
    }
    Serial.println("NTU: ");
    Serial.println(ntu);
    //int data=50;
    if(s.available()>0)
    {
     s.write(ntu);
    }
}
float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}
