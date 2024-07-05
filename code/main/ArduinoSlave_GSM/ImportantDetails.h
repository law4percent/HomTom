#ifndef ImportantDetails
#define ImportantDetails

#include <Arduino.h>

const char Separator = '_';
const char* myPasscode = "1234";

const char* TrustedRecipient1 = "09362348514";
const char* TrustedRecipient2 = "09062404856";
const char* TrustedRecipient3 = "09";

const String myGSMCommands[11] = {
  "sB",  "r1B", "r2B", "cB",  "sF",
  "r1F", "r2F", "*B", "*F", "*A",
  "st"
};

const char myGSMStateCommands[5] = {
  '1', '0'
};

const String TrustedRecipCode[3] = {
  "r1", "r2", "r3"
};

const String emergencyMSG1 = "Alert: There is a fire in the sala room!";
const String emergencyMSG2 = "Alert: Fire in the room!";
#endif