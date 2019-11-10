
# EMBEDDED SYSTEMS PROJECT

This refers to all the code and other material used during the ESW project course (Monsoon 2019).

## FILES UPLOADED

### 1. run_this.ino

Running this code on the esp32 board just sends the data recorded by it to the oneM2M server and to thingspeak server. (As it provides graphing service).

### 2. sensor_check.ino

Running this code on the esp32 board, displays whatever value is recorded by the microphone sensor.

### 3. server_check.ino

Running this code on the esp32 board just sends the data recorded by it to the oneM2M server.

## TROUBLESHOOTING

### Help! The code shows invalid head of package

1. Check connections
2. Try changing the port
3. Press the reset button and the other button simultaneously, till the board gets connected to it.

### While uploading the data to the server (onem2m), it shows error code 400

Use the esw WiFi connection (available at certain places in campus)