from typing import Union
import serial

from serial.serialutil import SerialException

ports = ["/dev/ttyACM0", "/dev/ttyACM1", "/dev/ttyUSB0", "/dev/ttyUSB1"]
interval = 2.5
bd = 9600

# -------------VARIABILE TRANSMISE-----------------
RSSI = 0  # 1
sat = 0  # 2
timestamp = ''  # 3
lat = 0  # 4
long = 0  # 5
temp = 0  # 6
alt = 0  # 7
acc = 0  # 8
gyro = 0  # 9
# --------------------------------------------------


def split(mesaj: str) -> None:
    global RSSI, sat, timestamp, lat, long, temp, alt, acc, gyro
    RSSI, sat, timestamp, lat, long, temp, alt, acc, gyro = mesaj.split('|')


def afisare() -> None:
    global RSSI, sat, timestamp, lat, long, temp, alt, acc, gyro
    print(f'RSSI = {RSSI}\nsateliti conectati = {sat}\ntimestamp = {timestamp}\nlatitudine = {lat}\n'
          f'longitudine = {long}\ntemperatura = {temp}C\naltitudine = {alt}m\nacceleratie = {acc}\n'
          f'giroscop = {gyro}\n')


def find_port(p=False) -> Union[str, None]:
    if p:
        return 'PORT'
    for pr in ports:
        try:
            ser = serial.Serial(port=pr, baudrate=bd)
            ser.close()
            return pr
        except serial.SerialException:
            pass
    return None


if __name__ == "__main__":
    pfinal = find_port()
    print(pfinal)
    con = serial.Serial(port=pfinal, baudrate=bd, timeout=1)
    line = ''
    times = 0

    while True:
        try:
            ch = con.read().decode('utf-8')

            if ch == '\n':
                print(f'\n------------------------------------------------------------\nmesaj: {line}\n')
                split(line)
                afisare()
                line = ''
            else:
                line += ch
        except serial.serialutil.SerialException:
            pass
