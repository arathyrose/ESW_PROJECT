#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"
#include "ThingSpeak.h"

//college oneM2M server connection is done using college WiFi network
char *wifi_ssid = "esw-m19@iiith";
char *wifi_pwd = "e5W-eMai@3!20hOct";

//Sensor pin to which the microphone(sound sensor) is connected to
int sensor = 34;

//Thingspeak server requirements
unsigned long myChannelNumber = 906986;
const char *myWriteAPIKey = "CYUPS7BAIW79C1V4";

WiFiClient client;

//One M2M server
String cse_ip = "onem2m.iiit.ac.in";
String cse_port = "443";
String server = "http://" + cse_ip + ":" + cse_port + "/~/in-cse/in-name/";

//"http://onem2m.iiit.ac.in:443/~/in-cse/in-name/Team38_Sound_Noise_around_the_campus/node_1"

String createCI(String server, String ae, String cnt, String val)
{
  HTTPClient http;
  http.begin(server + ae + "/" + cnt);
  http.addHeader("X-M2M-Origin", "admin:admin");
  http.addHeader("Content-Type", "application/json;ty=4");
  int code = http.POST("{\"m2m:cin\": {\"cnf\": \"text/plain:0\",\"con\": " + String(val) + "}}");
  http.end();
  Serial.print(code);
  delay(100);
}

void setup()
{
  Serial.begin(9600); // Initialise the Serial monitor and the connection between the sensor and the board
  pinMode(sensor, INPUT);

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client); // Initialize ThingSpeak

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(wifi_ssid, wifi_pwd);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println("Setup done");
}

String double2string(double n, int ndec) // converts a double to a string with ndec number of decimal places
{
  String r = "";
  int v = n;
  r += v;   // whole number part
  r += '.'; // decimal point
  int i;
  for (i = 0; i < ndec; i++)
  {
    // iterate through each decimal digit for 0..ndec
    n -= v;
    n *= 10;
    v = n;
    r += v;
  }
  return r;
}

void loop()
{
  //code starts here
  double second;
  float sample = analogRead(sensor);
  unsigned long startMillis = millis(); // Start of sample window
  double peakToPeak = 0;                // peak-to-peak level
  double signalMax = 0;
  double signalMin = 4096;
  unsigned int sampleWindow = 50; // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(sensor);
    if (sample > signalMax)
      signalMax = sample; // save the max level
    else if (sample < signalMin)
      signalMin = sample; // save the min level
  }
  peakToPeak = signalMax - signalMin;      // max - min = peak-peak amplitude
  double db = (peakToPeak * 5.0) / 4096;   // convert to volts
  double first = log10(db / 0.00631) * 20; // convert to a logarithmic scale
  second = first + 94 - 44 - 25 - 10;      //convert to decibels
  delay(100);
  //code ends here

  String sensor_string = double2string(DB_MAX, 4);
  Serial.printf("THE VALUE SENT IS %f\n", DB_MAX);
  createCI(server, "Team38_Sound_Noise_around_the_campus", "node_1", sensor_string); // update to OneM2M server
  delay(1000);
  Serial.println();
  int x = ThingSpeak.writeField(myChannelNumber, 1, DB_MAX, myWriteAPIKey); // update to thingSpeak
  Serial.println("Channel update successful.");

  delay(15000);
}
