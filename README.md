
<h1 align="center">
  SecPager
</h1>
  
   
  
  
  
![All](https://github.com/SecPager/pager/blob/main/all_systems.jpg)

__A Secure Pager-like Endpoint Communication Device.__

[Key Features](#key-features) •
[Installation](#installation) •
[How To Use](#how-to-use)

## Key Features

* End-to-end encryption using [AES-192](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard), CBC mode
* Radio communication over 433 MHz
* No transmission capabilities from endpoint devices

## Installation

### The Arduino—Receive

#### Software

The [code for
receiving](https://github.com/SecPager/pager/blob/main/Receiver_Arduino.ino)
needs to be uploaded to your endpoint Arduino. It depends on the
[RadioHead ASK
library](https://github.com/PaulStoffregen/RadioHead/blob/master/RH_ASK.h)
and the [Arduino AESLib
library](https://github.com/DavyLandman/AESLib/blob/master/AESLib.c) for
reception and encryption.

Once you have the dependencies installed, you can upload the code to the
Arduino using [Arduino IDE](https://www.arduino.cc/en/Main/Software).

The private key should be generated externally and inserted into the code.

#### Hardware

Follow our schematics for the hardware to set this up.

![Receive](https://github.com/SecPager/pager/blob/main/receiver.png)

### The Arduino—Send

The process is the same as the receiving Arduino. The dependencies are [RadioHead ASK
library](https://github.com/PaulStoffregen/RadioHead/blob/master/RH_ASK.h)
and the [Arduino AESLib
library](https://www.arduino.cc/reference/en/libraries/aeslib/) for
transmission and encryption.

Follow our schematics for the hardware to set this up, and connect it to your server via USB.

![Transmitter](https://github.com/SecPager/pager/blob/main/transmitter.png)

### The Server

The server needs [Python3](https://www.python.org/) and [Flask](https://flask.palletsprojects.com/en/2.1.x/) installed. We recommend running this server on a Raspberry Pi, which comes with Python3. Flask is a lightweight web framework for Python. It can be installed by following the instructions on [Flask's website](https://flask.palletsprojects.com/en/2.1.x/installation/).


Once the dependencies are installed, you can clone the server code onto a Raspberry Pi with the following command:

```bash
git clone https://github.com/SecPager/pager.git
```

Once the server code is cloned, you can run the server by running the following command:

```bash
cd pager
python3 server.py
```

## How To Use

Connect to the server over HTTP, enter the message you'd like to send, select the intended recipient, and click "Send".
