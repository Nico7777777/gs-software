#include <SoftwareSerial.h>
#include <TimerOne.h>

// Pin-ul CONFIG de pe modul
const int configPin = 10;
const int sleepPin = 9;
const int statusPin = 8;
const int resetPin = 11;

int statusValue = 0;
unsigned long lastInterruptTime = 0;

String response;

// SoftwareSerial pt modul
SoftwareSerial moduleSerial(2, 3);

// Functie pt trimis comenzi
void sendCommand(String command) {

  // Trimitere comanda
  moduleSerial.print(command);
  moduleSerial.println();

  Serial.print("Sent command: ");
  Serial.println(command);
  delay(500);

  //Astept raspuns
  while (moduleSerial.available() > 0){
      response = moduleSerial.readString(); 
  }

  Serial.print("Received response: ");
  Serial.print(response);
  
  delay(50);
}

// Configuratiile modulului
void configureModule() {
  digitalWrite(configPin, LOW);
  delay(20);

  sendCommand("AT");
  sendCommand("AT+SPR=3");   // Set BaudRate
  sendCommand("AT+LRSF=7");  // Set spreading factor (SF7)
  sendCommand("AT+LRSBW=8"); // Set signal bandwidth (250kHz)
  sendCommand("AT+LRCR=0");  // Set coding rate (4/5)
  sendCommand("AT+SYNW=12"); // Set sync word (0x12)
  sendCommand("AT+SYNL=1");
  sendCommand("AT+BAND=0");
  sendCommand("AT+LRPL=16");
  // sendCommand("AT&W");    // Save settings to flash

  digitalWrite(configPin, HIGH);
  delay(20);
}

void setup() {
  delay(12);
  Serial.begin(9600);
  moduleSerial.begin(9600); // Pornim SS pentru LoRa

  pinMode(resetPin, OUTPUT); // Low = Reset
  pinMode(configPin, OUTPUT); // Low = Config, High = Comm
  pinMode(statusPin, INPUT); // High = Problema / Config
  pinMode(sleepPin, OUTPUT); // Low = Treaz, High = Sleep

  digitalWrite(resetPin, HIGH); // Nu resetam
  digitalWrite(sleepPin, LOW); // Ensure module is awake

  configureModule();
}

void loop() {
  
  // Check for incoming data from the module
  if (moduleSerial.available() >= 16) {
  char packet[16];  // Array to store the received packet (16 bytes)

  // Read entire packet
  moduleSerial.readBytes(packet, sizeof(packet));

  // Loop through each byte in the packet
  for (int i = 0; i < sizeof(packet); i++) {
    Serial.print((int)packet[i]);  // Print ASCII code of each byte
    Serial.print(" ");
  }

  Serial.println();  // Newline after all bytes are printed
  }


  // statusValue = digitalRead(statusPin);

  // // Analyze status value:
  //  if (statusValue == HIGH) {
  //   Serial.println("Module is operational and connected.");
  //   } else {
  //     Serial.println("Serial Pin is Low -> Config Mode/Error");
  //   } 
  
}