/***********************************************
*
* LORA BY CATTELECOM
* 920 - 925 MHz (AS923)
* Created on : April 22, 2019
* For CAT StarterKit Gravitech S767
* Created by : if2u
*
***********************************************/

#include "catLoRa.h"

#define	PIN_RX				11
#define	PIN_TX				10
#define	LED_13				13
#define _NWKSKEY			"28AED22B7E1516A609CFABF715884F3C"
#define _APPSKEY			"1628AE2B7E15D2A6ABF7CF4F3C158809"
#define _APPKEY				"1628AE2B7E15D2A6ABF7CF4F3C158809"

#define _SIP_RESET			"sip reset"
#define _SET_CLASS		"mac set_class"
#define _SET_devEUI		"mac set_deveui"
#define _SET_appEUI		"mac set_appeui"
#define _SET_devADDR	"mac set_devaddr"
#define _SET_NWKSKEY	"mac set_nwkskey"
#define _SET_APPSKEY		"mac set_appskey"
#define _SET_APPKEY		"mac set_appkey"
#define _SET_FREQ			"mac set_ch_freq"
#define _SET_RX2			"mac set_rx2"
#define _SET_ABP			"mac join abp"
#define _SET_OTAA			"mac join otaa"
#define _SET_LORA			"mac save"
#define _SEND_PAYLOAD	"mac tx ucnf"

#define _GET_CLASS		"mac get_class"
#define _GET_devEUI		"mac get_deveui"
#define _GET_devADDR	"mac get_devaddr"
#define _GET_NWKSKEY	"mac get_nwkskey"
#define _GET_APPSKEY 	"mac get_appskey"

Uart Serial2(&sercom1, PIN_RX, PIN_TX, SERCOM_RX_PAD_0, UART_TX_PAD_2);

catLoRa :: catLoRa() {
	pinMode(LED_13, OUTPUT);
}

void catLoRa:: ledON(void){
	  digitalWrite(LED_13, HIGH);
}

void catLoRa:: ledOFF(void){
	  digitalWrite(LED_13, LOW);
}

void catLoRa:: ledBLINK(void){
	  digitalWrite(LED_13, HIGH);
	  delay(1000);
	  digitalWrite(LED_13, LOW);
	  delay(200);
}

bool catLoRa::begin(void) {
	Wire.begin();
	Serial2.begin(115200);
	
	pinPeripheral(PIN_TX, PIO_SERCOM);
	pinPeripheral(PIN_RX, PIO_SERCOM);
	delay(500);	
	
	setFrequency();
}

void SERCOM1_Handler(void){
  Serial2.IrqHandler();
}

void catLoRa:: setFrequency(void){
	
	int freq [] = { 923200000, 923400000, 922000000, 922200000, 922400000, 922600000, 922800000, 923000000 };
	
	macCommand(String(_SIP_RESET));
	delay(1000);
	
	for (int i=0; i<sizeof freq/sizeof freq[0]; i++){
		macCommand(String(_SET_FREQ)+" "+i+" "+freq[i]);
	}
	 macCommand(String(_SET_RX2)+" 2 923200000");
	 macCommand(String(_SET_LORA));

}

void catLoRa:: joinABP(String _CLASS, String _devEUI, String _devADDR){
  
  macCommand(String(_SET_CLASS)+" "+String(_CLASS));
  macCommand(String(_SET_devEUI)+" "+String(_devEUI));
  macCommand(String(_SET_devADDR)+" "+String(_devADDR));
  macCommand(String(_SET_NWKSKEY)+" "+String(_NWKSKEY));
  macCommand(String(_SET_APPSKEY)+" "+String(_APPSKEY));
  macCommand(String(_SET_ABP));
  macCommand(String(_SET_LORA));
  
}

void catLoRa:: joinOTAA(String _CLASS, String _devEUI, String _appEUI){

  macCommand(String(_SET_CLASS)+" "+String(_CLASS));
  macCommand(String(_SET_devEUI)+" "+String(_devEUI));
  macCommand(String(_SET_appEUI)+" "+String(_appEUI));
  macCommand(String(_SET_APPKEY)+" "+String(_APPKEY));
  macCommand(String(_SET_OTAA));
  macCommand(String(_SET_LORA));

}


bool catLoRa:: sendPayload(String _port, String _payLoad){
	
	Serial2.print(String(_SEND_PAYLOAD)+" "+String(_port) + String(" ") + String(_payLoad));
	delay(250);
	
	return_bool = false;
	msg = "";
	msg = Serial2.readString();
	
	String checkRX = ">> Ok";
	int checkStart = msg.indexOf(checkRX);
	if(checkStart != -1) return_bool = true;

return return_bool;
}

void catLoRa:: macCommand(String _Comm){
	Serial2.print(String(_Comm));
	msg = "";
    delay(250);
    msg = Serial2.readString();
    Serial.print(String(_Comm));
    Serial.println(msg);
	delay(100);
}

String catLoRa:: getTemp(){
	
	smeHumidity.begin();
	
	msg = "";
	msg = uint16_t(smeHumidity.readTemperature()*100);
	return msg;
	
}

String catLoRa:: getHumi(){
	
	smeHumidity.begin();
	
	msg = "";
	msg = uint16_t(smeHumidity.readHumidity()*100);
	return msg;
	
}

String catLoRa:: getLPPformat(){
	
	smeHumidity.begin();
	
	String temp = "";
	String humi = "";
	String CayenFormat = "";
	
	String cayen_tmp = "0067";
    String cayen_hum = "0168";
	
	temp = uint16_t(smeHumidity.readTemperature()*10);
	humi = uint16_t(smeHumidity.readHumidity()*2);
	
	char tmp_hex[5];
    char hum_hex[3];
    sprintf(tmp_hex, "%04x", temp.toInt());
    sprintf(hum_hex, "%02x", humi.toInt());
	
	CayenFormat = cayen_tmp + tmp_hex + cayen_hum + hum_hex;
	
	return CayenFormat;
	
}
