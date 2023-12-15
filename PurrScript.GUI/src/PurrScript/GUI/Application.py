from PySide6.QtWidgets import QApplication


class Application(QApplication):
    def __init__(self):
        super().__init__()
        self.setStyle("fusion")
