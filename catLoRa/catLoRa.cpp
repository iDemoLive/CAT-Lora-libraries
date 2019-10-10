/***********************************************
*
* LORA BY CATTELECOM
* 920 - 925 MHz (AS923)
* Created on : August 04, 2019
* For CAT StarterKit Gravitech S767
* Created by : if2u v.3
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
#define _SIP_getModel		"sip get_hw_model_ver"
#define _SIP_getDevEUI	"sip get_hw_deveui"
#define _SIP_getUUID		"sip get_uuid"

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

//********************************SET FREQUENCY**************************
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

//********************************JOIN ABP**************************
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

//********************************SEND PAYLOAD**************************
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

//********************************GET DOWN LINK**************************
String catLoRa:: getDL(){
	
	msg = "";
	msg = Serial2.readString();
	String macComm = "";
	
	String checkDL = ">> mac rx ";
	int checkStart = msg.indexOf(checkDL);
	if(checkStart != -1){
		int checkEND = msg.indexOf("\n", checkStart+1);
        macComm = msg.substring(checkStart+10, checkEND); 		
	}
	return macComm;
}

//********************************GET PORT DOWN LINK**************************
String catLoRa:: portDL(String _DL){
	
	String macComm = "";

	if(_DL != ""){
    int checkStart = _DL.indexOf(' ');
		if(checkStart > -1){
		  macComm = _DL.substring(0, checkStart);
		}
   }else{
		 macComm = "NO PORT DL";
   }
	return macComm;
}

//********************************GET PAYLOAD DOWN LINK**************************
String catLoRa:: payloadDL(String _DL){
	
	String macComm = "";
	
	if(_DL != ""){
    int checkStart = _DL.indexOf(' ');
		if(checkStart > -1){
		  int checkEND = _DL.indexOf("\n", checkStart+1);
		  macComm = _DL.substring(checkStart+1, checkEND); 
		}
   }else{
		 macComm = "NO PAYLOAD DL";
   }
	return macComm;
}

//********************************MAC Command**************************
void catLoRa:: macCommand(String _Comm){
	Serial2.print(String(_Comm));
	msg = "";
    delay(250);
    msg = Serial2.readString();
    Serial.print(String(_Comm));
    Serial.println(msg);
	delay(100);
}

//********************************GET Temperature**************************
String catLoRa:: getTemp(){
	
	smeHumidity.begin();
	
	msg = "";
	msg = uint16_t(smeHumidity.readTemperature()*100);
	return msg;
}

//********************************GET Humidity**************************
String catLoRa:: getHumi(){
	
	smeHumidity.begin();
	
	msg = "";
	msg = uint16_t(smeHumidity.readHumidity()*100);
	return msg;
}

//********************************GET Temp CAT**************************
String catLoRa:: getTempCATformat(){
	
	smeHumidity.begin();
	
	msg = "";
	// 03 Signed Short
	msg = String(uint16_t(smeHumidity.readTemperature()*100), HEX);
	if (msg.length() % 2 != 0) msg = String("0" + msg);
	msg = String("03" + msg);
	return msg;
}

//********************************GET Humi CAT**************************
String catLoRa:: getHumiCATformat(){
	
	smeHumidity.begin();
	
	msg = "";
	// 04 Unsigned Short
	msg = String(uint16_t(smeHumidity.readHumidity()*100), HEX);
	if (msg.length() % 2 != 0) String("0" + msg);
	msg = String("04" + msg);
	return msg;
}

//********************************GET Temp&Humi Cayenne Low Power Payload Format**************************
String catLoRa:: getLPPformat(){
	
	smeHumidity.begin();
	
	String temp = "";
	String humi = "";
	String CayenFormat = "";
	
	String cayen_tmp = "0067"; //3digi (2byte) *0.1(10)
    String cayen_hum = "0168"; //2digi (1byte) *0.5(2)
	
	temp = uint16_t(smeHumidity.readTemperature()*10);
	humi = uint16_t(smeHumidity.readHumidity()*2);
	
	char tmp_hex[5];
    char hum_hex[3];
    sprintf(tmp_hex, "%04x", temp.toInt());
    sprintf(hum_hex, "%02x", humi.toInt());
	
	CayenFormat = cayen_tmp + tmp_hex + cayen_hum + hum_hex;
	
	return CayenFormat;
}
