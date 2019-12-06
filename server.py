from gevent import monkey
monkey.patch_all()

import serial #install pySerial
import time
import threading

from flask_bootstrap import Bootstrap


from flask import Flask, render_template, request, json
from flask_socketio import SocketIO, emit, join_room, leave_room

from dataclasses import dataclass



# Flask Setup with SocketIO
app = Flask(__name__)
bootstrap = Bootstrap(app)
app.debug = False
app.config['SECRET_KEY'] = 'nomnomnom'
socketio = SocketIO(app)
thread = None
thread2 = None
thread3 = None
light_command = "None"
light_command_old = "None"

finished = False

board_list = []

# use some address perhaps, to make sure, no unwanted device is connected
@dataclass
class board:
    id: int
    master: bool = False
    connected: bool = False
    glow: bool = False

def init_boards():
    board1 = board(1, True, True, True)
    board2 = board(2, False, True, True)
    board3 = board(3, False, True, False)
    board4 = board(4, False, True, True)
    board_list.append(board1)
    board_list.append(board2)
    board_list.append(board3)
    board_list.append(board4)

messages = []

port = '/dev/ttyACM1' # '/dev/ttyS1' #
# port = '/dev/ttyS4' #
ser = serial.Serial(port, 115200, timeout = 0.5, rtscts=True,dsrdtr=True)
ser.close()
#port2 = '/dev/ttyS2' #
#ser2 = serial.Serial(port2, 115200, timeout = 0.5, rtscts=True,dsrdtr=True)
#ser2.close()
# 9600 baud rate, '/dev/tty.usbserial', ttyUSB0
# Ports: https://pyserial.readthedocs.io/en/latest/shortintro.html


# todo, when other data is read, then there is a bottleneck
# try to avoid messages that are not in the right format
# background thread
def serial_read_thread():
    global light_command_old
    ser.open()
    data = ""
    while True:
        # data = ser.read(5)
        try:
            data = ser.read(30).decode('utf-8')
        except IOError:
            print("Error")
        if len(data) > 0:
            # socketio.emit('message', {'messages': data})
            if(data != "None"):
                messages.append("Received: " + data)
            # print("Message", data)
        time.sleep(2)
        data = ""
        if (light_command_old != light_command):
            ser.write(bytes(light_command[0].encode('utf-8')))  # b'lalala
            light_command_old = light_command
            messages.append("Sent: " + light_command)
            # ser.write(b'\r')
        time.sleep(2)
    ser.flush()
    ser.close()


# writes hihihi every 3 seconds
"""def serial_write_thread():
    global light_command_old
    ser2.open()
    while True:
        if(light_command_old != light_command):
            ser2.write(bytes(light_command.encode('utf-8'))) # b'lalala
            light_command_old = light_command
            messages.append("Sent: " + light_command)
            ser2.write(b'\r')
        time.sleep(2)
    #ser2.flush()
    ser2.close()"""

@app.route("/",  methods=['GET', 'POST'])
@app.route("/index.html")
def mesh_dashboard():

    global light_command
    print(request.method)

    if request.method == 'POST':
        light_command = str(request.form.get('colour_button'))
        finished = False

    print(light_command)

    global thread
    if thread is None:
        thread = threading.Thread(target=serial_read_thread)
        thread.start()

  #  global thread2
  #  if thread2 is None:
  #      thread2 = threading.Thread(target=serial_write_thread)
   #     thread2.start()

#    global thread3
#    if thread3 is None:
#        thread3 = threading.Thread(target=button_pressed)
#        thread3.start()

    boards = [
        {
            'name': 'Leader Board',
            'status': 'Connected',
            'glow': '100',
            'colour_connected': 'info'
        },
        {
            'name': 'Child Board 1',
            'status': 'Connected',
            'glow': '10',
            'colour_connected': 'info'
        },
        {
            'name': 'Child Board 2',
            'status': 'Disconnected',
            'glow': '10',
            'colour_connected': 'secondary'
        },
        {
            'name': 'Child Board 3',
            'status': 'Disconnected',
            'glow': '10',
            'colour_connected': 'secondary'
        },
    ]

    return render_template('index.html', boards=boards, messages=messages)


@app.route("/connect/")
def mesh_connect():
    join_room("one")
    return "Connect"
'''
@app.route("/disconnect/")
def mesh_disconnect():
    leave_room("one")
    return "Disconnect"
'''

if __name__ == "__main__":
    init_boards()
    socketio.run(app)
    # app.run()
    # app.run()
    # app.run(host = "0.0.0.0", port = port)