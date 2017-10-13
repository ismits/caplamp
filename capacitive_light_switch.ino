#include "CapacitiveSensor.h"
//#define __NOCALIBRATE__
#define SENDPIN    4
#define SENSEPIN   2
#define LEDPIN     6
#define TOUCHPIN   8
#define SAMPLINGS 40

CapacitiveSensor capSense = CapacitiveSensor(SENDPIN, SENSEPIN);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

long touchCap = 1000;
long lastCap;
boolean isApproaching = false;
boolean isReceding = false;
int brightness[] = {0, 1, 3, 7, 15, 33, 65, 127, 255};
int brightnessCount = 8;
int curBrightness = 0;
unsigned long lastTouch = 0;
boolean isTouched = false;

void setup()                    
{
  #if defined(__NOCALIBRATE__)
    capSense.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  #endif
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  pinMode(TOUCHPIN, OUTPUT);
  analogWrite(LEDPIN, LOW);
  analogWrite(TOUCHPIN, LOW);
  lastCap = capSense.capacitiveSensor(SAMPLINGS);
}

int getBrightness(boolean increasing) {
  if (increasing && curBrightness < brightnessCount - 1) {
    return curBrightness + 1;
  }
  if (!increasing && curBrightness > 0) {
    return curBrightness - 1;
  }
  return curBrightness;
}

void blinkLed(int cnt) {
  for (int i = 0; i < cnt; i++) {
    analogWrite(LEDPIN, 0);
    delay(100);
    analogWrite(LEDPIN, curBrightness);
    delay(100);
  }
}

void loop()                    
{
    long start = millis();
    long total1 =  capSense.capacitiveSensor(SAMPLINGS);
    long duration = millis() - start;

    if (total1 - lastCap > touchCap) {
      isApproaching = true;
      curBrightness = brightnessCount;
      isTouched = true;
      analogWrite(TOUCHPIN, 255);
      Serial.print("TOUCH! ");
    }
    else if (!isApproaching && total1 - lastCap > 200) {
      // approaching
      isApproaching = true;
      isReceding = false;
      curBrightness = getBrightness(true);
      Serial.print("A ");
      Serial.print(curBrightness);
      Serial.print(" ");
      isTouched = false;
      analogWrite(TOUCHPIN, LOW);
    }
    else if (isApproaching && curBrightness < brightnessCount - 1) {
      isReceding = false;
      curBrightness = getBrightness(true);
      Serial.print("B ");
      Serial.print(curBrightness);
      Serial.print(" ");
      isTouched = false;
      analogWrite(TOUCHPIN, LOW);
    }
    else if (isApproaching && total1 < 200) {
      isApproaching = false;
      isReceding = true;
      Serial.print("C ");
      Serial.print(curBrightness);
      Serial.print(" ");
      isTouched = false;
      analogWrite(TOUCHPIN, LOW);
    }
    else if (isReceding && curBrightness > 0) {
      isReceding = true;
      isApproaching = false;
      curBrightness = getBrightness(false);
      Serial.print("D ");
      Serial.print(curBrightness);
      Serial.print(" ");
      isTouched = false;
      analogWrite(TOUCHPIN, LOW);
    }
    else if (isReceding) {
      isReceding = false;
      isApproaching = false;
      Serial.print("E ");
      Serial.print(curBrightness);
      Serial.print(" ");
      isTouched = false;
      analogWrite(TOUCHPIN, LOW);
    }
    else {
      isTouched = false;
      analogWrite(TOUCHPIN, LOW);
    }
    analogWrite(LEDPIN, brightness[curBrightness]);

    Serial.print(duration);        // check on performance in milliseconds
    Serial.print("\t");            // tab character for debug windown spacing
    Serial.print(total1);        // print sensor output 1
    Serial.print("\t");            // tab character for debug windown spacing
    Serial.println("");
    
    lastCap = total1;
    delay(10);                     // arbitrary delay to limit data to serial port 
}

