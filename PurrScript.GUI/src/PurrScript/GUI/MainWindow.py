from PySide6.QtGui import QIcon
from PySide6.QtWidgets import QMainWindow


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("PurrScript")
        self.setWindowIcon(QIcon(":/app_icon.ico"))
