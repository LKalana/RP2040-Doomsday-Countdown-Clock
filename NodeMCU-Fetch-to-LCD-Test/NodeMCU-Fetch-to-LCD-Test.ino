/*
   AVENGERS: DOOMSDAY COUNTDOWN
   NodeMCU ESP8266 - Fetch to LCD Test

   Release Date: August 23, 2026

   Features:
   - Connects to WiFi
   - Gets current time from Internet using NTP
   - Displays US Time
   - Calculates remaining:
       DAYS : HOURS : MINUTES : SECONDS
   - Display the fetched data on the LCD
*/

#include <ESP8266WiFi.h>
#include <time.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int test = 0;
// ======================================================
// WIFI SETTINGS
// ======================================================

const char* ssid = "SLT-Fiber-2.4G_8750"; // SSID of the WiFi
const char* password = "hard7767"; // Password of the WiFi
// ======================================================
// NTP SETTINGS
// ======================================================

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.google.com";
const char* ntpServer3 = "time.cloudflare.com";


// ======================================================
// USA EASTERN TIME SETTINGS
// ======================================================
//
// New York / USA Eastern Time
//
// Automatically switches between:
//
// EST = UTC - 5
// EDT = UTC - 4
//
// DST Rules:
// Starts: Second Sunday in March
// Ends:   First Sunday in November
//

const char* TIMEZONE = "EST5EDT,M3.2.0,M11.1.0";


// ======================================================
// AVENGERS: DOOMSDAY RELEASE TIME
// ======================================================
//
// December 18, 2026
// 00:00:00 USA Eastern Time
//
// December 18 is during EST.
//
// EST = UTC - 5
//
// Therefore:
//
// December 18, 2026 00:00 EST
// =
// December 18, 2026 05:00 UTC
//
// Unix Timestamp:
// 1797570000
//

const time_t DOOMSDAY_RELEASE = 1797570000;


// ======================================================
// FUNCTION DECLARATIONS
// ======================================================

void connectWiFi();

void displayCurrentTime(time_t currentEpoch);

void displayCountdown(time_t currentEpoch);

bool isLeapYear(int year);

int daysInMonth(int month, int year);


// ======================================================
// SETUP
// ======================================================

void setup() {

  Serial.begin(115200);
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  delay(1000);
  pinMode(D5,OUTPUT);
  digitalWrite(D5,LOW);
  digitalWrite(D5,HIGH);
  delay(50);
  digitalWrite(D5,LOW);
  delay(50);
  digitalWrite(D5,HIGH);
  delay(50);
  digitalWrite(D5,LOW);
  Serial.println();
  Serial.println("==================================================");
  Serial.println("          AVENGERS: DOOMSDAY COUNTDOWN");
  Serial.println("==================================================");
  Serial.println();

  // ----------------------------------------------------
  // CONNECT TO WIFI
  // ----------------------------------------------------
  connectWiFi();
  // ----------------------------------------------------
  // CONFIGURE INTERNET TIME
  // ----------------------------------------------------
  configTime(
    0,
    0,
    ntpServer1,
    ntpServer2,
    ntpServer3
  );


  // ----------------------------------------------------
  // CONFIGURE USA EASTERN TIME
  // ----------------------------------------------------

  setenv(
    "TZ",
    TIMEZONE,
    1
  );

  tzset();


  // ----------------------------------------------------
  // WAIT FOR NTP SYNCHRONIZATION
  // ----------------------------------------------------

  Serial.print("Synchronizing Internet Time");

  time_t now = time(nullptr);

  while (now < 1000000000) {

    Serial.print(".");

    delay(500);

    now = time(nullptr);
  }

  Serial.println();
  Serial.println();

  Serial.println("Internet Time Synchronized!");

  Serial.println();
}


// ======================================================
// MAIN LOOP
// ======================================================

void loop() {
  SHOWDOWN();
  lcd.backlight();
  time_t now = time(nullptr);
  // Display current USA Eastern Time
  displayCurrentTime(now);

  // Display Avengers: Doomsday Countdown
  displayCountdown(now);
  
  Serial.println(
    "--------------------------------------------------"
  );

  delay(1000);
}


