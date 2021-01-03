/* 
* i2c_port_address_scanner
* Scans ports D0 to D7 on an ESP8266 and searches for I2C device. based on the original code
* available on Arduino.cc and later improved by user Krodal and Nick Gammon (www.gammon.com.au/forum/?id=10896)
* D8 throws exceptions thus it has been left out
*
*/
#include <Arduino.h>
#include <Wire.h>

uint8_t portArray[] = {16, 14, 12, 13, 5, 4, 0, 2};
//String portMap[] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7"}; //for Wemos
String portMap[] = {"GPIO16", "GPIO14", "GPIO12", "GPIO13", "GPIO5", "GPIO4", "GPIO0", "GPIO2"};

void check_if_exist_I2C() {
  byte error, address;
  int nDevices;
  nDevices = 0;
  for (address = 1; address < 127; address++ )  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0){
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknow error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  } //for loop
  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("**********************************\n");
  //delay(1000);           // wait 1 seconds for next scan, did not find it necessary
}

void scanPorts() { 
  for (uint8_t i = 0; i < sizeof(portArray); i++) {
    for (uint8_t j = 0; j < sizeof(portArray); j++) {
      if (i != j){
        Serial.print("Scanning (SDA : SCL) - " + portMap[i] + " : " + portMap[j] + " - ");
        Wire.begin(portArray[i], portArray[j]);
        check_if_exist_I2C();
      }
    }
  }
}

void scanPorts(uint8_t I2C_SDA, uint8_t I2C_SCL){
    Wire.begin(I2C_SDA, I2C_SCL);
    check_if_exist_I2C();
}

void setup() {
  Serial.begin(115200);
  
  // enable LM75 and BME280/BME680 if you use mec-kon's weather station (https://github.com/mec-kon/weather_station) 
  //pinMode(12, OUTPUT);
  //digitalWrite(12, HIGH);
  
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\n\nI2C Scanner to scan for devices on each port pair D0 to D7");
  scanPorts(4, 5);
}

void loop() {
}

