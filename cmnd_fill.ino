void cmnd_fill(String p) {
  int saddr,eaddr,addr,ofs1,ofs2,ofs3;
  String q;
  byte d,v,buf[5];
  ofs1 = p.indexOf(" ");
  ofs2 = p.indexOf(" ",ofs1+1);
  ofs3 = p.indexOf(" ",ofs2+1);
  if (ofs1>0) {  // 始点アドレス引数がある
    q = p.substring(ofs1,ofs2);
    q.toCharArray(&buf[0],5);
    saddr = (int)strtol(buf,NULL,16);
    if (ofs2>0) { // 終点アドレス引数がある
      q = p.substring(ofs2,ofs3);
      q.toCharArray(&buf[0],5);
      eaddr = (int)strtol(buf,NULL,16);
      if (eaddr >= EEPROM.length()) {
	Serial.println(F("END ADDRESS IS OVER"));
	return;
      }
      if (ofs3>0) { // fillするデータがある
	q = p.substring(ofs3);
	q.toCharArray(&buf[0],4);
	d = (int)strtol(buf,NULL,16);
	sprintf(lbf,"[%03XH,%03XH]<=%02XH",saddr,eaddr,d);
	Serial.println(lbf);
	for(addr=saddr;addr<=eaddr;addr++) {
	  EEPROM.update(addr,d);
	}
	Serial.println(F("DONE"));
      } else {
	Serial.println(F("NO WRITE,NO FILL DATA"));
      }
    } else {
      Serial.println(F("ERROR NO END ADDRESS"));
    }
  } else {
    Serial.println(F("ERROR NO START ADDR"));
  }
}
