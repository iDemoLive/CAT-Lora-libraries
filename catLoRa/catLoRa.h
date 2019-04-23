/***********************************************
*
* LORA BY CATTELECOM
* 920 - 925 MHz (AS923)
* Created on : April 22, 2019
* For CAT StarterKit Gravitech S767
* Created by : if2u
*
***********************************************/

#ifndef catLoRa_h
#define catLoRa_h

#include <Arduino.h>
#include <Wire.h>
#include <HTS221.h>
#include "wiring_private.h"

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
		String getTemp();
		String getHumi();
		String getLPPformat();
	
	private:
		String msg;
		String _Comm;
		String return_msg;
		bool return_bool;
};

#endif