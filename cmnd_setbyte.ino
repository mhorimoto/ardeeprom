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
