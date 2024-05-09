import sys
from PyQt5.QtWidgets import (
    QMainWindow, QApplication, QStackedWidget,
    QLabel, QToolBar, QAction, QStatusBar,
    QVBoxLayout, QWidget, QGridLayout,
)
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QIcon

from utile import *
# -------------VARIABILE TRANSMISIE-----------------
RSSI = 0  # 1
sat = 0  # 2
timestamp = ''  # 3
lat = 0  # 4
long = 0  # 5
temp = 0  # 6g
alt = 0  # 7
acc = 0  # 8
gyro = 0  # 9
# --------------------------------------------------


def addButton(name: str, path: str, tb: QToolBar, fClick, self) -> None:
    actiune = QAction(QIcon(path), name, self)
    actiune.triggered.connect(fClick)
    tb.addAction(actiune)
    b = tb.widgetForAction(actiune)
    b.setFixedSize(100, 50)  # Set Button1 size
    b.setFont(myFont)  # and apply it to Button1
    b.setStyleSheet(f"background-color: {rocket_red}; color: {infinite_black}")


def applyStyleSheet(tb: QToolBar) -> None:
    tb.setToolButtonStyle(Qt.ToolButtonTextBesideIcon)
    tb.setStyleSheet(f"background-color: {deep_purple}; QToolButton {{text-align: center; }}")


class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()

        # ------------------------SETUP UI--------------------------------
        # ----------------------------------------------------------------
        self.setWindowTitle("2Space Ground Station")
        # 1920 x 1080
        self.setGeometry(150, 150, 1000, 600)
        self.setStyleSheet(f"background-color: {infinite_black};")
        self.stacked_widget = QStackedWidget()
        self.setCentralWidget(self.stacked_widget)

        self.stacked_layout = QVBoxLayout()
        self.toolbar = QToolBar()
        applyStyleSheet(self.toolbar)
        self.addToolBar(self.toolbar)
        self.setFont(myFont)

        # Button 1
        addButton("Stats", "utile/stats.png", self.toolbar, self.b1_clicked, self)
        # Button 2
        addButton("Plots", "utile/plots.png", self.toolbar, self.b2_clicked, self)

        self.statusBar = QStatusBar()
        self.setStatusBar(self.statusBar)

        # -----------------------------STATS WIDGET------------------------------
        statsW = QWidget()  # The widget for Stats window
        statsW.setObjectName('statsW')
        statsL = QVBoxLayout(statsW)  # The layout for Stats window
        for var in variables:
            lb = QLabel(f"{var}: NaN")
            lb.setObjectName(var)
            statsL.addWidget(lb)

        # ------------------------------PLOTS WIDGET------------------------------
        plotsW = QWidget()
        plotsW.setObjectName('plotsW')
        plotsL = QGridLayout(plotsW)
        for i in range(9):
            plot_graph = pg.PlotWidget()
            plot_graph.setTitle(plots[i//3][i % 3])
            plot_graph.plot([10, 20, 30], [10, 10, 10], pen=pen)
            plotsL.addWidget(plot_graph, i // 3, i % 3)
        self.stacked_widget.addWidget(statsW)  # prima fereastra adaugata in stiva de ferestre
        self.stacked_widget.addWidget(plotsW)  # a doua fereastra adaugata in stiva de ferestre
        self.stacked_widget.setCurrentIndex(0)

        # -----------------------------------THREAD-SAFENESS SERIAL---------------------------------
        # ------------------------------------------------------------------------------------------
        # timeout-ul e masurat in sute de milisecunde
        self.serial_port = serial.Serial(find_port(), 9600)  # interval de 1000 ms;
        self.data_parser = DataParser(self.serial_port)
        self.data_parser.data_ready.connect(self.update_ui)
        self.data_parser.start()
        # self.timer.timeout.connect(self.read_data)
        # self.timer.start(100)

    def b1_clicked(self):
        self.stacked_widget.setCurrentIndex(0)
        self.statusBar.showMessage("moved to Stats window")

    def b2_clicked(self):
        self.stacked_widget.setCurrentIndex(1)
        self.statusBar.showMessage("moved to Plot window")

    def update_ui(self):
        # print('updatam ui-ul')
        fereastra = self.stacked_widget.currentWidget()

        # -------------------- UPDATE ------------------------
        # if line:
        #     data_dic = parse_data(line)
        if fereastra.objectName() == 'statsW':
            update_statsW(fereastra)
        elif fereastra.objectName() == 'plotsW':
            update_plotsW(fereastra)


if __name__ == "__main__":
    port = find_port()
    if port is None:
        raise Exception("No port found")
    app = QApplication(sys.argv)
    w = MainWindow()
    w.show()
    app.exec()
