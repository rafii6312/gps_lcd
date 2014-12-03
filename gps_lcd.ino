#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <LiquidCrystal.h>

//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LiquidCrystal lcd(2, 3, 4, 5, 11, 12);


TinyGPS gps;
SoftwareSerial ss(6, 7);

void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
  lcd.begin(16, 2);

  printLcd(0, "waiting for data");
  printLcd(1, "                ");
}

void loop()
{
  bool newdata = false;
  unsigned long start = millis();

  // Every second we print an update
  while (millis() - start < 1000)
  {
    if (feedgps())
      newdata = true;
  }

  if (newdata)
  {

    String sflat, sflon;
    float flat, flon;
    unsigned long age, date, time, chars = 0;
    unsigned short sentences = 0, failed = 0;


    flat = TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat;
    flon = TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon;

    //flat = gps.

    gps.f_get_position(&flat, &flon, &age);

    //sflat.concat(flat);
    //sflon.concat(flon);

    printLcd(0, toString(flat, "lat: "));
    printLcd(1, toString(flon, "lon: "));


  } 
  else {
    //no new data
    //printLcd(0, "waiting for data");
    //printLcd(1, millis() / 1000);
  }
  //gps.stats(&chars, &sentences, &failed);
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

void printLcd(int line, float txt)
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

static bool feedgps()
{
  while (ss.available())
  {
    if (gps.encode(ss.read()))
      return true;
  }
  return false;
}

String toString(float f, String pre)
{
  char tmp[16];
  dtostrf(f,1,8,tmp);
  String strOut = pre + tmp;
  return strOut;
}




