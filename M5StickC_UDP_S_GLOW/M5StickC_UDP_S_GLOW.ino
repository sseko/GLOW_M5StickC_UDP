
#include <M5StickC.h>

const int LCD_W = M5.Lcd.width();
const int LCD_H = M5.Lcd.height();

void setup()
{ 
  M5.begin();
  M5.Lcd.setTextSize(1);
  M5.Lcd.setRotation(1);
  M5.Lcd.setCursor(0, 0);
  Serial.begin(115200);
  init_BLE("GlowSDK");
  delay(100);
  init_udp();
  delay(100);
}

void loop()
{
  int func;
  int group;
  int id;
  unsigned int comm;

  if(receive_udp(&func,&group,&id,&comm)){
    if(comm>0x7fff)comm=0;
    serial_display(func,group,id,comm);
    displayOut(func,group,id,comm);
    sendGlowData(func,group,id,comm);
  }

  if(receive_serial(&func,&group,&id,&comm)){
    if(comm>0x7fff)comm=0;
    serial_display(func,group,id,comm);
    displayOut(func,group,id,comm);
    sendGlowData(func,group,id,comm);
  }
  
  if (M5.BtnA.wasPressed()){
    displayOut(0,group,id,0x0);
    sendGlowData(0,group,id,0x0);
  }
  if (M5.BtnB.wasPressed()){}
  M5.update();
    
  delay(100);
}

/***************
 * Diplay out
 */

void displayOut(int func, int group, int id, unsigned int command){
  M5.Lcd.fillRect(0, 24, LCD_H, LCD_W-20, BLUE);
  M5.Lcd.setTextColor(WHITE,BLUE);
  M5.Lcd.setCursor(10, 26);
  M5.Lcd.printf("Function data:%d",func);
  M5.Lcd.setCursor(10, 37);
  M5.Lcd.printf("Group ID:%d",group);
  M5.Lcd.setCursor(10, 48);
  M5.Lcd.printf("Device ID:%d",id);
  M5.Lcd.setCursor(10, 59);
  M5.Lcd.printf("Command data:%x",command);
}

void displayOut2(uint32_t func, uint32_t group, uint32_t id, uint32_t command){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setTextSize(2);

  M5.Lcd.setCursor(10, 10); // Function
  M5.Lcd.printf("F:");
  M5.Lcd.setCursor(35, 10);
  M5.Lcd.printf("%d", func);

  M5.Lcd.setCursor(10, 35); // Group
  M5.Lcd.printf("G:");
  M5.Lcd.setCursor(35, 35);
  M5.Lcd.printf("%d", group);

  M5.Lcd.setCursor(10, 60); // ID
  M5.Lcd.printf("ID:");
  M5.Lcd.setCursor(10, 80);
  M5.Lcd.printf("%d", id);

  M5.Lcd.setCursor(10, 105);  // Command
  M5.Lcd.printf("Cmd:");
  M5.Lcd.setCursor(10, 125);
  M5.Lcd.printf("%x", command);
}

void serial_display(int func,int group,int id,int comm){
  Serial.print("function = ");
  Serial.println(func);
  Serial.print("group    = ");
  Serial.println(group);
  Serial.print("id       = ");
  Serial.println(id);
  Serial.print("command  = ");
  Serial.println(comm);
}
