#include <SoftwareSerial.h>
#include <Wire.h>

#include "ImportantDetails.h"

#define RELAY_NO
// #define RELAY_NC

// #define Testing_Relays  // Uncomment this line for debugging
// #define Serial_Debug  // Uncomment this line for debugging
// #define Serial_Debug_Master_Slave  // Uncomment this line for debugging

#ifdef RELAY_NO
#define ON 1
#define OFF 0
const char* Mode = "Normally Open (ON == 1)";
#elif defined(RELAY_NC)
#define ON 0
#define OFF 1
const char* Mode = "Normally Close (ON == 0)";
#endif

// Pin used
const byte relay1 = 2;
const byte relay2 = 3;
const byte relay3 = 4;
const byte relay4 = 5;
const byte relay5 = 6;
const byte relay6 = 7;
const byte relay7 = 8;
// const byte RX = 12;
// const byte TX = 11;
const byte LightIndicator = 13;

bool relay1State = OFF;
bool relay2State = OFF;
bool relay3State = OFF;
bool relay4State = OFF;
bool relay5State = OFF;
bool relay6State = OFF;
bool relay7State = OFF;
bool GSMstatus = true;  // true means GSM is Okay
bool LedInd = 0;
unsigned long previousMillis = 0;
const unsigned long interval = 1000;

char MasterCommand = '_';
SoftwareSerial SIM900(9, 10);  // RX, TX pins

void setup() {
  Serial.begin(115200);
  SIM900.begin(115200);
  delay(1000);

  Serial.println("Initializing SIM900A...");
  SIM900.println("AT");
  delay(1000);

  if (SIM900.find("OK")) {
    SIM900.println("AT+CMGF=1");  // Set SMS mode to text mode
    delay(1000);
    SIM900.println("AT+CNMI=2,2,0,0,0");  // Enable SMS text mode
    delay(3000);
    SIM900.println("AT+CMGD=1,4");  // delete all SMS
    delay(3000);
    Serial.println(F("SIM900A is ready."));
  } else {
    Serial.println(F("SIM900A initialization failed. Check connections and restart the board."));
    GSMstatus = false;
  }

  Wire.begin(8);
  Wire.onReceive(receiveEvent);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  pinMode(relay7, OUTPUT);
  pinMode(LightIndicator, OUTPUT);

  // Initialize
  digitalWrite(LightIndicator, 0);
  digitalWrite(relay1, OFF);
  digitalWrite(relay2, OFF);
  digitalWrite(relay3, OFF);
  digitalWrite(relay4, OFF);
  digitalWrite(relay5, OFF);
  digitalWrite(relay6, OFF);
  digitalWrite(relay7, OFF);

#ifdef Testing_Relays
  Serial.println(F("=== Relays Testing Mode ===\n"));

  digitalWrite(relay1, ON);
  delay(2000);
  digitalWrite(relay2, ON);
  delay(2000);
  digitalWrite(relay3, ON);
  delay(2000);
  digitalWrite(relay4, ON);
  delay(2000);
  digitalWrite(relay5, ON);
  delay(2000);
  digitalWrite(relay6, ON);
  delay(2000);
  digitalWrite(relay7, ON);
  delay(2000);

  digitalWrite(relay1, OFF);
  delay(2000);
  digitalWrite(relay2, OFF);
  delay(2000);
  digitalWrite(relay3, OFF);
  delay(2000);
  digitalWrite(relay4, OFF);
  delay(2000);
  digitalWrite(relay5, OFF);
  delay(2000);
  digitalWrite(relay6, OFF);
  delay(2000);
  digitalWrite(relay7, OFF);
  delay(2000);

  while (1)
    ;
#endif
}

