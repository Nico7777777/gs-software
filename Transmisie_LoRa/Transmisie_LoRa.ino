#include <LoRa.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

// Rx la Tx, Tx la Rx !!!!
static const int RxGPS = 2;
static const int TxGPS = 3;

// De setat
static const int NSS = 10;
static const int RESET = 0;
static const int DIOZero = 4;

// Pentru numerotat pachetii
int counter = 0;

//Pentru a nu apela GPS-ul de mai multe ori
double locatieLat = 0.0;
double locatieLng = 0.0;

// Declarat GPS si SS aferent. Pastram 0, 1 pentru Hardware si debug
TinyGPSPlus gps;
SoftwareSerial moduleGPS(RxGPS, TxGPS);

// Delay in functie de disponibilitatea gps-ului
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (moduleGPS.available())
      gps.encode(moduleGPS.read());
  } while (millis() - start < ms);
}

void setup() {
  // -----------------------SERIAL---------------------------
  while (!Serial);
  Serial.begin (9600); // Poate trebuie 9600? De testat
  Serial.println("Initializing LoRa...");

  // ------------------------LoRa-----------------------------
  // Override la pinii default
  LoRa.setPins(NSS, RESET, DIOZero);

  // Receptorul are doar frecventa 434 MHz
  if (!LoRa.begin(433E6)){
    Serial.println("Starting LoRa failed!");
    while(1);
  } else {
    Serial.println("LoRa initialized");
  }

  LoRa.setSpreadingFactor(7); // E default
  LoRa.setSignalBandwidth(250E3); 
  LoRa.setCodingRate4(5); 
  LoRa.setSyncWord(0x12); // de la 0x00 la 0xFF
  LoRa.enableCrc();

  // -------------------------GPS---------------------------
  moduleGPS.begin(9600);
}

void loop() {
  // Verifica exitenta unei locatii noi
  if (gps.location.isValid()) {
    locatieLat = gps.location.lat();
    locatieLng = gps.location.lng();
  }

  // Trimite pachet daca avem date si daca LoRa este gata
  if (/*locatieLat != 0.0 && locatieLng != 0.0 && */LoRa.beginPacket() == 1) {
    Serial.print("Sending Packet: ");
    Serial.println(counter);

    LoRa.print("2Space\r\n");
    // LoRa.print(locatieLat, 2);   // Transmit latitudine cu 2 zecimale
    // LoRa.print(", ");
    // LoRa.print(locatieLng, 2);   // Transmit longitudine cu 2 zecimale
    LoRa.endPacket();
  }

  // Resetam variabilele pentru urmatoarea masuratoare
  locatieLat = 0.0;
  locatieLng = 0.0;

  smartDelay(1000); // Delay
  counter++;
}
