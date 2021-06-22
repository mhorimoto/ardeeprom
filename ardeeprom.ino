// -*- C++ -*-
//  Arduino の EEPROMを扱うユーティリティプログラム
//

#include <avr/pgmspace.h>
#include <EEPROM.h>

char cha,cmnd[81],lbf[81];
int  cp ;

void setup(void) {
  Serial.begin(115200);
  Serial.println(F("START COMPLETED V030"));
  sprintf(lbf,"EEPROM SIZE 0-%03XH(%d bytes)",EEPROM.length(),EEPROM.length());
  Serial.println(lbf);
  cha = (char)NULL;
  cp  = 0;
}

void loop(void) {
  bool reading;
  String s;
  reading = true;
  cp = 0;
  Serial.print("% ");
  Serial.flush();
  while(reading) {
    if (Serial.available() > 0) {
      cha = Serial.read();
      if ((cha>=0x20)&&(cha<=0x7e)) {
	cmnd[cp] = (char)cha;
	Serial.print(cha);
	cp++;
      } else if (cha==0x08) {
	if (cp>0) {
	  cmnd[cp] = (char)NULL;
	  Serial.write(0x08);
	  Serial.print(" ");
	  Serial.write(0x08);
	  cp--;
	}
      } else if ((cha==0x0a)||(cha==0x0d)) {
	reading = false;
	Serial.println();
	cmnd[cp] = (char)NULL;
	s = String(cmnd);
      }
    }
  }
  if (s.indexOf("dump")==0) {
    cmnd_dump(s);
  } else if (s.indexOf("setb")==0) {
    cmnd_setbyte(s);
  } else if (s.indexOf("fill")==0) {
    cmnd_fill(s);
  }
}