void loop() {

  while (SIM900.available() > 0) {
    String incoming = SIM900.readString();
    String getPassword = WordSplitter(incoming, '%', 1);
    String getCommand = WordSplitter(incoming, '%', 2);
    String getState = WordSplitter(incoming, '%', 3);
    
    Serial.println("RECEIVED: " + incoming);
    Serial.println("Password: " + getPassword);
    Serial.println("Command: " + getCommand);
    Serial.println("State: " + getState);

    if (getPassword.equals(myPasscode)) {
      Serial.println("PASSCODE OK");
      if (getCommand.equals(myGSMCommands[0])) {
        Serial.println("COMMAND OK");
        CheckState(1, getState);
      } else if (getCommand.equals(myGSMCommands[1])) {
        CheckState(2, getState);
      } else if (getCommand.equals(myGSMCommands[2])) {
        CheckState(3, getState);
      } else if (getCommand.equals(myGSMCommands[3])) {
        CheckState(4, getState);
      } else if (getCommand.equals(myGSMCommands[4])) {
        CheckState(5, getState);
      } else if (getCommand.equals(myGSMCommands[5])) {
        CheckState(6, getState);
      } else if (getCommand.equals(myGSMCommands[6])) {
        CheckState(7, getState);
      } else if (getCommand.equals(myGSMCommands[7])) {
        AllBulb(getState);
      } else if (getCommand.equals(myGSMCommands[8])) {
        AllFan(getState);
      } else if (getCommand.equals(myGSMCommands[9])) {
        AllAppliances(getState);
      } else if (getCommand.equals(myGSMCommands[10])) {
        String msg = Message();

        if (getState.equals(TrustedRecipCode[0])) {
          sendSMS(TrustedRecipient1, msg);
        } else if (getState.equals(TrustedRecipCode[1])) {
          sendSMS(TrustedRecipient2, msg);
        } else if (getState.equals(TrustedRecipCode[2])) {
          sendSMS(TrustedRecipient3, msg);
        }
      }
    }
    UpdateAppliancesStates();
  }


#ifdef Serial_Debug
  Serial.print(F("GSM status: "));
  Serial.println((GSMstatus) ? "Okay" : "Not Okay");
  Serial.print(F("Relay Mode: "));
  Serial.println(Mode);
  Serial.println(F("Relay_Channel | State"));
  Serial.println("     1        |   " + String(relay1State));
  Serial.println("     2        |   " + String(relay2State));
  Serial.println("     3        |   " + String(relay3State));
  Serial.println("     4        |   " + String(relay4State));
  Serial.println("     5        |   " + String(relay5State));
  Serial.println("     6        |   " + String(relay6State));
  Serial.println("     7        |   " + String(relay7State));
  Serial.println(F("\n-------------------------------------\n"));
#endif

  delay(20);

  // If the LED13 of the arduino is constantly turning on and off, this means the GSM is need to fix
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    if (!GSMstatus) {
      if (LedInd) LedInd = 0;
      else LedInd = 1;
      digitalWrite(LightIndicator, LedInd);
    }
    previousMillis = currentMillis;
  }
}

void receiveEvent(int numBytes) {
  if (Wire.available() > 0) {
    MasterCommand = Wire.read();
    Serial.println(MasterCommand);

    if (MasterCommand != 'L' and MasterCommand != 'M') {
      switch (MasterCommand) {
        case 'a':
          relay1State = OFF;
          break;
        case 'b':
          relay2State = OFF;
          break;
        case 'c':
          relay3State = OFF;
          break;
        case 'd':
          relay4State = OFF;
          break;
        case 'e':
          relay5State = OFF;
          break;
        case 'f':
          relay6State = OFF;
          break;
        case 'g':
          relay7State = OFF;
          break;

        case 'h':
          relay1State = OFF;
          relay2State = OFF;
          relay3State = OFF;
          relay4State = OFF;
          relay5State = OFF;
          relay6State = OFF;
          relay7State = OFF;
          break;

        case 'i':
          relay1State = OFF;
          relay2State = OFF;
          relay3State = OFF;
          relay4State = OFF;
          break;

        case 'j':
          relay5State = OFF;
          relay6State = OFF;
          relay7State = OFF;
          break;

        case 'A':
          relay1State = ON;
          break;
        case 'B':
          relay2State = ON;
          break;
        case 'C':
          relay3State = ON;
          break;
        case 'D':
          relay4State = ON;
          break;
        case 'E':
          relay5State = ON;
          break;
        case 'F':
          relay6State = ON;
          break;
        case 'G':
          relay7State = ON;
          break;

        case 'I':
          relay1State = ON;
          relay2State = ON;
          relay3State = ON;
          relay4State = ON;
          break;

        case 'J':
          relay5State = ON;
          relay6State = ON;
          relay7State = ON;
          break;
      }
      UpdateAppliancesStates();
    } else {
      if (MasterCommand == 'L') {
        sendSMS(TrustedRecipient1, emergencyMSG1);
        sendSMS(TrustedRecipient2, emergencyMSG1);
        sendSMS(TrustedRecipient3, emergencyMSG1);
      } else if (MasterCommand == 'M') {
        sendSMS(TrustedRecipient1, emergencyMSG2);
        sendSMS(TrustedRecipient2, emergencyMSG2);
        sendSMS(TrustedRecipient3, emergencyMSG2);
      }
    }
  }
}

