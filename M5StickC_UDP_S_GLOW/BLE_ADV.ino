#include <BLEDevice.h>
#include <BLEUtils.h>
#include "BLEBeacon.h"

#define BEACON_UUID           "65432461-1EFE-4ADB-BC7E-9F7F8E27FDC1"
#define TX_POWER   -65

// function
#define GLOW_OFF 0
#define GLOW_ON 1
#define GROUP_ID_START 2
#define GROUP_ID_END 3
#define DEVICE_ID_START 4
#define DEVICE_ID_END 5

BLEAdvertising *pAdvertising;

std::string setUUID(){
  std::string org = BEACON_UUID;
  std::string dst = "";
  if( org.length() != 36 ){
    return "error";
  }
  dst  = org[34]; dst += org[35];
  dst += org[32]; dst += org[33];
  dst += org[30]; dst += org[31];
  dst += org[28]; dst += org[29];
  dst += org[8];
  dst += org[26]; dst += org[27];
  dst += org[24]; dst += org[25];
  dst += org[23];
  dst += org[21]; dst += org[22];
  dst += org[19]; dst += org[20];
  dst += org[18];
  dst += org[16]; dst += org[17];
  dst += org[14]; dst += org[15];
  dst += org[13];
  dst += org[11]; dst += org[12];
  dst += org[9];  dst += org[10];
  dst += org[6];  dst += org[7];
  dst += org[4];  dst += org[5];
  dst += org[2];  dst += org[3];
  dst += org[0];  dst += org[1];
  return dst;
}

void init_BLE(String device_name){
  BLEDevice::init(device_name.c_str());
  pAdvertising = BLEDevice::getAdvertising();
}

void sendGlowData(uint32_t func, uint32_t group, uint32_t id, uint32_t command){
  if(func==GLOW_OFF){            // Glow OFF
    controlDeviceOFF(group,id);
  }
  else if(func==GLOW_ON){
    controlDeviceON(group,id,command);
  }
  else if(func==GROUP_ID_START){
    startGroupSet(group);
  }
  else if(func==GROUP_ID_END){
    endGroupSet();
  }
  else if(func==DEVICE_ID_START){
    startIDSet(id);
  }
  else if(func==DEVICE_ID_END){
    endIDSet();
  }
  else{
    controlDeviceOFF(group,id);
  }
}

void controlDeviceON(int group, int id, int pattern){
  pAdvertising->stop();
  delay(100);
  int sendid = ((group<<10) & 0xffff) + id;
  setAdvertise(sendid,pattern);
  pAdvertising->start(); 
  delay(200);
}

void controlDeviceOFF(int group, int id){
  pAdvertising->stop();
  delay(100);
  int sendid = ((group<<10) & 0xffff) + id;
  setAdvertise(sendid,0);
  pAdvertising->start(); 
  delay(200);
}

void startGroupSet(int group) {
  pAdvertising->stop();
  delay(100);
  if(group<1){
    group=1;
  }
  else if (group>7){
    group=7;
  }
  int sendgrp = ((3 << 13) & 0xffff) +  ((group << 10) & 0xffff);
  setAdvertise(sendgrp,0);
  pAdvertising->start(); 
  delay(100);
}

void endGroupSet() {
  pAdvertising->stop();
  delay(100);
  setAdvertise(0,0);
  pAdvertising->start(); 
  delay(1000);
  pAdvertising->stop();
  delay(100);
}

void startIDSet(int id) {
  pAdvertising->stop();
  delay(100);
  if(id<1){
    id=1;
  }
  else if (id>1023){
    id=1023;
  }
  int sendid = ((2 << 13) & 0xffff) + id;
  setAdvertise(sendid,0);
  pAdvertising->start(); 
  delay(100);
}

void endIDSet() {
  pAdvertising->stop();
  delay(100);
  setAdvertise(0,0);
  pAdvertising->start(); 
  delay(1000);
  pAdvertising->stop();
  delay(100);
}

void setAdvertise(int serial, int cmd){
  BLEBeacon oBeacon = BLEBeacon();
  oBeacon.setManufacturerId(0x4c00);
  oBeacon.setProximityUUID(BLEUUID(setUUID()));
  oBeacon.setMajor(serial);
  oBeacon.setMinor(cmd);
  oBeacon.setSignalPower(TX_POWER);
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();

  oAdvertisementData.setFlags(0x04);

  std::string strServiceData = "";

  strServiceData += (char)26;     // Len
  strServiceData += (char)0xFF;   // Type
  strServiceData += oBeacon.getData(); 
  oAdvertisementData.addData(strServiceData);

  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);
}
