#
# Python Webserver (based off of https://randomnerdtutorials.com/raspberry-pi-web-server-using-flask-to-control-gpios/)
#

# Import Section
from flask import Flask, render_template, request # Import Flask web framework
from time import sleep # Import the sleep function from the time module

import serial
import time
import binascii
import secrets

# Global Variables

IV_SIZE = 24 # bytes

ids = [
    {
        "name": "pager-1",
        "key": b"vq\xc5\r\xa0\xa6W\x9b\xdb}\x80]\xc5+v\xff4\xa0\xf5H\xc8\x11t\xd0",
        "cap": b"\x00\x00\x00\x01",
    },
    {
        "name": "pager-2",
        "key": b"\x1e\n\x9cx$+fX\xab1'\xce\x08\xc0W`\xb5\xd0\x03'\x0c\x08\xa1\xfb",
        "cap": b"\x00\x00\x00\x02",
    },
]

# Setup

app = Flask(__name__, template_folder='Templates')   # Create an instance of flask called "app"

@app.route("/")
def main():
    # Return the template index.html
    return render_template('index.html', ids=ids)

# The function below is executed when someone requests a URL with the mode and action in it:
# FIX ME
@app.route("/send", methods=["POST"])
def action():
    global ids
    #encryptAES(request.form.get('text'), keys[request.form.get('dropdown')])
    id_ = next(id_ for id_ in ids if id_["name"] == request.form.get('dropdown'))
    message = request.form.get('text').encode()
    iv = secrets.token_bytes(IV_SIZE)
    full = id_['cap'] + id_['key'] + iv + message
    ser.write(full)
    # print(full)
    return render_template('index.html', ids=ids, test=full)

if __name__ == "__main__":
    ser = serial.Serial(port="/dev/ttyACM0", baudrate=9600)
    time.sleep(2)
    app.run(host="0.0.0.0", debug=True)


