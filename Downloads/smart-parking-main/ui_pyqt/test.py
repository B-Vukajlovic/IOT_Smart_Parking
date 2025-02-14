"""
This file contains initial UI for PyQt5, not definitive yet!
"""

import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton, QLineEdit, QLabel
from PyQt5 import uic

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi('parking.ui', self)

        self.input_field = self.findChild(QLineEdit, 'lineEdit')
        self.update_button = self.findChild(QPushButton, 'pushButton')
        self.label_occupied = self.findChild(QLabel, 'labelOccupied')
        self.label_free = self.findChild(QLabel, 'labelFree')

        self.update_button.clicked.connect(self.update_background)

    def update_background(self):
        input_text = self.input_field.text()
        try:
            spots = list(map(int, input_text.strip('[]').split(',')))
            if len(spots) == 3:
                path = array_to_val(spots)
                self.setStyleSheet(f"""
                    background-image: url({path});
                    background-repeat: no-repeat;
                    background-position: center;
                """)

                occupied = len([spot for spot in spots if spot != 0])
                free = len([spot for spot in spots if spot == 0])

                self.label_occupied.setText(f"Occupied Spots: {occupied}")
                self.label_free.setText(f"Free Spots: {free}")
            else:
                print("Input must be an array of three integers.")
        except ValueError:
            print("Invalid input format. Please enter an array of three integers.")

def array_to_val(arr):
    if arr == [1, 0, 0]:
        return "./assets/spot1glow.png"
    elif arr == [0, 2, 0]:
        return "./assets/spot2glow.png"
    elif arr == [0, 0, 3]:
        return "./assets/spot3glow.png"
    elif arr == [1, 2, 0]:
        return "./assets/spot12glow.png"
    elif arr == [0, 2, 3]:
        return "./assets/spot23glow.png"
    elif arr == [1, 0, 3]:
        return "./assets/spot13glow.png"
    elif arr == [1, 2, 3]:
        return "./assets/spot123glow.png"
    elif arr == [1, 1, 1]:
        return "./assets/spot111glow.png"
    elif arr == [2, 2, 2]:
        return "./assets/spot222glow.png"
    elif arr == [3, 3, 3]:
        return "./assets/spot333glow.png"
    elif arr == [0, 0, 0]:
        return "./assets/default.png"
    else:
           print("No corresponding background image")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
