#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

char *wifi_ssid = "esw-m19@iiith";
char *wifi_pwd = "e5W-eMai@3!20hOct";

// if testing using personal WiFi
// char *wifi_ssid = "Disconnected";
// char *wifi_pwd = "12345678";

String cse_ip = "onem2m.iiit.ac.in";
int microphonePin = 34;
int sensor = 34;

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
    Serial.begin(9600);
    pinMode(microphonePin, INPUT);

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

String double2string(double n, int ndec)
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
    float sample;

    // code starts here
    double second;
    float sample = analogRead(sensor);
    unsigned long startMillis = millis(); // Start of sample window
    double peakToPeak = 0;                // peak-to-peak level
    double signalMax = 0;
    double signalMin = 4096;
    // collect data for 50 mS
    unsigned int sampleWindow = 50;
    while (millis() - startMillis < sampleWindow)
    {
        sample = analogRead(sensor);
        if (sample > signalMax)
            signalMax = sample; // save just the max levels
        else if (sample < signalMin)
            signalMin = sample; // save just the min levels
    }
    peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude

    double db = (peakToPeak * 5.0) / 4096; // convert to volts*/

    double first = log10(db / 0.00631) * 20;
    second = first + 94 - 44 - 25 - 10; //convert to decibels
    delay(100);
    // code ends here

    String sensor_string = double2string(second, 4);
    Serial.printf("THE VALUE SENT IS %f\n", second);
    createCI(server, "Team38_Sound_Noise_around_the_campus", "node_1", sensor_string);
    delay(1000);
    Serial.println();
    //*************    delay(15000);
}