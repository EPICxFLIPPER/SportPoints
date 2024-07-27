import serial

# # Open the serial port (adjust baudrate as needed)
# ser = serial.Serial('COM7', 115200)

# try:
#     while True:
#         if ser.in_waiting > 0:
#             data = ser.readline().decode('utf-8').rstrip()
#             print(f"Received: {data[0]}")
#             print(len(data))
# except KeyboardInterrupt:
#     print("Program interrupted")
# finally:
#     ser.close()

import serial
import json

try:
    ser = serial.Serial('COM7', 115200)
    print("Serial port opened successfully")
except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
    exit()

try:
    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').rstrip()
            try:
                json_data = json.loads(data)
                print(f"Received JSON: {json_data}")
                # Access individual values from the JSON data
                if "buttonOne" in json_data:
                    button_state_one = json_data["buttonOne"]
                    button_state_two = json_data["buttonTwo"]
                    button_state_three = json_data["buttonThree"]
                    led_state = json_data["led"]
                    print(f"Button One: {button_state_one}, Button Two: {button_state_two}, Button Three: {button_state_three}, LED: {led_state}")
                if "acceleration" in json_data:
                    accel = json_data["acceleration"]
                    gyro = json_data["gyro"]
                    temperature = json_data["temperature"]
                    print(f"Acceleration: {accel}, Gyro: {gyro}, Temperature: {temperature}")
                if "temperature" in json_data and "pressure" in json_data:
                    temp = json_data["temperature"]
                    pressure = json_data["pressure"]
                    altitude = json_data["altitude"]
                    humidity = json_data["humidity"]
                    print(f"Temperature: {temp} °C, Pressure: {pressure} hPa, Altitude: {altitude} m, Humidity: {humidity} %")
            except json.JSONDecodeError as e:
                print(f"Error decoding JSON: {e}")
except KeyboardInterrupt:
    print("Program interrupted")
finally:
    ser.close()
    print("Serial port closed")