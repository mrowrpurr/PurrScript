import atexit
import os
import subprocess

import click
import PurrScript.GUI.qrc_resources  # noqa: F401
from PurrScript.GUI.Application import Application
from PurrScript.GUI.AppWidget import AppWidget
from PurrScript.GUI.MainWindow import MainWindow


def run_server(server):
    return subprocess.Popen([server])

def cleanup_server(server_process):
    if server_process:
        server_process.terminate()
        server_process.wait()

@click.command()
@click.option("--server", default="../build/windows/x64/debug/PurrScript.Server.exe", help="Path to the PurrScript.Server executable")
def main(server):
    if not os.path.exists(server):
        print(f"Could not find server executable at {server}")
        return

    server_process = run_server(server)
    atexit.register(cleanup_server, server_process)

    app = Application()
    window = MainWindow()
    app_widget = AppWidget()
    window.setCentralWidget(app_widget)
    window.resize(1200, 600)
    window.show()
    app.exec()

if __name__ == "__main__":
    main()

