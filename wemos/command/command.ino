
/*

   cmd2jura.ino V1.00

   ESP8266 IP Gateway for Jura coffee machines

   Usage:
   Open "http://jura/" or "curl -d 'TY:' http://jura/api"

   (C) 2017 Hajo Noerenberg

   http://www.noerenberg.de/
   https://github.com/hn/jura-coffee-machine

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 3.0 as
   published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program. If not, see <http://www.gnu.org/licenses/gpl-3.0.txt>.

*/
/*
 * 
 * Jura 7-pin interface
(pin 8 - not used)
pin 7 - not used
pin 6 - +5V
pin 5 - not used
pin 4 - RX
pin 3 - GND
pin 2 - TX
pin 1 - not used
(pin 0 - not used)
 */


#include <SoftwareSerial.h>



#define GPIORX    12
#define GPIOTX    13

SoftwareSerial softserial(GPIORX, GPIOTX);

String cmd2jura(String outbytes) {
  String inbytes;
  int w = 0;

  while (softserial.available()) {
    softserial.read();
  }

  outbytes += "\r\n";
  for (int i = 0; i < outbytes.length(); i++) {
    for (int s = 0; s < 8; s += 2) {
      char rawbyte = 255;
      bitWrite(rawbyte, 2, bitRead(outbytes.charAt(i), s + 0));
      bitWrite(rawbyte, 5, bitRead(outbytes.charAt(i), s + 1));
      softserial.write(rawbyte);
    }
    delay(8);
  }

  int s = 0;
  char inbyte;
  while (!inbytes.endsWith("\r\n")) {
    if (softserial.available()) {
      byte rawbyte = softserial.read();
      bitWrite(inbyte, s + 0, bitRead(rawbyte, 2));
      bitWrite(inbyte, s + 1, bitRead(rawbyte, 5));
      if ((s += 2) >= 8) {
        s = 0;
        inbytes += inbyte;
      }
    } else {
      delay(10);
    }
    if (w++ > 500) {
      return "";
    }
  }

  return inbytes.substring(0, inbytes.length() - 2);
}



void setup() {
  // put your setup code here, to run once:

  
  Serial.begin(9600);
  softserial.begin(9600);
  Serial.println("starting");



}

int i = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    String output = cmd2jura(Serial.readString());
    Serial.println(output);
  }
}
