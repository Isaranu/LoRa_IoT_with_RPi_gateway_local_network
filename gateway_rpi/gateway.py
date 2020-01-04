import os, sys
import serial
import time
import requests

gateway = serial.Serial('/dev/ttyUSB0',115200)

while True:

   data = gateway.readline()
   _arr = data.split(":")
   
   ## Set payload ##
   if len(_arr) == 5:
    payload = {'cnt':_arr[0],'node_id':_arr[1],'temp':_arr[2],'humid':_arr[3],'rssi':_arr[4].replace('\r\n','')}
    print (payload)
    try:
     r = requests.get('http://{LOCAL-SERVER-IP-ADDRESS}:31416/setdata', params=payload)
     response = r.text
     print (response)
    except requests.exceptions.ConnectionError as e:
      print (e) 
   else:
    print("Data format invalid !")
