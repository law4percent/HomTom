#include <Wire.h>
#include <SoftwareSerial.h>

#include "Command.h"

// #define RELAY_NO
#define RELAY_NC

#ifdef RELAY_NO
  #define ON 1
  #define OFF 0
#elif defined(RELAY_NC)
  #define ON 0
  #define OFF 1
#endif

/* *************** Pin Used ****************** */
  const byte relayPins[] = {
    2, 3, 4, 5, 6, 7, 8
  };
  const byte MQ2sPins[] = {9, 10};
  const byte RX = 11;
  const byte TX = 12;
/* ******************************************* */
SoftwareSerial BTserial(RX, TX);

/* *********** Global Declaration ************ */
  const byte relaysLength = 7;
  const byte mq2sLength = 2;
  unsigned long previousMillis = 0;
  const long timeInterval = 1000;

  byte currRelay1State = OFF;
  byte currRelay2State = OFF;
  byte currRelay3State = OFF;
  byte currRelay4State = OFF;
  byte currRelay5State = OFF;
  byte currRelay6State = OFF;
  byte currRelay7State = OFF;

  byte prevRelay1Data = currRelay1State;
  byte prevRelay2Data = currRelay2State;
  byte prevRelay3Data = currRelay3State;
  byte prevRelay4Data = currRelay4State;
  byte prevRelay5Data = currRelay5State;
  byte prevRelay6Data = currRelay6State;
  byte prevRelay7Data = currRelay7State;
  bool SwitchState = false;
/* ******************************************* */

