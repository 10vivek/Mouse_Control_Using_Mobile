import pyautogui
import serial
import time

# Configure fail-safe (optional, allows moving mouse to corner to stop)
pyautogui.FAILSAFE = False

# Set up serial connection
try:
    ArduinoSerial = serial.Serial('COM5', 9600)  # Replace COM5 with the correct port
    time.sleep(1)  # Delay for connection setup
except serial.SerialException as e:
    print(f"Error connecting to serial: {e}")
    exit()
while True:
    try:
        # Read data from Arduino
        data = ArduinoSerial.readline().decode('ascii').strip()
        if data:
            x, y, z = data.split(":")  # Expected format: "x:y:z"
            x, y = int(x), int(y)

            # Move mouse
            pyautogui.moveRel(x, y, duration=0.1)  # Move relative to current position

            # Handle clicks and scrolls based on z
            if z == '1':
                pyautogui.click(button='left')
            elif z == '2':
                pyautogui.click(button='right')
            elif z == '3':
                pyautogui.scroll(-1)  # Scroll down
            elif z == '4':
                pyautogui.scroll(1)   # Scroll up
    except ValueError:
        print("Received data in unexpected format.")
    except serial.SerialException as e:
        print(f"Serial communication error: {e}")
        break
