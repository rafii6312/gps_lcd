#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
TinyGPS gps;
SoftwareSerial ss(6, 7);

void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
  lcd.begin(16, 2);
  
  printLcd(0, "waiting for data");
  printLcd(1, "................");
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      if (gps.encode(c)) newData = true;
    }
  }

  if (newData)
  {
    
    String sflat, sflon;
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    
    sflat.concat(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat);
    sflon.concat(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon);
    
    printLcd(0, "lat:" + sflat);
    printLcd(1, "lon:" + sflon);
        
    
  } else {
    //no new data
  }
  gps.stats(&chars, &sentences, &failed);
}

void printLcd(int line, String txt)
{
  clearLcd(line);
  lcd.setCursor(0, line);
  lcd.print(txt);
  Serial.print(line);
  Serial.print(") ");
  Serial.println(txt);
}

void clearLcd(int line)
{
  lcd.setCursor(0, line);
  lcd.print("                ");
}



