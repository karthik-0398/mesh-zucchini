# from gevent import monkey
# monkey.patch_all()

# import serial
# import time
# from threading import Thread

from flask import Flask, render_template
# from flask.ext.socketio import SocketIO, emit, join_room, leave_room

app = Flask(__name__)
app.debug = True
# app.configure['SECRET_KEY'] = 'nomnomnom'
# socketio = SocketIO(app)
# thread = None

'''port = '/dev/ttyS0'
ser = serial.Serial(port, 115200, timeout = 0)

def background_thread():
    while True:
        data = ser.read(9999)
        if len(data) > 0:
            socket.emit('message', {'data': data})
        ser.close()
'''
@app.route("/")
def mesh_dashboard():
    '''global thread
    if thread is None:
        thread = Thread(target=background_thread)
        thread.start()'''
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
    #socketio.run(app)
    app.run()
    # app.run(host = "0.0.0.0", port = port)
