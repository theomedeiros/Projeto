#include <ESP8266WiFi.h>
#include "HX711.h"
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#define BLYNK_PRINT Serial
 
const char *ssid = "myssid"; // replace with your wifi ssid and wpa2 key
const char *pass = "mypass";
char auth[] = "uPz1n8QwX06W79L7ybFS1svF7uzaMlt0";    // You should get Auth Token in the Blynk App.

int INA = 0; 
int INB = 2;

WiFiClient client;
 
HX711 scale(14, 12);

float weight;
float calibration_factor = -104345; // for me this vlaue works just perfect 419640

void setup() 
{
  Serial.begin(57600);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading

  { 
  pinMode(INA,OUTPUT); 
  pinMode(INB,OUTPUT); 
  }

  Blynk.begin(auth, ssid, pass);
  
  WiFi.begin(ssid, pass);
  {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  delay(2000);
}
 
void loop() 
 
{
 
  Blynk.run();
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
 
  weight = scale.get_units(5); 

  Blynk.virtualWrite(V1, weight * 1000);
  Blynk.virtualWrite(V2, weight * 1000);
  Blynk.setProperty(V2, "label",  " gramas");
  Blynk.setProperty(V3, "label",  "Despejar Ração");

 
}

BLYNK_WRITE(V3)

{

   if( param.asInt() == 1){

        digitalWrite(INA,LOW);
        digitalWrite(INB,HIGH); 
        delay(1000); 

   }else{

        digitalWrite(INA,0);
        digitalWrite(INB,0); 
        delay(1000); 

   }

}
