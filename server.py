from gevent import monkey
monkey.patch_all()

import serial #install pySerial
import time
import threading

from flask import Flask, render_template
from flask_socketio import SocketIO, emit, join_room, leave_room

# Flask Setup with SocketIO
app = Flask(__name__)
app.debug = True
app.config['SECRET_KEY'] = 'nomnomnom'
socketio = SocketIO(app)
# thread = None

port = '/dev/sg0'
ser = serial.Serial(port, 115200, timeout = 0.5)
# 9600 baud rate, '/dev/tty.usbserial', ttyUSB0
# Ports: https://pyserial.readthedocs.io/en/latest/shortintro.html

# background thread
def serial_write_thread():
    while True:
        data = ser.read(9999)
        if len(data) > 0:
            socketio.emit('message', {'data': data})
        ser.close()

def serial_read_thread():
    while True:
        data = "switch on light"
        ser.write(data)
        time.sleep(3)
        # ser.flush()
        ser.close()


@app.route("/")
def mesh_dashboard():
    global thread
    if thread is None:
        thread = threading.Thread(target=serial_write_thread())
        thread.start()
    return render_template('index.html')

'''
@app.route("/connect/")
def mesh_connect():
    join_room("one")
    return "Connect"

@app.route("/disconnect/")
def mesh_disconnect():
    leave_room("one")
    return "Disconnect"
'''

if __name__ == "__main__":
    socketio.run(app)
    # app.run()
    # app.run(host = "0.0.0.0", port = port)
