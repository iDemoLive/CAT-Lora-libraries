#include "catLoRa.h"
catLoRa lora;

String _CLASS = "C";
String _devEUI = "XXXXXXXXXXXXXXXXX"; 	// 16digit
String _devADDR = "XXXXXXXX";	//8 digit
String _port = "6";
String _payLoad = "";

uint32_t   Interval_Time_Old;
uint16_t   Interval_Time;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Interval_Time = 30000;
  lora.begin();
  lora.joinABP(_CLASS, _devEUI, _devADDR);
}//setup

void loop() {
  // put your main code here, to run repeatedly:
  lora.ledBLINK();
  
  // Check Time for send payload
  if (millis() - Interval_Time_Old >= Interval_Time){
    Interval_Time_Old = millis();

     String temp = lora.getTemp();
     Serial.println(temp);

     String humi = lora.getHumi();
     Serial.println(humi);

     _payLoad = lora.getLPPformat();
     
    if(lora.sendPayload(_port,_payLoad)){
      Serial.println(_payLoad);
      Serial.println("Send Payload OK");
    }
    
    Serial.println("....................................");
    delay(500);
  }//

}//loop
