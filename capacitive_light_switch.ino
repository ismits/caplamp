#include "CapacitiveSensor.h"
//#include "RunningAverage.h"

CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
//RunningAverage rawCap(30);

void setup()                    
{
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(115200);
   pinMode(6, OUTPUT);
   digitalWrite(6, LOW);
//   pinMode(8, OUTPUT);
//   digitalWrite(8, LOW);
//    rawCap.clear();
}

void loop()                    
{
    long start = millis();
    long total1 =  cs_4_2.capacitiveSensor(40);
    long duration = millis() - start;
//    rawCap.addValue(total1);

    if (duration > 82 && total1 > 1000) {
      digitalWrite(6, HIGH);
      digitalWrite(8, HIGH);
      Serial.print("TOUCH! ");
    }
    else {
      digitalWrite(6, LOW);
      digitalWrite(8, LOW);
    }
    Serial.print(duration);        // check on performance in milliseconds
    Serial.print("\t");            // tab character for debug windown spacing
    Serial.print(total1);        // print sensor output 1
    Serial.print("\t");            // tab character for debug windown spacing
//    Serial.println(rawCap.getAverage(), 3);
//    Serial.print("\tRaw: \t");
//    Serial.println(raw1);
    Serial.println("");

    delay(10);                     // arbitrary delay to limit data to serial port 
}

