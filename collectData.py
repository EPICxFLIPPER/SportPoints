import serial
import json
import pandas as pd
from datetime import datetime
import time

# Open serial port
try:
    ser = serial.Serial('COM7', 115200)
    print("Serial port opened successfully")
except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
    exit()

# Initialize an empty DataFrame
columns = ['timestamp', 'accel_x', 'accel_y', 'accel_z', 'gyro_x', 'gyro_y', 'gyro_z', 'altitude', 'label']
data_frame = pd.DataFrame(columns=columns)

def collect_entries(entries, label):
    global data_frame
    print("Buffer Time: 3")
    time.sleep(1)
    print("Buffer Time: 2")
    time.sleep(1)
    print("Buffer Time: 1")
    time.sleep(1)
    i = 0
    while i < entries:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').rstrip()
            try:
                json_data = json.loads(data)
                timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')
                # Add the timestamp and label to the JSON data
                json_data['timestamp'] = timestamp
                json_data['label'] = label
                # Append to the DataFrame
                row_df = pd.DataFrame([json_data])
                data_frame = pd.concat([data_frame, row_df], ignore_index=True)
                print(f"Received JSON: {json_data}")
                i += 1
            except json.JSONDecodeError as e:
                print(f"Error decoding JSON: {e}")

def combineAndSaveData():
    try:
        oldData = pd.read_csv("sensordata.csv")
        combined = pd.concat([oldData,data_frame],ignore_index=True)
        combined.to_csv('sensordata.csv', index=False)
    except FileNotFoundError:
        data_frame.to_csv('sensordata.csv', index=False)
    finally:
        ser.close()



def main():
    while True:
        userInput = input("Select movement type \n 1: standing \n 2: walking \n 3: stairs_up \n 4: stairs_down\n 5: exit \n your response: ")
        if (userInput == "1"):
            collect_entries(100,"standing")
        if (userInput == "2"):
            collect_entries(100,"walking")
        if (userInput == "3"):
            collect_entries(100,"stairs_up")
        if (userInput == "4"):
            collect_entries(100,"staurs_down")
        if (userInput == "exit"):
            combineAndSaveData()
            break


            

main()



# try:
#     collect_entries(100, "stairs_up")
# except KeyboardInterrupt:
#     print("Program interrupted")
# finally:
#     ser.close()
#     print("Serial port closed")
#     # Save the DataFrame to a CSV file
#     data_frame.to_csv('sensordata.csv', index=False)
#     print("Data saved to sensordata.csv")