#include <Arduino.h>

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// TODO закомментировать эту строку перед заливкой на
// «боевую» сборку
#define DEBUG

// Светодиод успешного запуска
#define HEALTH_CHECK_LED 13

// Пины для общения с DFPlayer
#define RX_PIN 12 // к пину TX на DFP
#define TX_PIN 11 // к пину RX на DFP

// Пины для сэмплов
#define KEYBOARD_INIT_PIN 4
#define KEYBOARD_LAST_PIN 10

#ifdef DEBUG
#define debug(msg) Serial.println(msg)
#else
#define debug(msg)
#endif

SoftwareSerial dfplayerSerial(RX_PIN, TX_PIN);
DFRobotDFPlayerMini myDFPlayer;

int trackDiff = KEYBOARD_INIT_PIN - 1;
int currentRecord;

void setup()
{
  dfplayerSerial.begin(9600);
#ifdef DEBUG
  Serial.begin(9600);
#endif

  for (byte pin = KEYBOARD_INIT_PIN; pin <= KEYBOARD_LAST_PIN; pin++) {
    pinMode(pin, INPUT);
  }

  pinMode(HEALTH_CHECK_LED, OUTPUT);

  debug("Initializing DFPLayer");
  if (!myDFPlayer.begin(dfplayerSerial))
  {
    debug("Unable to begin:");
    debug("1.Please recheck the connection!");
    debug("2.Please insert the SD card!");
    while (true)
    {
      digitalWrite(HEALTH_CHECK_LED, HIGH);
      delay(200);
      digitalWrite(HEALTH_CHECK_LED, LOW);
      delay(1000);
    };
  }
  else
  {
    debug("DFPlayer Mini online.");
    digitalWrite(HEALTH_CHECK_LED, HIGH);
  }

  myDFPlayer.volume(30); //TODO сделать настройку громкости через переменный резистор
}

void loop()
{
  currentRecord = -1;
  for (byte button = KEYBOARD_INIT_PIN; button <= KEYBOARD_LAST_PIN; button++) {
    if (digitalRead(button)) {
      currentRecord = button - trackDiff;
    }
  }
  if (currentRecord > -1) {    
    myDFPlayer.play(currentRecord);
  } else {
    myDFPlayer.pause();
  }
}
