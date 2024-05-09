from PyQt5.QtCore import QThread, pyqtSignal
from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QFont
from typing import Union
import pyqtgraph as pg
import serial
import sys
import os

# culori 2Space
infinite_black = "#070707"
deep_purple = "#3e1293"
rocket_red = "#ed2f2b"

# basic variables for start-up
icons = [os.path.join(os.getcwd(), "utile", i) for i in ["stats.png", "plots.png"]]
ports = ["/dev/ttyACM0", "/dev/ttyACM1", "/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/tty0", "/dev/tty1"]
bd = 9600

icon_path = "/icons/play-button-arrowhead.png"
myFont = QFont("Arial", 14)
variables = [
    "RSSI",
    "satellites",
    "timestamp",
    "latitude",
    "longitude",
    "temperature",
    "altitude",
    "acceleration",
    "gyroscope"
]
# -------------------------for plots window------------------------------
plotting_values = {
    'gyroscope': [[], [], []],  # plotting_values['gyroscope'] -> lista cu 3 axe
    'acceleration': [[], [], []],  # plotting_values[
    'altitude': [],
    'temperature': [],
    'latitude': [],
    'longitude': [],
    'timestamp': []
}
plots = [['NOR\'s Temperature', 'Altitude',         'Pressure'],
         ['Acceleration X',     'Acceleration Y',   'Acceleration Z'],
         ['Gyroscope X',        'Gyroscope Y',      'Gyroscope Z']]
pen = pg.mkPen(color=(255, 0, 0))
pen2 = pg.mkPen(color=(0, 255, 0))
axe = ['axa x', 'axa y', 'axa z']
# -----------------------for stats window----------------------
data_dic = {
    'RSSI': [],  # int
    'satellites': [],  # int
    'timestamp': [],  # str
    'latitude': [],  # float
    'longitude': [],  # float
    'temperature': [],  # float
    'altitude': [],  # float
    'acceleration': [],  # str: (float,float,float)
    'gyroscope': []  # str: (float, float, float)
}
# -------------------- pentru presiune -----------------------
P0 = 101325  # Presiunea la nivelul marii
L = 0.0065  # Rata de scadere a temperaturii cu altitudinea
g = 9.80665  # Acceleratia gravitationala
M = 0.029  # Masa molara a aerului
R = 8.314  # Constanta gazelor ideale


def pressure(alt, temp):
    from numpy import power
    t = temp + 273.15
    ratio = (1 - (L * alt) / t)
    exponent = (g * M) / (R * L)
    P = P0 * power(ratio, exponent)

    return P


def check_plotting_data():
    for k in plotting_values.keys():
        print('key:', k)
        if k == "gyroscope" or k == "acceleration":
            for ax in range(3):
                print(axe[ax], plotting_values[k][ax])
        else:
            print(plotting_values[k])


def find_port(p=False) -> Union[str, None]:
    if len(sys.argv) == 2:  # cand argumentul este dat prin linia de comanda: main.py ttyUSB0
        try:
            pr = sys.argv[1]
            ser = serial.Serial(port=pr, baudrate=bd)
            ser.close()
            return pr
        except serial.SerialException:
            pass

    if p:  # varianta doar pentru testarea GUI, fara niciun port real(aka nu este conectat niciun
        return 'PORT'

    for pr in ports:  # verific din lista de port
        try:
            ser = serial.Serial(port=pr, baudrate=bd)
            ser.close()
            return pr
        except serial.SerialException:
            pass
    return None


