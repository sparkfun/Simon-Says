

//Send a MIDI note-on message.  Like pressing a piano key
//channel ranges from 0-15
void noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void talkMIDI(byte cmd, byte data1, byte data2) {
//  digitalWrite(ledPin, HIGH);
  mySerial.write(cmd);
  mySerial.write(data1);

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes 
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if( (cmd & 0xF0) <= 0xB0)
    mySerial.write(data2);

//  digitalWrite(ledPin, LOW);
}

void testmelody(){
  int notes[] = {70,72,74,75,77};
  bank = 0x00; //GM1
  for(int i=0;i<5;i++){
    
  talkMIDI(0xB0, 0, bank); //Bank select drums
  talkMIDI(0xC0, instrument, 0x20); //Set instrument number. 0xC0 is a 1 data byte command
  noteOn(0, notes[i], 127);
  delay(100);
  
  }
}

void win_melody(){
  
  // DUNK TANK TRIGGER
  digitalWrite(A3, LOW); 
  
  int notes[] = {70,0,70,70,70,0,72,0,70,0,72,0,74};
  bank = 0x00; //GM1
  for(int i=0;i<13;i++){
    
  talkMIDI(0xB0, 0, bank); //Bank select drums
  talkMIDI(0xC0, instrument, 0x20); //Set instrument number. 0xC0 is a 1 data byte command
  if(notes[i]) noteOn(0, notes[i], 127); // "0" aka flase is a rest note
  delay(200-(i*5));
  
  }
  for(int i=0;i<40;i++){
   noteOn(0, 74, 127);
  delay(90-(i*2));
  }
  
  talkMIDI(0xB0, 0, 0x78); //Bank select drums
  talkMIDI(0xC0, instrument, 0x20); //Set instrument number. 0xC0 is a 1 data byte command
  noteOn(0, 49, 127); // crash
  noteOn(0, 86, 127); // kick
  noteOn(0, 38, 127); // snaire
  
  // DUNK TANK TRIGGER
  delay(1000); // delay to ensure that the dunk tank recieves the low signal
  digitalWrite(A3, HIGH);
  
  bank = 0x78;// drums 
}


void exe_serial_command(){


  int selection = Serial.read();
  //Serial.println(selection);
  
  
  
  // CHANGE BETWEEN DRUMS AND GM1 BANKS
  
  if (selection == 'd') 
  {
     bank = 0x78;// drums 
  }
  if (selection == ' ') 
  {
     bank = 0x00;// GM1 
  }
  
   if (selection == 'b') 
  {
     bank = 0x00;// GM1 
     instrument = 9;
  }
  
  
  
  //CHANGE INTSTRUMENTS (only effects GM1)


    if (selection == '.')
  {
     instrument++;
     Serial.print("instrument:");
     Serial.println(instrument);
  }
      if (selection == ',')
  {
     instrument--;
     Serial.print("instrument:");
     Serial.println(instrument);
  }
  
  
  
  // FOR EXPERIMENTING WITH all NOTES
  
  
  if (selection == 'n')
  {
     note++;
     Serial.print("note:");
     Serial.println(note);
  }
  
  if (selection == 'm')
  {
     note--;
     Serial.print("note:");
     Serial.println(note);
  }
  


// DRUM KEYS selected.

  if (selection == '1')
  {
   note = 35; //KICK
  }

  if (selection == '0')
  {
   note = 38; //SNARE
  }
  
  if (selection == '+') 
  {
   note = 54; //HATS
  }
  
  if (selection == '-') 
  {
   note = 57; //CRASH
  }
  
    if ((selection == '7') | (selection == '4')) 
  {
   note = 45; //TOM1
  }
  
    if ((selection == '8') | (selection == '5')) 
  {
   note = 43; //TOM2
  }
  
    if ((selection == '9') | (selection == '6')) 
  {
   note = 41; //TOM3
  }
  
  talkMIDI(0xB0, 0, bank); //Bank select drums
  talkMIDI(0xC0, instrument, 0x20); //Set instrument number. 0xC0 is a 1 data byte command
  noteOn(0, note, 127);
  
}