// ======================================================
// CONNECT TO WIFI
// ======================================================

void connectWiFi() {
  int cnt = 9;
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);

  WiFi.begin(
    ssid,
    password
  );


  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    lcd.setCursor(0,0);
    lcd.print("Searching");
    lcd.setCursor(cnt,0);
    lcd.print(".");
    cnt++;
    Serial.print(".");
    digitalWrite(LED_BUILTIN,HIGH);
  }


  Serial.println();
  Serial.println();

  Serial.println("WiFi Connected!");
  digitalWrite(LED_BUILTIN,LOW);
  lcd.clear();
  lcd.setCursor(0,0);
  for(int i=0;i<=2;i++)
  {
    lcd.print("Found");
    delay(100);
    lcd.clear();
    delay(100);
    lcd.setCursor(0,0);
  }
  Serial.print("IP Address: ");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Located....");
  lcd.setCursor(0,1);
  lcd.print("7.8731N,80.7718E");
  Serial.println(
    WiFi.localIP()
  );

  Serial.println();
  delay(500);
  lcd.clear();
  delay(100);
  
}


// ======================================================
// DISPLAY CURRENT USA EASTERN TIME
// ======================================================

void displayCurrentTime(time_t currentEpoch) {

  struct tm* timeInfo;

  timeInfo = localtime(
    &currentEpoch
  );


  char timeBuffer[40];


  // Example:
  // 2026-08-23 08:50:35 EDT

  strftime(
    timeBuffer,
    sizeof(timeBuffer),
    "%Y-%m-%d %H:%M:%S %Z",
    timeInfo
  );


  Serial.print(
    "Current USA Eastern Time : "
  );

  Serial.println(
    timeBuffer
  );
}


// ======================================================
// CHECK LEAP YEAR
// ======================================================

bool isLeapYear(int year) {

  return (
    (year % 400 == 0) ||
    (
      (year % 4 == 0) &&
      (year % 100 != 0)
    )
  );
}


// ======================================================
// GET NUMBER OF DAYS IN MONTH
// ======================================================

int daysInMonth(
  int month,
  int year
) {

  switch (month) {

    case 1:
      return 31;

    case 2:

      if (isLeapYear(year)) {
        return 29;
      }
      else {
        return 28;
      }

    case 3:
      return 31;

    case 4:
      return 30;

    case 5:
      return 31;

    case 6:
      return 30;

    case 7:
      return 31;

    case 8:
      return 31;

    case 9:
      return 30;

    case 10:
      return 31;

    case 11:
      return 30;

    case 12:
      return 31;
  }


  return 30;
}


// ======================================================
// DISPLAY AVENGERS: DOOMSDAY COUNTDOWN
// ======================================================

