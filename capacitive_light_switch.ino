#include "CapacitiveSensor.h"
#include "RunningStatistics.h"

#define __NOCALIBRATE__
#define SENDPIN    2
#define SENSEPIN   4
#define LEDPIN     6
#define TOUCHPIN   8
#define SAMPLINGS 40
#define RSLENGTH  40

CapacitiveSensor capSense = CapacitiveSensor(SENDPIN, SENSEPIN);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
RunningStatistics statistics = RunningStatistics(RSLENGTH);

long touchCap = 1000;
long lastCap;
long index = 0;
boolean isApproaching = false;
boolean isReceding = false;
int brightness[] = {0, 1, 3, 7, 15, 33, 65, 127, 255};
int brightnessCount = 8;
int curBrightness = 0;
unsigned long lastTouch = 0;
boolean isTouched = false;

float lastAvg = 0;
float lastDev = 0;

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
  statistics.fillValue(lastCap, RSLENGTH);
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

void loop()                    
{
    long start = millis();
    long total1 =  capSense.capacitiveSensor(SAMPLINGS);
    long duration = millis() - start;

    if  (index > RSLENGTH && (total1 - lastAvg) / lastDev > 3.0)
    {
        Serial.println("SOMETHING HAPPENED");
    }
    else {
        statistics.addValue(total1);
    }
    
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

    Serial.print(index++);        // print sensor output 1
    Serial.print("\t");            // tab character for debug windown spacing
    Serial.print(duration);        // check on performance in milliseconds
    Serial.print("\t");            // tab character for debug windown spacing
    Serial.print(total1);        // print sensor output 1
    Serial.print("\t");            // tab character for debug windown spacing
    float avg = statistics.getAverage();
    Serial.print(avg);
    Serial.print("\t");            // tab character for debug windown spacing
    float sd = statistics.getStandardDeviation();
    Serial.print(sd);
    Serial.print("\t");            // tab character for debug windown spacing
    float devs = (total1 - avg) / sd;
    Serial.print(devs);
//    if (devs < -2.0) Serial.print("\tBYE!");
    if (devs > 4.0) Serial.print("\tHELLO!");
    if (devs > 4.0) Serial.print("\tHELLO!");

    Serial.println("");
    
    lastCap = total1;
    lastDev = sd;
    lastAvg = avg;
    delay(50);                     // arbitrary delay to limit data to serial port 
}

