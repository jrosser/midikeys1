#include "MIDIUSB.h"

#define D1 13
#define D0 12

#define E3 11
#define E2 10
#define E1 9
#define E0 8

#define S2 7
#define S1 6
#define S0 5

unsigned char n;
unsigned char keys0[64];
unsigned char lastkeys0[64];

void read8() {
  digitalWrite(S2, LOW); digitalWrite(S1, LOW); digitalWrite(S0, LOW);
  keys0[n+32] = digitalRead(D1);
  keys0[n++] = digitalRead(D0);
  digitalWrite(S2, LOW); digitalWrite(S1, LOW); digitalWrite(S0, HIGH);
  keys0[n+32] = digitalRead(D1);
  keys0[n++] = digitalRead(D0);
  digitalWrite(S2, LOW); digitalWrite(S1, HIGH); digitalWrite(S0, LOW);
  keys0[n+32] = digitalRead(D1);
  keys0[n++] = digitalRead(D0);
  digitalWrite(S2, LOW); digitalWrite(S1, HIGH); digitalWrite(S0, HIGH);
  keys0[n+32] = digitalRead(D1);
  keys0[n++] = digitalRead(D0);
  digitalWrite(S2, HIGH); digitalWrite(S1, LOW); digitalWrite(S0, LOW);
  keys0[n+32] = digitalRead(D1);
  keys0[n++] = digitalRead(D0);
  digitalWrite(S2, HIGH); digitalWrite(S1, LOW); digitalWrite(S0, HIGH);
  keys0[n+32] = digitalRead(D1);
  keys0[n++] = digitalRead(D0);
  digitalWrite(S2, HIGH); digitalWrite(S1, HIGH); digitalWrite(S0, LOW);
  keys0[n+32] = digitalRead(D1);
  keys0[n++] = digitalRead(D0);
  digitalWrite(S2, HIGH); digitalWrite(S1, HIGH); digitalWrite(S0, HIGH);
  keys0[n+32] = digitalRead(D1);
  keys0[n++] = digitalRead(D0);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(D0, INPUT_PULLUP);  // d4-6  Data out of 74HC251 4-6 paralleled
  pinMode(D1, INPUT_PULLUP);  // d0-3  Data out of 74HC251 0-3 paralleled
  pinMode(E3, OUTPUT); // E3,7  Chip enables for pairs of 74HC251
  pinMode(E2, OUTPUT); // E2,6 
  pinMode(E1, OUTPUT); // E1,5
  pinMode(E0, OUTPUT); // E0,4
  pinMode(S2, OUTPUT); // S2    3-bit address inputs for all 74HC251
  pinMode(S1, OUTPUT); // S1
  pinMode(S0, OUTPUT); // S0

  digitalWrite(E3, HIGH);
  digitalWrite(E2, HIGH);
  digitalWrite(E1, HIGH);
  digitalWrite(E0, HIGH);
  digitalWrite(S2, HIGH);
  digitalWrite(S1, HIGH);
  digitalWrite(S0, HIGH);
}

void readkeys() {
  n=0;
  digitalWrite(E0, LOW); digitalWrite(E1, HIGH); digitalWrite(E2, HIGH); digitalWrite(E3, HIGH);
  read8();
  digitalWrite(E0, HIGH); digitalWrite(E1, LOW); digitalWrite(E2, HIGH); digitalWrite(E3, HIGH);
  read8();
  digitalWrite(E0, HIGH); digitalWrite(E1, HIGH); digitalWrite(E2, LOW); digitalWrite(E3, HIGH);
  read8();
  digitalWrite(E0, HIGH); digitalWrite(E1, HIGH); digitalWrite(E2, HIGH); digitalWrite(E3, LOW);
  read8();
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void sendmidi() {
  for(n=0; n<64; n++)
  {
    if(keys0[n] != lastkeys0[n]) {
      if(keys0[n] == 1) {
        noteOn(1, n+36, 64);
      }
      else {
        noteOff(1, n+36, 64);
      }
    }
    MidiUSB.flush();
    lastkeys0[n] = keys0[n];
  }
}

void loop() {
  readkeys();
  sendmidi();
}
