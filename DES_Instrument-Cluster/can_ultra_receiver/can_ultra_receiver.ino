#include "Waveshare_LCD1602_RGB.h"
#include <SPI.h>              //Library for using SPI Communication
#include <mcp2515.h>          //Library for using CAN Communication (https://github.com/autowp/arduino-mcp2515/)
#define slaveID 0x036

Waveshare_LCD1602_RGB lcd(16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

struct can_frame canMsg;

char num_str[20];

MCP2515 mcp2515(10);                 // SPI CS Pin 10

void setup()
{
  Serial.begin(9600);                //Begins Serial Communication at 9600 baudrate
  SPI.begin();                       //Begins SPI communication
  lcd.init();
  lcd.clear();              // Make sure backlight is on
  lcd.setCursor(0, 0);
  lcd.send_string("CANBUS TUTORIAL");
  delay(3000);
  lcd.clear();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS); //Sets CAN at speed 500KBPS Clock
  mcp2515.setNormalMode();                  //Sets CAN at normal mode
}
void loop()
{
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) // To receive data
  {
    if(slaveID != canMsg.can_id) return;
    int x = canMsg.data[0];
    Serial.println(x);
    lcd.setCursor(0, 0);         //Display Temp & Humidity value received
    lcd.send_string("Distance : ");
    sprintf(num_str, "%d", x);
    lcd.send_string(num_str);
    delay(1000);
    lcd.clear();
  }
}