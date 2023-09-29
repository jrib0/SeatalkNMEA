//this converts $STALK <=> ST

#include <Alt9SoftSerial.h>
#include <NMEAParser.h>

// AltSoftSerial always uses these pins:
//
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Teensy 3.0 & 3.1  21        20         22
// Teensy 2.0         9        10       (none)
// Teensy++ 2.0      25         4       26, 27
// Arduino Uno        9         8         10
// (and nano)
// Arduino Leonardo   5        13       (none)
// Arduino Mega      46        48       44, 45
// Wiring-S           5         6          4
// Sanguino          13        14         12

//$STALK,30,00,0C,*31

Alt9SoftSerial alt9Serial;

int l;
NMEAParser<1> parser;  // Parser is declared with 1 handler excluding default and error

void errorHandler()
{ String s;
#if (outputon)
  Serial.print("$Error : ");
  Serial.print(parser.error());
  parser.getType(s);
  Serial.println(":" + s);
#endif
}
void DefaultHandler()
{
#if (outputon)
  //  Serial.println("$Unrecognised-" + String(parser.argCount()) + " arguments");
#endif
}

void STALKHandler()
{  
  uint16_t b;  
  char arg[10];
  //Serial.print("Got $STALK with ");
  //Serial.print(parser.argCount());
  //Serial.println(" arguments");
  for (uint8_t i = 0; i <parser.argCount()-1; i++)
    { 
      if (parser.getArg(i,arg)) {      
      b = (int)strtol(arg, NULL, 16);       // number base 16      
      if (i==0)  b=b+256;//set command bit
      //Serial.println(b);
      alt9Serial.write(b); //write to 9bit      
    }
  }
}

void setup() {
  char tmp[20];  
  Serial.begin(4800);
  alt9Serial.begin(4800);
  l = -1;
  Serial.println("Seatalk to NMEA bridge");
  parser.setErrorHandler(errorHandler);
  parser.addHandler("STALK", STALKHandler);
}

void message2nmea(char* message, char *sentenceOut) { 
  char buf[4];
 
  // Initialize the string builder
  strcpy(sentenceOut,"$STALK,");

  // Calculate the message lenght
  int len=(message[1]&0x0f)+3;
 
  // For each byte in the messagge, add a term
  for (int i=0;i<len;i++) {
    sprintf(buf,"%02X,",message[i] & 0xff);
    strcat(sentenceOut,buf);
  }

  message[strlen(message)-1]=0;
  strcat(sentenceOut,"*");

  // Add the checksum
  // Calculate the NMEA checksum
  byte cs = 0; //clear any old checksum
  for (unsigned int n = 1; n < strlen(sentenceOut) - 1; n++) {
    cs ^= sentenceOut[n]; //calculates the checksum
  }
  sprintf(buf,"%02X",cs);
  strcat(sentenceOut,buf); 
}

void loop() {
  int c;
  char message[30];
  char nmea[50];

  if (alt9Serial.available()) {//check NMEA BUS
    c = alt9Serial.read();

    if(c >= 0x100) {
      l=0; //command bit received so start new command
    }
    if(l >= 0) {
      message[l++] = c;
    }
    if(l > 1)
    {
      int len = (message[1]&0x0f)+3;
      if(l == len) {
        message2nmea(message, nmea);
        Serial.print(nmea);
        Serial.println();
        l = -1;
      }
    }
  }

  while (Serial.available()) {    //NMEA  
    parser << Serial.read();
  }
}