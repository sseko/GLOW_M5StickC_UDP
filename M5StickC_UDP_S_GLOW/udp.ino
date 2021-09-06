#include <WiFi.h>
#include <WiFiUdp.h>

const char pass[] = "password";  // password
const int localPort = 10000;      // ポート番号(pythonのポート番号に合わせる)

const IPAddress ip(192, 168, 4, 1);       // IPアドレス(pythonのIPアドレスに合わせる)
const IPAddress subnet(255, 255, 255, 0); // サブネットマスク

String BleLocalName = "";
String MacAddress = "";

WiFiUDP udp;
String cmds[6] = {"\n"};

void init_udp(){
  readDeviceName();
  int ln = BleLocalName.length();
  char deviceName[ln+1];
  int i;
  for (i=0;i<ln;i++){
    deviceName[i]=BleLocalName[i];
  }
  deviceName[ln]=0x00;
  
  WiFi.softAP(deviceName, pass);
  delay(100);
  WiFi.softAPConfig(ip, ip, subnet);

  Serial.print("AP IP address: ");
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(localPort);

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.print(BleLocalName);
  M5.Lcd.print(" / "+String(pass));
}

bool receive_serial(int* func , int* group , int* id , unsigned int* comm){
  if (Serial.available() > 0){
    //key = Serial.read(); 
    String str = Serial.readStringUntil('\n');
    delay(100);
    Serial.println(str); // Serial通信で来た値を表示
    int index = split(str, ',', cmds);
    
    int pos = cmds[3].indexOf("0x");
    if(pos>=0){
      String cm;
      cm = cmds[3].substring(pos+2);
      *comm = HexAtoi(cm);
    }
    else{
      *comm = cmds[3].toInt();
    }
    
    *func = cmds[0].toInt();
    *group =cmds[1].toInt();
    *id =   cmds[2].toInt();
    //*comm = cmds[3].toInt();
    //*comm = HexAtoi(cmds[3]);
    return true;
  }
  return false;
}

bool receive_udp(int* func , int* group , int* id , unsigned int* comm){
  char c[32];
  int packetSize = udp.parsePacket();
  if(packetSize>0){
    for (int i=0; i<packetSize; i++){
      c[i]=udp.read();
    }
    c[packetSize]=0x00;
    String str = String(c);
    Serial.println(str); // UDP通信で来た値を表示
    int index = split(str, ',', cmds);
    *func = cmds[0].toInt();
    *group =cmds[1].toInt();
    *id =   cmds[2].toInt();
    *comm = cmds[3].toInt();
    return true;
  }
  return false;
}

unsigned int HexAtoi(String str) {
  int len = str.length();
  char sNum[len+1];
  int num[len+1];
  str.toCharArray(sNum, len+1);
  
  unsigned int ret = 0;
  for(int i=0 ; i<len ; i++ ){
    int cn = (len-1)-i;
    if ( sNum[cn] >= 'A' && sNum[cn] <= 'F' ) {
      ret = ret + ((sNum[cn] - 'A' + 10)<<(i*4));
    }
    else if ( sNum[cn] >= 'a' && sNum[cn] <= 'f' ) {
      ret = ret + ((sNum[cn] - 'a' + 10)<<(i*4));
    }
    else if ( sNum[cn] >= '0' && sNum[cn] <= '9' ) {
      ret = ret + ((sNum[cn] - '0')<<(i*4));
    }
    else {
      return -1;
    }
  }
  return ret;
}

int split(String data, char delimiter, String *dst){
  int index = 0;
  int arraySize = (sizeof(data))/sizeof((data[0]));
  int datalength = data.length();

  for(int j=0; j<6; j++){
    dst[j]="";
  }

  for(int i = 0; i < datalength; i++){
    char tmp = data.charAt(i);
    if( tmp == delimiter ){
      index++;
      if( index > (arraySize - 1)) return -1;
    }
    else dst[index] += tmp;
  }
  return (index + 1);
}

void readDeviceName(){
  uint8_t mac0[6];
  esp_efuse_mac_get_default(mac0);
  Serial.printf("Default Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac0[0], mac0[1], mac0[2], mac0[3], mac0[4], mac0[5]);
  MacAddress = String(mac0[4],HEX)+String(mac0[5],HEX);
  BleLocalName = "GLOW_"+MacAddress;
  Serial.println(BleLocalName);
}