void AllAppliances(String state) {
  bool relayState;

  if (myGSMStateCommands[0] == 1) {
    relayState = ON;
  } else if (myGSMStateCommands[1] == 0)  {
    relayState = OFF;
  }

  relay1State = relayState;
  relay2State = relayState;
  relay3State = relayState;
  relay4State = relayState;
  relay5State = relayState;
  relay6State = relayState;
  relay7State = relayState;
}

void AllFan(String state) {
  bool relayState;

  if (state.equals(myGSMStateCommands[0])) {
    relayState = ON;
  } else if (state.equals(myGSMStateCommands[1])) {
    relayState = OFF;
  }

  relay5State = relayState;
  relay6State = relayState;
  relay7State = relayState;
}

void AllBulb(String state) {
  bool relayState;

  if (state.equals(myGSMStateCommands[0])) {
    relayState = ON;
  } else if (state.equals(myGSMStateCommands[1])) {
    relayState = OFF;
  }

  relay1State = relayState;
  relay2State = relayState;
  relay3State = relayState;
  relay4State = relayState;
}

void CheckState(const byte relayChannel, String state) {
  bool relayState;

  if (state.equals(myGSMStateCommands[0])) {
    relayState = ON;
  } else if (state.equals(myGSMStateCommands[1])) {
    relayState = OFF;
  }

  switch (relayChannel) {
    case 1:
      Serial.println("STATUS OK");
      relay1State = relayState;
      break;

    case 2:
      relay2State = relayState;
      break;

    case 3:
      relay3State = relayState;
      break;

    case 4:
      relay4State = relayState;
      break;

    case 5:
      relay5State = relayState;
      break;

    case 6:
      relay6State = relayState;
      break;

    case 7:
      relay7State = relayState;
      break;
  }
}

void UpdateAppliancesStates() {
  digitalWrite(relay1, relay1State);
  digitalWrite(relay2, relay2State);
  digitalWrite(relay3, relay3State);
  digitalWrite(relay4, relay4State);
  digitalWrite(relay5, relay5State);
  digitalWrite(relay6, relay6State);
  digitalWrite(relay7, relay7State);
}

String WordSplitter(String data, char separator, int index) {
    int cntSeparator = 0;
    String getData = "";

    for (int i = 0; i < data.length(); i++) {
        if (data[i] == separator) {
            cntSeparator++;
        } else if (cntSeparator == index) {
            getData += data[i];
        }
    }

    return getData;
}

String Message() {
  String msg = "";

  msg += "Sala Bulb: " + String(relay1State) + "\n";
  msg += "Room1: " + String(relay2State) + "\n";
  msg += "Room2: " + String(relay3State) + "\n";
  msg += "Cr Bulb: " + String(relay4State) + "\n";
  msg += "Sala Fan: " + String(relay5State) + "\n";
  msg += "Sala Fan: " + String(relay6State) + "\n";
  msg += "Sala Fan: " + String(relay7State) + "\n";

  return msg;
}

void sendSMS(const char* phoneNumber, const String message) {
  digitalWrite(LightIndicator, 1);

  // Set SMS mode to text mode
  SIM900.println("AT+CMGF=1");
  delay(3000);

  // Specify the recipient phone number
  SIM900.print("AT+CMGS=\"");
  SIM900.print(phoneNumber);
  SIM900.println("\"");
  delay(3000);

  // Send the SMS message
  SIM900.print(message);
  delay(5000);
  SIM900.println((char)26);  // ASCII code for Ctrl+Z
  delay(3000);

  // Print the SMS status
  if (SIM900.find("OK")) {
    Serial.println(F("SMS sent successfully."));
  } else {
    Serial.println(F("Error sending SMS."));
  }

  digitalWrite(LightIndicator, 0);
}