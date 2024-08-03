import serial
import json

try:
    ser = serial.Serial('COM7', 115200)
    print("Serial port opened successfully")
except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
    exit()

def collectEntries(entries):
    i = 0
    while (i < entries):
        if ser.in_waiting > 0:
            i = i + 1
            data = ser.readline().decode('utf-8').rstrip()
            try:
                json_data = json.loads(data)
                print(f"Received JSON: {json_data}")
                print(i)
            except json.JSONDecodeError as e:
                print(f"Error decoding JSON: {e}")



try:
    collectEntries(100)
except KeyboardInterrupt:
    print("Program interrupted")
finally:
    ser.close()
    print("Serial port closed")