void setup() {
  Wire.begin();
  Serial.begin(9600);
  BTserial.begin(9600);

  for (int i = 0; i < relaysLength; i++)
    pinMode(relayPins[i], INPUT_PULLUP);
  
  for (int i = 0; i < mq2sLength; i++)
    pinMode(MQ2sPins[i], INPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // Check Switch States every 1 secs
  if (SwitchState and currentMillis - previousMillis >= timeInterval) {
    previousMillis = currentMillis;

    bool MQ2_Astate = digitalRead(MQ2sPins[0]);
    bool MQ2_Bstate = digitalRead(MQ2sPins[1]);

    if (MQ2_Astate) SendUpdateToSlave(MQ2signal[0]);
    if (MQ2_Bstate) SendUpdateToSlave(MQ2signal[1]);

    currRelay1State = !digitalRead(relayPins[0]);
    currRelay2State = !digitalRead(relayPins[1]);
    currRelay3State = !digitalRead(relayPins[2]);
    currRelay4State = !digitalRead(relayPins[3]);
    currRelay5State = !digitalRead(relayPins[4]);
    currRelay6State = !digitalRead(relayPins[5]);
    currRelay7State = !digitalRead(relayPins[6]);
    
    currRelay1State = ConvertValue(currRelay1State);
    currRelay2State = ConvertValue(currRelay2State); 
    currRelay3State = ConvertValue(currRelay3State);
    currRelay4State = ConvertValue(currRelay4State);
    currRelay5State = ConvertValue(currRelay5State);
    currRelay6State = ConvertValue(currRelay6State);
    currRelay7State = ConvertValue(currRelay7State);
    
    if (currRelay1State != prevRelay1Data) {
      prevRelay1Data = currRelay1State;
      if (currRelay1State == ON) {
        SendUpdateToSlave(ONcommand[0]);
      } else {
        SendUpdateToSlave(OFFcommand[0]);
      }
    }

    if (currRelay2State != prevRelay2Data) {
      prevRelay2Data = currRelay2State;
      if (currRelay2State == ON) {
       SendUpdateToSlave(ONcommand[1]);
      } else {
        SendUpdateToSlave(OFFcommand[1]);
      }
    }

    if (currRelay3State != prevRelay3Data) {
      prevRelay3Data = currRelay3State;
      if (currRelay3State == ON) {
        SendUpdateToSlave(ONcommand[2]);
      } else {
        SendUpdateToSlave(OFFcommand[2]);
      }
    }

    if (currRelay4State != prevRelay4Data) {
      prevRelay4Data = currRelay4State;
      if (currRelay4State == ON) {
        SendUpdateToSlave(ONcommand[3]);
      } else {
        SendUpdateToSlave(OFFcommand[3]);
      }
    }

    if (currRelay5State != prevRelay5Data) {
      prevRelay5Data = currRelay5State;
      if (currRelay5State == ON) {
        SendUpdateToSlave(ONcommand[4]);
      } else {
        SendUpdateToSlave(OFFcommand[4]);
      }
    }

    if (currRelay6State != prevRelay6Data) {
      prevRelay6Data = currRelay6State;
      if (currRelay6State == ON) {
        SendUpdateToSlave(ONcommand[5]);
      } else {
        SendUpdateToSlave(OFFcommand[5]);
      }
    }

    if (currRelay7State != prevRelay7Data) {
      prevRelay7Data = currRelay7State;
      if (currRelay7State == ON) {
        SendUpdateToSlave(ONcommand[6]);
      } else {
        SendUpdateToSlave(OFFcommand[6]);
      }
    }
  }

  // Serial Bluetooth Communication
  if (BTserial.available() > 0) {
    char BTreceivedData = char(BTserial.read());

    if (BTreceivedData != '\n' and BTreceivedData != '\r') {
      SendUpdateToSlave(BTreceivedData);

      if (BTreceivedData == 'k') {
        SwitchState = false;
      } else if (BTreceivedData == 'K') {
        SwitchState = true;
      }

      if (SwitchState) {
        switch(BTreceivedData) {
          case 'a':
            prevRelay1Data = OFF;
            break;
          case 'b':
            prevRelay2Data = OFF;
            break;
          case 'c':
            prevRelay3Data = OFF;
            break;
          case 'd':
            prevRelay4Data = OFF;
            break;
          case 'e':
            prevRelay5Data = OFF;
            break;
          case 'f':
            prevRelay6Data = OFF;
            break;
          case 'g':
            prevRelay7Data = OFF;
            break;
          case 'h':
            prevRelay1Data = OFF;
            prevRelay2Data = OFF;
            prevRelay3Data = OFF;
            prevRelay4Data = OFF;
            prevRelay5Data = OFF;
            prevRelay6Data = OFF;
            prevRelay7Data = OFF;
            break;
          case 'i':
            prevRelay1Data = OFF;
            prevRelay2Data = OFF;
            prevRelay3Data = OFF;
            prevRelay4Data = OFF;
            break;
          case 'j':
            prevRelay5Data = OFF;
            prevRelay6Data = OFF;
            prevRelay7Data = OFF;
            break;

          case 'A':
            prevRelay1Data = ON;
            break;
          case 'B':
            prevRelay2Data = ON;
            break;
          case 'C':
            prevRelay3Data = ON;
            break;
          case 'D':
            prevRelay4Data = ON;
            break;
          case 'E':
            prevRelay5Data = ON;
            break;
          case 'F':
            prevRelay6Data = ON;
            break;
          case 'G':
            prevRelay7Data = ON;
            break;
          /*
            case ONcommand[7]:
              prevRelay1Data = ON;
              prevRelay2Data = ON;
              prevRelay3Data = ON;
              prevRelay4Data = ON;
              prevRelay5Data = ON;
              prevRelay6Data = ON;
              prevRelay7Data = ON;
              break;
          */
          case 'I':
            prevRelay1Data = ON;
            prevRelay2Data = ON;
            prevRelay3Data = ON;
            prevRelay4Data = ON;
            break;
          case 'J':
            prevRelay5Data = ON;
            prevRelay6Data = ON;
            prevRelay7Data = ON;
            break;
          
        }
      }
    }
  }
}

void SendUpdateToSlave(const char command) {
  Serial.print("Send to slave: ");
  Serial.println(command);
  Wire.beginTransmission(8);
    Wire.write(command);
  Wire.endTransmission();
}

bool ConvertValue(bool state) {
  if (state) {
    return ON;
  } else {
    return OFF;
  }
}