int dataReceived[5] = {0, 0, 0, 0};  // Array to store data bytes from the phone
int inByte = 0;
int arrayIndex = 0;
int lPrev = 0, rPrev = 0;            // Previous status of left and right clicks
int mouseX = 0, mouseY = 0;

void setup() {
    Serial.begin(9600);              // Begin serial communication at 9600 baud rate
}

void loop() {
    int click = 0;
    const int sensitivity = 20;      // Adjust movement sensitivity
    int xOffset = 0, yOffset = 0;

    // Check if there is data available in the serial buffer
    if (Serial.available() > 0) {  
        inByte = Serial.read();       // Read the incoming byte

        // Start of a new data packet (indicated by 255)
        if (inByte == 255) {
            arrayIndex = 0;
        }

        // Store incoming byte into the data array
        dataReceived[arrayIndex] = inByte;
        arrayIndex++;

        // Process data only when we have received the full packet (4 bytes)
        if (arrayIndex >= 4) {
            // Process joystick data for X and Y directions
            if (dataReceived[1] >= 110) {
                xOffset = map(dataReceived[1], 110, 172, 0, sensitivity);  // Move right
            } else if (dataReceived[1] <= 70) {
                xOffset = map(dataReceived[1], 60, 1, 0, -sensitivity);    // Move left
            }

            if (dataReceived[2] >= 110) {
                yOffset = map(dataReceived[2], 110, 255, 0, sensitivity);  // Move down
            } else if (dataReceived[2] <= 60) {
                yOffset = map(dataReceived[2], 70, 1, 0, -sensitivity);    // Move up
            }

            // Check for mouse actions based on the third byte
            if (dataReceived[3] == 1 && lPrev == 0) {
                click = 1;  // Left click
            } else if (dataReceived[3] == 2 && rPrev == 0) {
                click = 2;  // Right click
            } else if (dataReceived[3] == 3) {
                click = 3;  // Scroll down
            } else if (dataReceived[3] == 4) {
                click = 4;  // Scroll up
            }

            // Update the previous click status
            lPrev = (dataReceived[3] == 1) ? 1 : 0;
            rPrev = (dataReceived[3] == 2) ? 1 : 0;

            // Update the mouse position and click status if there is movement or a click
            if (xOffset != 0 || yOffset != 0 || click != 0) {
                mouseX = constrain(mouseX + xOffset, 0, 1279);  // Constrain X position within screen width
                mouseY = constrain(mouseY + yOffset, 0, 799);    // Constrain Y position within screen height

                // Send the updated position and action over serial
                Serial.print(mouseX);
                Serial.print(":");
                Serial.print(mouseY);
                Serial.print(":");
                Serial.println(click);

                // Reset the click status after sending
                click = 0;
            }

            // Reset array index for the next data packet
            arrayIndex = 0;
        }
    }
}
