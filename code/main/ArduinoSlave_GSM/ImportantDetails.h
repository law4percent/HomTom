#ifndef ImportantDetails
#define ImportantDetails

#include <Arduino.h>

const char Separator = '_';
const char* myPasscode = "1234";

const char* TrustedRecipient1 = "09161962846";
const char* TrustedRecipient2 = "09161962846";
const char* TrustedRecipient3 = "09161962846";

const String myGSMCommands[11] = {
  "slB",  "rm1B", "rm2B", "crB",  "slF",
  "rm1F", "rm2F", "allB", "allF", "allAp",
  "status"
};

const String myGSMStateCommands[5] = {
  "ON", "OFF"
};

const String TrustedRecipCode[3] = {
  "r1", "r2", "r3"
};

const String emergencyMSG1 = "Huy naay sunog sa Sala";
const String emergencyMSG2 = "Huy naay sunog sa Kwarto";
#endif