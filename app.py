from flask import *
import json
import eiscp
import sys
import sqlite3
import paho.mqtt.client as mqtt
import requests

app = Flask(__name__)

@app.route('/', methods=['GET'])
def home_splash():
#    volume = int(raw_send_command('MVLQSTN')[3:], 16) # causes hang if AV isn't fully on
#    print(volume)
    return render_template('index.html')


@app.route('/tv-control', methods=['GET', 'POST'])
def tv_splash():
    return render_template('tv-control.html')

# TODO: INCLUDE templates for all controls

# AUDIO

receiver_ip = '192.168.2.2'

@app.route("/vol_change/<command>", methods=['GET'])
def send_command(command):
    # print(command)
    volume = int(raw_send_command(command)[3:], 16)
    print(volume)
    return("")


def code_send_command(command):
    with eiscp.eISCP(receiver_ip) as r:
        return r.command(command)


def raw_send_command(command):
    with eiscp.eISCP(receiver_ip) as r:
        try:
            return r.raw(command)
        except:
            return str("failed")


# TELEVISION

TV_ip = "192.168.2.6"

@app.route("/roku/<path:command>", methods=['GET'])
def send_roku(command):
    adr = 'http://{}:8060/{}'.format(TV_ip, command) # TODO: change to f strings when raspbian updates python3
    if command.startswith('keypress') | command.startswith('launch'):
        requests.post(adr)
    elif command.startswith('$~'):
        [requests.post("http://{}:8060/keypress/Lit_{}".format(TV_ip, i)) for i in command[2:]]
    else:
        r = requests.get(adr)
        print(r.text)
    return("")


# MQTT FOR MCS
@app.route("/mqtt/<path:msg>", methods=['GET'])
def send_mqtt(msg):
    msg = json.loads(msg)
    client.publish(msg['topic'], str(msg['cmd']))
    return(str(msg))


def on_connect(client, userdata, flags, rc):
    print("Connected with code: {}".format(str(rc)))
    # sub_topics = ["lights/bookcase"]
    # client.subscribe("lights/bookcase")


def on_message(client, userdata, msg):
    print("message received")
    print(str(msg.payload))


def mqtt_init():
    client = mqtt.Client()
    client.connect("localhost", 1883, 60)
    client.on_connect = on_connect
    client.on_message = on_message
    client.loop_start()
    return(client)

if __name__ == "__main__":
    client = mqtt_init()
    app.run(debug=True, host='0.0.0.0', threaded=True)
