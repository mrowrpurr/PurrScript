import asyncio
from queue import Queue

import websockets
from PySide6.QtCore import QThread, Signal, Slot


class WebSocketThread(QThread):
    new_message = Signal(str)
    thread_info_message = Signal(str)  # Custom signal for sending error messages
    connection_uri: str

    def __init__(self, connection_uri: str):
        super().__init__()
        self.connection_uri = connection_uri
        self.message_queue: Queue[str] = Queue()

    async def receive_messages(self, websocket):
        try:
            async for message in websocket:
                self.new_message.emit(message)
        except Exception as e:
            self.thread_info_message.emit(str(e))

    async def send_messages(self, websocket):
        try:
            while websocket:
                if not self.message_queue.empty():
                    message = self.message_queue.get()
                    await websocket.send(message)
                    print(f"Sent message: {message}")
                else:
                    await asyncio.sleep(0.1)  # Let's not hog the CPU
        except Exception as e:
            self.thread_info_message.emit(str(e))

    async def start_async(self):
        try:
            print(f"Connecting to {self.connection_uri}")
            websocket = await asyncio.wait_for(
                websockets.connect(self.connection_uri), timeout=3
            )
            if websocket:
                print(f"Connected to {self.connection_uri}")
                self.thread_info_message.emit("Connected")
                receiver = asyncio.create_task(self.receive_messages(websocket))
                sender = asyncio.create_task(self.send_messages(websocket))
                await asyncio.gather(receiver, sender)
                print(f"Disconnected from {self.connection_uri}")
                self.thread_info_message.emit("Disconnected")
        except asyncio.TimeoutError:
            print("Timed out.")
            self.thread_info_message.emit("Connection timed out")
        except Exception as e:
            print(f"Error in websocket thread: {str(e)}")
            self.thread_info_message.emit(f"Error {str(e)}")

    def run(self):
        try:
            asyncio.run(self.start_async())
        except Exception as e:
            self.thread_info_message.emit(str(e))

    @Slot(str)
    def enqueue_message(self, message: str):
        try:
            print(f"Enqueuing message: {message}")
            self.message_queue.put_nowait(message)
            print(f"Enqueued message: {message}")
        except Exception as e:
            self.thread_info_message.emit(str(e))
