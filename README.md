# mesh-zucchini

# How to run the board
Directory needed: 
* hello-world

compile with: make

flash with: make flash

screen with: screen /dev/ttyACM0

## Install

*sudo dnf install clang-format-5.0
* sudo dnf install clang
* sudo dnf install automake
*  sudo dnf install g++
* sudo dnf install libtool
* sudo dnf install make
* sudo dnf install rpm



# How to install and run the web server

Files needed: 
* server.py 
* template/index.html

## Flask Installation
https://blog.miguelgrinberg.com/post/the-flask-mega-tutorial-part-i-hello-world

## DNF INSTALL
https://developer.fedoraproject.org/tech/languages/python/flask-installation.html

* mkdir mesh
* cd mesh
* python3 -m venv zucc
* source zucc/bin/activate
* then I am in the virtual environment
* pip install flask
* pip install flask-socketio
* pip install pyserial

## activate and deactivate

* source zucc/bin/activate # activate flask
* python3 server.py # start server
* deactivate # deaktivate flask

## Browser

http://127.0.0.1:5000/

## Error handling

Try to start the server with the development env.

* $ export FLASK_APP=server
* $ export FLASK_ENV=development
* and then you start the server with:
* $ flask run

## GEVNET

* pip install gevent

## pyCharm IDE

* download
* run with [Prinzessin@localhost pycharm-community-2019.2.4]$ bin/pycharm.sh

## Ports and port testing

after activating Flask:

* sudo socat -d -d pty,link=/dev/ttyS0,raw,echo=0 pty,link=/dev/ttyS1,raw,echo=0
* sudo python3 server.py
* echo "Tesmmddddddddmsssmi" > /dev/pts/7 ... get the right port from socat here

