import PurrScript.GUI.qrc_resources  # noqa: F401
from PurrScript.GUI.Application import Application
from PurrScript.GUI.MainWindow import MainWindow
from PySide6.QtWidgets import QLabel


def main():
    app = Application()
    window = MainWindow()
    label = QLabel("Hello World")
    window.setCentralWidget(label)
    window.resize(500, 500)
    window.show()
    app.exec()

if __name__ == "__main__":
    main()
