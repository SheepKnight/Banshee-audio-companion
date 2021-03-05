import serial
import time
import json

host = serial.Serial(port = 'COM24',baudrate=9600,timeout=0.1)

def getData():
    """
    !! A transformer en JSON !!

    Demande les informations au controleurs et les formates sous formes de liste de tuples
    return : [(gain1,delay1),(gain2,delay2),(gain3,delay3)]
    """
    res = []

    host.write(bytes("data","utf-8"))
    time.sleep(0.1)
    data = str(host.readline())

    for i in data.split(":"):
        res+= tuple(i.split("&"))

    return res

def writeData(data):
    """
    !! A transformer en JSON !!
    Ecrit les nouveux réglages au controleur"""
    json_to_send = json.dumps(data)
    print(json_to_send)
    host.write(bytes(json_to_send,"utf-8"))

    return None