void displayCountdown(time_t currentEpoch) {

  // ----------------------------------------------------
  // CHECK IF COUNTDOWN IS COMPLETE
  // ----------------------------------------------------

  if (
    currentEpoch >= DOOMSDAY_RELEASE
  ) {

    Serial.println();

    Serial.println(
      "=================================================="
    );

    Serial.println(
      "                 DOOMSDAY IS HERE"
    );

    Serial.println(
      "=================================================="
    );

    Serial.println();

    return;
  }


  // ----------------------------------------------------
  // GET CURRENT USA EASTERN TIME
  // ----------------------------------------------------

  struct tm currentTimeData;

  localtime_r(
    &currentEpoch,
    &currentTimeData
  );


  // ----------------------------------------------------
  // CURRENT DATE / TIME
  // ----------------------------------------------------

  int currentYear =
    currentTimeData.tm_year + 1900;

  int currentMonth =
    currentTimeData.tm_mon + 1;

  int currentDay =
    currentTimeData.tm_mday;

  int currentHour =
    currentTimeData.tm_hour;

  int currentMinute =
    currentTimeData.tm_min;

  int currentSecond =
    currentTimeData.tm_sec;


  // ----------------------------------------------------
  // TARGET DATE / TIME
  // ----------------------------------------------------
  //
  // Avengers: Doomsday
  //
  // December 18, 2026
  // 00:00:00 Eastern Time
  //

  int targetYear = 2026;

  int targetMonth = 12;

  int targetDay = 18;

  int targetHour = 0;

  int targetMinute = 0;

  int targetSecond = 0;


  // ----------------------------------------------------
  // CALCULATE SECONDS
  // ----------------------------------------------------

  int seconds =
    targetSecond - currentSecond;


  if (seconds < 0) {

    seconds += 60;

    targetMinute--;
  }


  // ----------------------------------------------------
  // CALCULATE MINUTES
  // ----------------------------------------------------

  int minutes =
    targetMinute - currentMinute;


  if (minutes < 0) {

    minutes += 60;

    targetHour--;
  }


  // ----------------------------------------------------
  // CALCULATE HOURS
  // ----------------------------------------------------

  int hours =
    targetHour - currentHour;


  if (hours < 0) {

    hours += 24;

    targetDay--;
  }


  // ----------------------------------------------------
  // CALCULATE DAYS
  // ----------------------------------------------------

  int days =
    targetDay - currentDay;


  if (days < 0) {

    targetMonth--;


    if (targetMonth == 0) {

      targetMonth = 12;

      targetYear--;
    }


    days += daysInMonth(
      targetMonth,
      targetYear
    );
  }


  // ----------------------------------------------------
  // CALCULATE MONTHS
  // ----------------------------------------------------

  int months =
    ((targetYear - currentYear) * 12)
    +
    (targetMonth - currentMonth);


  // ----------------------------------------------------
  // DISPLAY COUNTDOWN
  // ----------------------------------------------------

  Serial.println();

  Serial.println(
    "             AVENGERS: DOOMSDAY IN"
  );

  Serial.println();


  Serial.println(
    " MONTHS : DAYS : HOURS : MINS : SECS"
  );


  Serial.printf(
    "   %02d   :  %02d  :   %02d  :  %02d  :  %02d\n",
    months,
    days,
    hours,
    minutes,
    seconds
  );
  // ------------------------------ Printing on LCD.
  lcd.setCursor(0,1);
  lcd.print("0");
  lcd.setCursor(1,1);
  lcd.print(months);
  lcd.setCursor(2,1);
  lcd.print("M");
  //----------------------------------- Months
  lcd.setCursor(3,1);
  lcd.print(" ");
  lcd.setCursor(4,1);
  lcd.print(days);
  lcd.setCursor(6,1);
  lcd.print("D");
  //----------------------------------- Days
  lcd.setCursor(7,1);
  lcd.print(" ");
  lcd.setCursor(8,1);
  lcd.print(hours);
  lcd.setCursor(10,1);
  lcd.print("h");
  //----------------------------------- Hours
  lcd.setCursor(11,1);
  lcd.print(" ");
  lcd.setCursor(12,1);
  lcd.print(minutes);
  lcd.setCursor(14,1);
  lcd.print("m");
  //----------------------------------- Minutes
  if(seconds < 10)
  {
    lcd.setCursor(13,0);
    lcd.print("0");
    lcd.setCursor(14,0);
    lcd.print(seconds);
    lcd.setCursor(15,0);
    lcd.print("s");
  }else{
    lcd.setCursor(13,0);
    lcd.print(seconds);
    lcd.setCursor(15,0);
    lcd.print("s");
  }
  //----------------------------------- Secs

  Serial.println();
}
// Display the Dimming
void SHOWDOWN()
{
  if(!test)
  {
    for(int i=0;i<=5;i++){
    lcd.backlight();
    delay(50);
    lcd.noBacklight();
    delay(50);
  }
  for(int i=0;i<=1;i++){
    lcd.backlight();
    delay(50);
    lcd.noBacklight();
    delay(50);
  }
  for(int i=0;i<=1;i++){
    lcd.backlight();
    delay(50);
    lcd.noBacklight();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DOOM ARRIVES");
    delay(50);
    test = 1;
  }
  }
}
