#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
/***************************************************************************
* Sketch Name: Lab3 meteor approach
*
* Original Version: 07/09/2021 (by Hakan KAYAN)
* Updated Version: 14/03/2026 (Ndungu Karanja)
*
* Description:
*   - Demonstrates how to read temperature and humidity values from a DHT sensor 
*     (blue variant) using the GrovePi library on an Arduino-compatible board.
*   - Continuously prints the readings in JSON format to the console.
*   - Checks for invalid (NaN) data and outputs an appropriate message if no valid data.
*   - Will activate a buzzer (D8) when the measured teperature is over a threshold. 
*
* Parameters:
*   - PIR, Light, Button, LED (listed in the header),
*     though this specific code focuses on reading from the DHT sensor on D4.
*
* Return:
*   - Temperature (°C), Humidity (%), printed as JSON when valid
***************************************************************************/
"""

import time
import sys
import os
import math
import json
import grovepi

# -----------------------------------------------------------------------------
# 1. DHT Sensor Setup
# -----------------------------------------------------------------------------
#  - The DHT sensor is attached to digital port D4 on the Grove Base Shield.
#  - 'blue = 0' refers to the "blue" version of the Grove DHT (often DHT11).
# -----------------------------------------------------------------------------
sensor = 4  # Digital port D4 for the DHT sensor
buzzer = 8 #Digital port D8 for the Buzzer
blue = 0    # 0 => Grove 'blue' DHT sensor (DHT11)
white = 1   # 1 => 'white' sensor (DHT22), not used here

grovepi.pinMode(buzzer, "OUTPUT")

# -----------------------------------------------------------------------------
# MAIN LOOP
# -----------------------------------------------------------------------------
#  - Repeatedly read temperature and humidity from the DHT sensor, checks if it
#    is greater than a threshold, decides if the buzzer is to be rang and then 
#    print them in JSON format if valid. 
#  - Sleeps 3 seconds between reads to avoid over-polling the sensor.
# -----------------------------------------------------------------------------
while True:
    try:
        # 2. Read from the DHT sensor (blue variant).
        #    grovepi.dht(port, type) returns [temp, humidity].
        [temp, humidity] = grovepi.dht(sensor, blue)
        
        if temp > 22:
            grovepi.digitalWrite(buzzer, 1)
            time.sleep(1)
        else:
            # 7. If temp <= 22, turn the buzzer OFF for 1 second.
            grovepi.digitalWrite(buzzer, 0)
            time.sleep(1)

        # 3. Sleep briefly to avoid saturating the sensor with requests.
        time.sleep(3)

        # 4. Create a JSON string from the readings, e.g. {"temperature":24.2,"humidity":58.1}.
        x = json.dumps({'temperature': temp, 'humidity': humidity})

        # 5. Check for valid (non-NaN) data before printing. 
        #    If either temp or humidity is NaN, skip or print a placeholder message.
        if not math.isnan(temp) and not math.isnan(humidity):
            # Use 'print(..., end="", flush=True)' to print on the same line 
            # and flush output immediately, if desired.
            print(x, end='', flush=True)
        else:
            print("No data.", flush=True)

    except KeyboardInterrupt:
        # 6. Gracefully exit if the user presses Ctrl+C. 
        #    'os._exit(0)' ensures immediate termination without tracebacks.
        print("Terminated.")
        os._exit(0)
