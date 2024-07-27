import serial

# Open the serial port (adjust baudrate as needed)
ser = serial.Serial('COM7', 115200)

try:
    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').rstrip()
            print(f"Received: {data}")
except KeyboardInterrupt:
    print("Program interrupted")
finally:
    ser.close()