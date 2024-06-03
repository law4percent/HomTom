#include <EEPROM.h>

// Define the maximum length of each string
const int maxLength = 10;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Example strings to store
  String commands[] = {"Bulb1", "Bulb2", "Bulb3", "Bulb4", "Fan1", "Fan2", "Fan3"};

  // Write strings to EEPROM
  for (int i = 0; i < 7; i++) {
    writeString(i * maxLength, commands[i]);
  }

  // Read and print strings from EEPROM
  for (int i = 0; i < 7; i++) {
    String command = readString(i * maxLength);
    Serial.println(command);
  }

  // Wait for some time before looping again
  delay(5000);
}

// Function to write a string to EEPROM
void writeString(int addr, String data) {
  for (size_t i = 0; i < data.length(); i++) {
    EEPROM.write(addr + i, data[i]);
  }
  EEPROM.write(addr + data.length(), '\0'); // Null terminate the string
}

// Function to read a string from EEPROM
String readString(int addr) {
  String data = "";
  char character;
  for (int i = 0; i < maxLength; i++) {
    character = EEPROM.read(addr + i);
    if (character == '\0') break; // Stop reading if null character is encountered
    data += character;
  }
  return data;
}
