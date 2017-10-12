#include "CapacitiveSensor.h"
//#include "RunningAverage.h"

#define DT 1 //Derivative threshold.
#define IT 50 //Integration threshold.
#define HAND 200 // Hand threshold.
#define AVGN 3 //Degree of the IIR filter.
#define L 0.7 //Leakege Factor.
#define READINGS 2

CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
//RunningAverage rawCap(30);

long current, delta, Integral, Integral_1, avg, avg_1, last;


void setup()                    
{
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(115200);
   pinMode(6, OUTPUT);
   digitalWrite(6, LOW);

    current = cs_4_2.capacitiveSensor(READINGS);
    Serial.print("Initial reading: ");
    Serial.println(current);
    delta = 0;
    Integral = 0;
    Integral_1 = 0;
    avg = 0;
    avg_1 = 0;
    last = 0;
  //avg = current;

//   pinMode(8, OUTPUT);
//   digitalWrite(8, LOW);
//    rawCap.clear();
}

void loop()                    
{
    long start = millis();
//    long total1 =  cs_4_2.capacitiveSensor(40);
    long reading1 = cs_4_2.capacitiveSensor(READINGS);
    Serial.print("Reading: ");
    Serial.println(reading1);
    long total1 = iir_filter(reading1);
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

    delay(1000);                     // arbitrary delay to limit data to serial port 
}


//IIR Filter
long average(unsigned long meassure, long avg_1, int n) {
  long avg = ((avg_1 * n) - avg_1 + meassure) / n;
  return avg;
}

long iir_filter(long measurement) {
  last = current;   // Shift register. The variable last will storage the current value for the next iteration.
  current = average(measurement, last, AVGN); //New value for current return from IIR filter --> function average(meassure, avg_1, n)

  //Derivative integration algorithm
  delta = current - last;

  if (abs(delta) > DT) {
    Integral = Integral_1 + delta;
  }
  else {
    Integral = Integral_1;
  }

  if (abs(Integral) >= IT && abs(Integral) < HAND) {
//    LedOff();
    Integral_1 = Integral;
    Serial.println("Pot approaching...");
  }
  else {
//    LedOn();
    Integral_1 = Integral * L;
//    Serial.println("Pot leaving...");
  }
  return abs(Integral);

//  Serial.println(abs(Integral));
//  delay(100);
}

