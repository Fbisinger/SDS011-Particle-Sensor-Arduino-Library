/* ============================================
Arduino SDS011 - Sensor Library - Sleepmode example
      2018 F.Bisinger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/
#include <SDS011.h>

float pm10,pm25;

SDS011 demo_sensor;

void setup() {
  demo_sensor.begin(5,6); //TX,RX
  Serial.begin(9600);
}

void loop() {
  demo_sensor.sleep();
  delay(30000);                                 //sleeps for 30 seconds (The fan stops spinning & senor turns into sleeping mode => longer lifetime + lower power consumption)
  demo_sensor.wakeup();
  delay(5000);                                   //some reaction time for the sensor
  
  if (! demo_sensor.read(&pm25,&pm10)) {        //new measurement (returns a 0 when reading was successfull)
    Serial.println("PM2.5: "+String(pm25));
    Serial.println("PM10:  "+String(pm10));
  }
  
  else {
    Serial.println("Error reading sensor data. Check wiring!");
    }
  delay(1000);
}