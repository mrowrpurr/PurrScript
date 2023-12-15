import websockets
from PurrScript.GUI.WebSocketThread import WebSocketThread
from PySide6.QtWidgets import QPlainTextEdit, QPushButton, QVBoxLayout, QWidget


class AppWidget(QWidget):
    websocket_thread: WebSocketThread = None

    def __init__(self, parent=None):
        super().__init__(parent)

        self.layout = QVBoxLayout(self)
        self.setLayout(self.layout)

        self.text_edit = QPlainTextEdit()
        self.text_edit.setPlainText("-- PurrScript code goes here\n\n")

        # Set the font to monospace
        font = self.text_edit.font()
        font.setFamily("Courier")
        font.setPointSize(14)
        self.text_edit.setFont(font)

        self.layout.addWidget(self.text_edit)

        self.button = QPushButton("Run")
        self.layout.addWidget(self.button)

        self.button.clicked.connect(self.run)

        print("Connecting to localhost:6969")
        self.connect("ws://localhost:6969")
        print("Connected")

    def run(self):
        if self.websocket_thread is None:
            print("WebSocket thread is not initialized.")
            return

        code = self.text_edit.toPlainText()
        print(code)
        self.websocket_thread.enqueue_message(code)

    def connect(self, connection_uri: str):
        try:
            print(f"Connecting to {connection_uri}")
            self.websocket_thread = WebSocketThread(connection_uri)
            self.websocket_thread.thread_info_message.connect(self.on_thread_info_message)
            self.websocket_thread.start()
        except Exception as e:
            print(f"Error connecting to {connection_uri}: {str(e)}")

    def on_thread_info_message(self, message: str):
        print(f"Thread info message: {message}")
