/* =============================================================================
  LIDAR-Lite v2: Single sensor get single distance measurement

  This example file demonstrates how to take a single distance measurement with
  LIDAR-Lite v2 "Blue Label".

  The library is in BETA, so subscribe to the github repo to recieve updates, or
  just check in periodically:
  https://github.com/PulsedLight3D/LIDARLite_v2_Arduino_Library
  
  To learn more read over lidarlite.cpp as each function is commented
=========================================================================== */

#include <Wire.h>
#include <LIDARLite.h>
#include <RFduinoBLE.h>

/*****************BEGIN VARIABLES*****************/

LIDARLite myLidarLite;
int current_distance = 0;
boolean send_distance = false;

/*****************BEGIN SETUP*****************/
void setup() {
  setup_lidar();
  setup_ble();
}

void setup_lidar(){
  Serial.begin(9600);
  myLidarLite.begin();
}

void setup_ble(){
  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  RFduinoBLE.advertisementData = "moonshine_distance";
  // start the BLE stack
  RFduinoBLE.begin();
}

/*****************BEGIN LOOP*****************/
void loop() {
  lidar_loop();
  ble_loop();
  delay(500);
}

void lidar_loop(){
  current_distance = myLidarLite.distance();
  Serial.println(current_distance);
}

void ble_loop(){
  if(send_distance==true){
    RFduinoBLE.sendInt(current_distance);
    send_distance = false;
  }
}

/*****************BLE EVENTS*****************/
void RFduinoBLE_onDisconnect()
{
  
}

void RFduinoBLE_onReceive(char *data, int len)
{
  // if the first byte is 0x01 / on / true
  if (data[0]){
    send_distance = true;
  }

}

