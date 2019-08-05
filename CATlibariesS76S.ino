#include "catLoRa.h"
catLoRa lora;

String _CLASS = "C";
String _devEUI = "XXXXXXXXXXXXXXXXX";  // 16 digit
String _devADDR = "XXXXXXXX"; // 8 digit
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
     String humi = lora.getHumi();
    
     _payLoad = lora.getLPPformat();
     String _tempHumi = temp+humi;
     
    if(lora.sendPayload(_port,_tempHumi)){
      Serial.println(_tempHumi);
      Serial.println("Send Payload OK");
    }
    
    Serial.println("....................................");
    delay(500);
  }//

  String x = lora.getDL();
  if(x != ""){

      Serial.println("portDL:"+lora.portDL(x));
      Serial.println("payloadDL:"+lora.payloadDL(x));
      
      if(lora.payloadDL(x) == "22") lora.ledON();
      if(lora.payloadDL(x) == "11") lora.ledOFF();
      
  }else{
    Serial.println("NO DATA DL");
  }

}//loop
