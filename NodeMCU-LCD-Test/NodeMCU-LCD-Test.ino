/*
   AVENGERS: DOOMSDAY COUNTDOWN
   NodeMCU ESP8266 - LCD TEST

   Release Date: August 23, 2026

   Features:
   - This will test NodeMCU with 16x2 LCD Display
*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int test = 0;
void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
}


void loop()
{
  delay(1000);
  SHOWDOWN();
  delay(100);
  lcd.backlight();
}

void SHOWDOWN()
{
  if(!test)
  {
    for(int i=0;i<=2;i++){
    lcd.backlight();
    delay(100);
    lcd.noBacklight();
    delay(100);
  }
  for(int i=0;i<=1;i++){
    lcd.backlight();
    delay(100);
    lcd.noBacklight();
    delay(100);
  }
  for(int i=0;i<=1;i++){
    lcd.backlight();
    delay(100);
    lcd.noBacklight();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DOOM ARRIVES IN");
    delay(100);
    test = 1;
  }
  }
}
