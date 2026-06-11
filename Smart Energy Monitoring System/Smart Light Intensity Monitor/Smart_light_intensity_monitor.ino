#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// LDR PIN
#define LDR_PIN 34

int lightValue = 0;

void setup()
{
  Serial.begin(115200);

  // I2C PINS
  Wire.begin(21, 22);

  // OLED START
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED FAILED");
    while(true);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(2);
  display.setCursor(10,20);
  display.println("START");
  display.display();

  delay(2000);
}

void loop()
{
  // READ LDR
  lightValue = analogRead(LDR_PIN);

  Serial.println(lightValue);

  // OLED DISPLAY
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("SMART LIGHT");

  display.setCursor(0,15);
  display.println("INTENSITY:");

  display.setTextSize(2);
  display.setCursor(0,35);
  display.print(lightValue);

  // LIGHT STATUS
  display.setTextSize(1);
  display.setCursor(85,40);

  if(lightValue > 2000)
  {
    display.println("DARK");
  }
  else
  {
    display.println("BRIGHT");
  }

  display.display();

  delay(500);
}