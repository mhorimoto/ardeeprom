// -*- C++ -*-
//  Arduino の EEPROMを扱うユーティリティプログラム
//

#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <AT24CX.h>

AT24CX  atmem(7,32);

char cha,cmnd[81],lbf[81];
int  cp ;

void setup(void) {
  Serial.begin(115200);
  Serial.println("START COMPLETED V030");
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
    cmnd_dump(s,-1);
  } else if (s.indexOf("atdump")==0) {
    cmnd_dump(s,7);
  } else if (s.indexOf("setb")==0) {
    cmnd_setbyte(s);
  }
}


void cmnd_setbyte(String p) {
  int addr,ofs1,ofs2;
  String q;
  byte d,v,buf[5];
  ofs1 = p.indexOf(" ");
  ofs2 = p.indexOf(" ",ofs1+1);
  if (ofs1>0) {  // アドレス引数がある
    q = p.substring(ofs1,ofs2);
    q.toCharArray(&buf[0],5);
    addr = (int)strtol(buf,NULL,16);
    if (ofs2>0) { // データ引数がある
      q = p.substring(ofs2);
      q.toCharArray(&buf[0],4);
      d = (int)strtol(buf,NULL,16);
      sprintf(lbf,"[%03XH]<=%02XH",addr,d);
      Serial.println(lbf);
      v = EEPROM.read(addr);
      if (v!=d) {
	EEPROM.write(addr,d);
	Serial.println("DONE");
      } else {
	Serial.println("NO WRITE,SAME DATA");
      }
    } else {
      Serial.println(F("ERROR NO DATA"));
    }
  } else {
    Serial.println(F("ERROR NO ADDR"));
  }
}

