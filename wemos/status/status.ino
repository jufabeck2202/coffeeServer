/*
    CoffeeMaker: Arduino Uno Wifi REST Server for the Jura S95 CoffeeMaker
    Acts as a backend for Homebridge / Siri
    Gitub Repository: https://github.com/thomaswitt/CoffeeMaker
    Written by Thomas Witt <https://github.com/thomaswitt>

    CoffeeMaker is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CoffeeMaker is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CoffeeMaker. If not, see <http://www.gnu.org/licenses/>.
*/

#include <SoftwareSerial.h>
#define GPIORX    12
#define GPIOTX    13

SoftwareSerial softserial(GPIORX, GPIOTX);
String uriPrefix = "/arduino/custom/";
int ledPin = 13;



void setup() {
Serial.begin(9600);
  softserial.begin(9600);
  delay(100);
  softserial.listen();

}

void loop() {
  String test = getStatus();
  Serial.println(test);
  delay(5000);
}





/*
  S95-Commands:
  AN:01 switches coffeemaker on
  AN:02 switches coffeemaker off
  FA:02 flush
  FA:04 small cup
  FA:05 two small cups
  FA:06 large cup
  FA:07 two large cups
  FA:08 Steam portion
  FA:09 Steam
  FA:0C XXL cup

  Thanks to:
  - https://github.com/psct/sharespresso
  - https://github.com/oliverk71/Coffeemaker-Payment-System
*/

void toCoffeemaker(String outputString)
{
  for (byte a = 0; a < outputString.length(); a++) {
    byte d0 = 255;
    byte d1 = 255;
    byte d2 = 255;
    byte d3 = 255;
    bitWrite(d0, 2, bitRead(outputString.charAt(a), 0));
    bitWrite(d0, 5, bitRead(outputString.charAt(a), 1));
    bitWrite(d1, 2, bitRead(outputString.charAt(a), 2));
    bitWrite(d1, 5, bitRead(outputString.charAt(a), 3));
    bitWrite(d2, 2, bitRead(outputString.charAt(a), 4));
    bitWrite(d2, 5, bitRead(outputString.charAt(a), 5));
    bitWrite(d3, 2, bitRead(outputString.charAt(a), 6));
    bitWrite(d3, 5, bitRead(outputString.charAt(a), 7));
    softserial.write(d0);
    delay(1);
    softserial.write(d1);
    delay(1);
    softserial.write(d2);
    delay(1);
    softserial.write(d3);
    delay(7);
  }
}

String fromCoffeemaker() {
  delay(10);
  String inputString = "";
  byte d0, d1, d2, d3;
  char d4 = 255;
  while (softserial.available() > 0) {
    d0 = softserial.read();
    delay (1);
    d1 = softserial.read();
    delay (1);
    d2 = softserial.read();
    delay (1);
    d3 = softserial.read();
    delay (7);
    bitWrite(d4, 0, bitRead(d0, 2));
    bitWrite(d4, 1, bitRead(d0, 5));
    bitWrite(d4, 2, bitRead(d1, 2));
    bitWrite(d4, 3, bitRead(d1, 5));
    bitWrite(d4, 4, bitRead(d2, 2));
    bitWrite(d4, 5, bitRead(d2, 5));
    bitWrite(d4, 6, bitRead(d3, 2));
    bitWrite(d4, 7, bitRead(d3, 5));
    inputString += d4;
  }
  inputString.trim();
  if (inputString != "") {
    return (inputString);
  }
}

String getStatus() {
  String input = "";
  String onOff = "";
  String status1 = "";
  String status2 = "";
  toCoffeemaker(F("RR:03\r\n"));
  delay(100);
  input = fromCoffeemaker();
  onOff = input.substring(4, 5);
  status1 = input.substring(6, 7);
  status2 += input.substring(25, 28);
  
    return "Unknown (" + status1 + " / " + status2 + " / " + input +")";
  
}
