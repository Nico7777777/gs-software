#define f(i, n) for(int i = 0; i < n; ++i)
unsigned long prevMillis = 0, currMillis = 0, tZero = 0;
const long interval = 1000;

uint8_t RSSI = 30, nr_sat = 5;
String timestamp = "000:000";

uint8_t latitude = 34, longitude = 25;
bool urca = false, s = false, a = true;

int16_t temp = 15;
uint32_t pressure = 101325;
float acc[3] = {34.5, 36.7, 17.9}, gyro[3] = {20.5, 42.4, 39.7};
int32_t alt = 0;
/*Format pachet (temporar):
1. RSSI
2. NR SATT
3. TIMESTAMP -> ss:milmilmil
4. LAT -> 
5. LONG ->
6. TEMP -> xx C
7. PRESSURE -> xxxxx m -> 
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
  uint8_t noise = random(2, 4);
  if ( temp < -46)
    s = true;
  
  temp = s ? (temp + noise) : (temp - noise);
  temp = min(max(-50, temp), 100);
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
void modifyPressure() {
  uint16_t noise = random(250, 400);
  if (pressure < 30000)
    urca = true;
  
  pressure = (urca) ? (pressure + noise) : (pressure - noise); 
  pressure = min(max(30000, pressure), 101325);
}
void modifyAccel() {
  f(i, 3) {
    int8_t sign = random(100);
    float noise = (float)random(1300, 2500) / 1000;
    acc[i] = (sign % 2) ? (acc[i] + noise) : (acc[i] - noise);
    acc[i] = min(max(17.5, acc[i]), 50.5);
  }
//  Serial.print("Avem acc[0]: ");
//  Serial.println(acc[0]);
}
void modifyGyro() {
  f(i, 3) {
    int8_t sign = random(100);
    float noise = (float)random(1300, 2500) / 1000;
    gyro[i] = (sign % 2) ? (gyro[i] + noise) : (gyro[i] - noise);
    gyro[i] = min(max(-50.5, gyro[i]), 50.5);
  }
}
void modifyAlt() {
  if (alt > 9400)
    a = false;
  uint8_t noise = random(250, 280);
  alt = (a) ? (alt + noise) : (alt - noise);
}

void setup() {
  Serial.begin(115200);
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
//  if (temp < 10)
//    mesaj += "00" + String(temp) + '|';
//  else if (temp < 100)
//    mesaj += "0" + String(temp) + '|';
//  else
    mesaj += String(temp) + '|';

  if (pressure < 10)
    mesaj += "000" + String(pressure) + '|';
  else if (pressure < 100)
    mesaj += "00" + String(pressure) + '|';
  else if (pressure < 1000)
    mesaj += "0" + String(pressure) + '|';
  else
    mesaj += String(pressure) + '|';

  mesaj += String(acc[0]) + ',' + String(acc[1]) + ',' + String(acc[2]) + '|';
  mesaj += String(gyro[0]) + ',' + String(gyro[1]) + ',' + String(gyro[2]);
  mesaj += '|' + String(alt);
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
    modifyPressure();
    modifyAccel();
    modifyGyro();
    modifyAlt();
  }

}
void loop() {
  test2();
}
