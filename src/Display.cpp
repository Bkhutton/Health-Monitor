/* Display Setup */
#include <U8g2lib.h>
#include <BMP280.h>
#include <HeartRateMonitor.h>

void initDisplay();
void updateScreen();

U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R2, /* clock=*/ 18, /* data=*/ 23, /* cs=*/ 5, /* dc=*/ 16, /* reset=*/ 17);


void initDisplay() {
  u8g2.begin();
}

void updateScreen() {
  u8g2.clearBuffer();					              // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	      // choose a suitable font
  u8g2.setCursor(0, 15);
  u8g2.print("Temp: ");
  u8g2.println(bmp.readTemperature());
  u8g2.setCursor(0, 30);
  u8g2.print("Pressure: ");
  u8g2.println(bmp.readPressure());
  u8g2.setCursor(0, 45);
  u8g2.print("Heart: ");
  u8g2.println(pox.getHeartRate());
  u8g2.setCursor(0, 60);
  u8g2.print("Sp02: ");
  u8g2.println(pox.getSpO2());
  u8g2.sendBuffer();					              // transfer internal memory to the display
}