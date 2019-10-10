/***********************************************
*
* LORA BY CATTELECOM
* 920 - 925 MHz (AS923)
* Created on : October 10, 2019
* For CAT StarterKit Gravitech S767
* Created by : if2u v.4
*
***********************************************/

#ifndef catLoRa_h
#define catLoRa_h

#include <Arduino.h>   // required before wiring_private.h
#include <Wire.h>
#include <HTS221.h>
#include "wiring_private.h" // pinPeripheral() function

class catLoRa {
	public:
		catLoRa();
		bool begin(void);
		void ledON(void);
		void ledOFF(void);
		void ledBLINK(void);
		void SERCOM1_Handler(void);
		void setFrequency(void);
		void joinABP(String _CLASS, String _devEUI, String _devADDR);
		void joinOTAA(String _CLASS, String _devEUI, String _appEUI);
		void macCommand(String _Comm);
		bool sendPayload(String _port, String _payLoad);
		String getDL();
		String payloadDL(String _DL);
		String portDL(String _DL);
		String getTemp();
		String getHumi();
		String getTempCATformat();
		String getHumiCATformat();
		String getLPPformat();
	
	private:
		String msg;
		String _Comm;
		String return_msg;
		bool return_bool;
};

#endif
