// Include libraries
#include <WiFi.h>
#include "time.h"
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"

// Configure LiquidCrystal_I2C library with 0x27 address, 16 columns and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// initialize Class:
BluetoothSerial ESP_BT;

//  Time and wifi declarations
const char* ntpServer = "pool.ntp.org";

//  BT declarations
int zone = 0;
int const offset_arr[]={7200, 3600, 7200, 10800, 36000, 28800, 32400, -14400};
char const *zones_arr[] = {"Cairo  ", "London ", "Paris  ", "Moscow ", "Sydney ", "Beijing", "Tokyo  ", "Toronto"};

// Print the time on the LCD
void printTime()
{
  //  Get out of function if no time available
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return;
  }

  // Print time in LCD
  lcd.setCursor(0, 0);
  lcd.print(&timeinfo, "%H:%M:%S");
  lcd.setCursor(0, 1);
  lcd.print(&timeinfo, "%B %d %Y");
}


void setup()
{  
  //connect to WiFi
  WiFi.begin("YOUR_SSID", "YOUR_PASSWORD");

  // Initialize I2C LCD module (SDA = GPIO21, SCL = GPIO22)
  lcd.begin(21, 22);
  // Turn backlight ON
  lcd.backlight();
  
  // Name of your Bluetooth interface -> will show up on your phone
  ESP_BT.begin("ESP32_Control");

}

void loop()
{
    // get the time
  configTime(offset_arr[zone], 0, ntpServer);
  
  delay(500);
  
  lcd.setCursor(9, 0);
  lcd.print(zones_arr[zone]);
  // Print time on LCD
  printTime();

  // -------------------- Receive Bluetooth signal ----------------------
  if (ESP_BT.available()) 
  {
    zone = ESP_BT.read(); //Read what we receive 
  }
  
}
