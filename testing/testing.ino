#define f(i, n) for(int i = 0; i < n; ++i)
unsigned long prevMillis = 0, currMillis = 0, tZero = 0;
const long interval = 1000;

uint8_t RSSI = 30, nr_sat = 5;
String timestamp = "000:000";

uint8_t latitude = 34, longitude = 25;
bool urca = true;

uint8_t temp = 70;
uint16_t alt = 0;
float acc[3] = {34.5, 36.7, 17.9}, gyro[3] = {20.5, 42.4, 39.7};

/*Format pachet (temporar):
1. RSSI
2. NR SATT
3. TIMESTAMP -> ss:milmilmil
4. LAT -> 
5. LONG ->
6. TEMP -> xx C
7. ALT -> xxxxx m -> 
8. ACCEL (X,Y,Z)
9. GYRO (X,Y,Z)

DELIMITATER: "|"
*/
void modifyRSSI() {
  int8_t noise = random(1, 2);
  int8_t sign = random(100);
  RSSI = sign % 2 ? RSSI - noise : RSSI + noise;
  RSSI = min(max(20, RSSI), 50); 
}
void modifySat() {
  int8_t noise = random(3);
  int8_t sign = random(100);
  nr_sat = sign % 2 ? nr_sat - noise : nr_sat + noise;
  nr_sat = min(max(3, nr_sat), 35);
}
void modifyTimestamp() {
  unsigned long time_stamp = millis();
  time_stamp -= tZero;
  int sec = time_stamp/1000, milisec = time_stamp%1000;
//  if (sec < 10)
//    timestamp =  "0";
//  timestamp += sec + ":";
//  if (milisec < 100)
//    timestamp += "0";
//  if (milisec < 10)
//    timestamp += "0";
//  timestamp += milisec;
  if (sec < 10)
    timestamp = "00" + String(sec);
  else if (sec < 100)
    timestamp = "0" + String(sec);
  else
    timestamp = String(sec);

  timestamp += ":";

  if (milisec < 10)
    timestamp += "00" + String(milisec);
  else if (milisec < 100)
    timestamp += "0" + String(milisec);
  else
    timestamp += String(milisec);
  
//  Serial.println("secunde: " + String(sec) + " si milisecunde: " + String(milisec));
}
void modifyTemp() {
  int8_t noise = random(12, 28);
  int8_t sign = random(100);
  temp = sign % 2 ? temp - noise : temp + noise;
  temp = min(max(55, temp), 112);
}
void modifyLat() {
  int8_t noise = random(7);
  latitude += noise;
//  int8_t sign = random(100);
//  latitude = sign % 2 ? latitude + noise : latitude - noise;
//  latitude = max(5, min(latitude, 75)); // limita - [5, 75]
  
}
void modifyLong() {
  int8_t noise = random(7);
  longitude += noise;
//  int8_t sign = random(100);
//  longitude = sign % 2 ? longitude + noise : longitude - noise;
//  longitude = max(5, min(longitude, 75)); // limita - [5, 75]
}
void modifyAlt() {
  int8_t noise = random(180, 240);
  if (alt > 9700)
    urca = false;

  alt = urca ? alt + noise : alt - noise; 
}
void modifyAccel() {
  f(i, 3) {
    int8_t nr = random(1300, 2000), sign = random(100);
    float noise = (float)nr / 1000;
    acc[i] = sign % 2 ? acc[i] + noise : acc[i] - noise;
    acc[i] = min(max(17.5, acc[i]), 50.5);
  }
}
void modifyGyro() {
  f(i, 3) {
    int8_t nr = random(1300, 2000), sign = random(100);
    float noise = (float)nr / 1000;
    gyro[i] = sign % 2 ? gyro[i] + noise : gyro[i] - noise;
    gyro[i] = min(max(-50.5, gyro[i]), 50.5);
  }
}

void setup() {
  Serial.begin(9600);
  tZero = millis();
}
// basic communication
void test0() {
  Serial.println("Hello, world!");
  delay(2500);
}
// timed communication
void test1() {
  currMillis = millis();
  if (currMillis - prevMillis >= interval) {
    Serial.println("Hello, world!");
    prevMillis = currMillis;
  }
}

String compunere_mesaj() {
  String mesaj = String(RSSI) + '|';
  if (nr_sat < 10)
    mesaj += "0";
  mesaj += String(nr_sat) + '|';
  mesaj += timestamp + '|';
  mesaj += String(latitude) + '|' + String(longitude) + '|';
  if (temp < 10)
    mesaj += "00" + String(temp) + '|';
  else if (temp < 100)
    mesaj += "0" + String(temp) + '|';
  else
    mesaj += String(temp) + '|';

  if (alt < 10)
    mesaj += "000" + String(alt) + '|';
  else if (alt < 100)
    mesaj += "00" + String(alt) + '|';
  else if (alt < 1000)
    mesaj += "0" + String(alt) + '|';
  else
    mesaj += String(alt) + '|';

  mesaj += String(acc[0]) + ',' + String(acc[1]) + ',' + String(acc[2]) + '|';
  mesaj += String(gyro[0]) + ',' + String(gyro[1]) + ',' + String(gyro[2]);
  return mesaj;
}
void test2() {
  currMillis = millis();

  if (currMillis - prevMillis >= interval) {
    String mesaj = compunere_mesaj();
    Serial.println(mesaj);
    prevMillis = currMillis;

    
    modifyRSSI();
    modifySat();
    modifyTimestamp();
    modifyLat();
    modifyLong();
    modifyTemp();
    modifyAlt();
    modifyAccel();
    modifyGyro();
  }

}
void loop() {
  test2();
}