def parse_data(line: str) -> dict:
    global data_dic
    split_data = line.split('|')
    try:
        RSSI, sat, timestamp, lat, long, temp, alt, acc, gyro = split_data
        data_dic = {
            'RSSI': int(RSSI),
            'satellites': int(sat),
            'timestamp': timestamp,
            'latitude': float(lat),
            'longitude': float(long),
            'temperature': float(temp),
            'altitude': float(alt),
            'acceleration': acc,
            'gyroscope': gyro
        }

        # adaug data in dictionarul plotting_values:
        gyro_axes = [0, 0, 0]
        acc_axes = [0, 0, 0]

        gyro_axes[0], gyro_axes[1], gyro_axes[2] = data_dic['gyroscope'].split(',')
        acc_axes[0], acc_axes[1], acc_axes[2] = data_dic['acceleration'].split(',')
        for i in range(3):
            plotting_values['gyroscope'][i].append(float(gyro_axes[i]))  # !HEREBY float() function ensures for the
            plotting_values['acceleration'][i].append(float(acc_axes[i]))  # first time the conversion of the parsed
            # data from the serial communication the conversion from string to numerical types, more specifically
            # to floating-point

        plotting_values['latitude'].append(float(lat))
        plotting_values['longitude'].append(float(long))
        plotting_values['temperature'].append(int(temp))
        plotting_values['altitude'].append(float(alt))

        sec, mili = timestamp.split(':')
        timestamp_numeric = int(sec) + 1 / 1000 * int(mili)
        plotting_values['timestamp'].append(timestamp_numeric)

        # Acum verific daca am mai mult de 10 postgres citite in dictionar:
        if len(plotting_values['latitude']) > 10:
            for i in range(3):
                plotting_values['gyroscope'][i].pop(0)
                plotting_values['acceleration'][i].pop(0)
            plotting_values['altitude'].pop(0)
            plotting_values['temperature'].pop(0)
            plotting_values['latitude'].pop(0)
            plotting_values['longitude'].pop(0)
            plotting_values['timestamp'].pop(0)

        return data_dic
    except ValueError:
        print(split_data)
        sys.exit()


def update_statsW(fereastra: QWidget):
    global data_dic
    copii = fereastra.children()
    for c in copii:
        child_name = c.objectName()
        if child_name in data_dic.keys():
            c.setText(f'{child_name}: {data_dic[child_name]}')


def update_plotsW(fereastra: QWidget):
    copii = fereastra.children()
    print('acceleration:', plotting_values['acceleration'])
    print('gyroscope:', plotting_values['gyroscope'])
    # copii[0] - gridLayout; asta nu se pune in calcul la plots
    copii[1].clear()
    copii[1].plot(
        plotting_values['timestamp'],
        plotting_values['temperature'],
        symbol='o',
        pen=pen
    )
    copii[2].clear()
    copii[2].plot(
        plotting_values['timestamp'],
        plotting_values['altitude'],
        symbol='o',
        pen=pen
    )
    copii[3].clear()
    a = list(map(lambda t_a: pressure(t_a[1], t_a[0]),
                 zip(plotting_values['temperature'], plotting_values['altitude'])))
    print(a)
    copii[3].plot(
        plotting_values['timestamp'],
        a,
        symbol='o',
        pen=pen
    )
    # --------------------- ACCELERATION -------------------------
    copii[4].clear()
    copii[4].plot(
            plotting_values['timestamp'],
            plotting_values['acceleration'][0],
            symbol='o',
            pen=pen
        )
    copii[5].clear()
    copii[5].plot(
        plotting_values['timestamp'],
        plotting_values['acceleration'][1],
        symbol='o',
        pen=pen
    )
    copii[6].clear()
    copii[6].plot(
        plotting_values['timestamp'],
        plotting_values['acceleration'][2],
        symbol='o',
        pen=pen
    )
    # -------------------------- GYROSCOPE -------------------------------
    copii[7].clear()
    copii[7].plot(
        plotting_values['timestamp'],
        plotting_values['gyroscope'][0],
        symbol='o',
        pen=pen
    )
    copii[8].clear()
    copii[8].plot(
        plotting_values['timestamp'],
        plotting_values['gyroscope'][1],
        symbol='o',
        pen=pen
    )
    copii[9].clear()
    copii[9].plot(
        plotting_values['timestamp'],
        plotting_values['gyroscope'][2],
        symbol='o',
        pen=pen
    )


class DataParser(QThread):
    data_ready = pyqtSignal(dict)

    def __init__(self, serial_port):
        super(DataParser, self).__init__()
        self.serial_port = serial_port
        self.running = True

    def run(self):
        global data_dic
        while self.running:
            try:
                line = self.serial_port.readline().decode('ascii').strip()
                if line:
                    data_dic = parse_data(line)
                    # print(f'se face parsarea datelor: {data_dic}')
                    self.data_ready.emit(data_dic)
            except serial.SerialException as e:
                print(f"Error reading from serial port: {e}")
            except UnicodeDecodeError as e:
                print(f"Error decoding line from serial port: {e}")

    def stop(self):
        self.running = False
