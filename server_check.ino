#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

char *wifi_ssid = "esw-m19@iiith";
char *wifi_pwd = "e5W-eMai@3!20hOct";
String cse_ip = "onem2m.iiit.ac.in";
int microphonePin = 34;

//StaticJsonBuffer<200> jsonBuffer;
//JsonObject &user_data = jsonBuffer.createObject();
//JsonObject &temp_user_data = jsonBuffer.createObject();
//JsonObject &sensor_data = jsonBuffer.createObject();

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

String double2string(double n, int ndec) {
    String r = "";

    int v = n;
    r += v;     // whole number part
    r += '.';   // decimal point
    int i;
    for (i=0;i<ndec;i++) {
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
  float sample,db[200];
  float DB_MAX=0;
  for(int i=0;i<10;i++)
  {
    sample = analogRead(microphonePin);
    db[i]=(500* sample) / pow(2,12) ;
    Serial.printf("%f ", db[i]);
    if(DB_MAX<db[i])DB_MAX=db[i];
    //String d=String(db[i]);
    //s=String(s+" ");
    //Serial.printf("\nSTRING: %s K\n", s);
    delay(100);
  }
  String sensor_string = double2string(DB_MAX,4);
  Serial.printf("THE VALUE SENT IS %f\n",DB_MAX);
  //Serial.printf(" %s\n",sensor_string);
    createCI(server, "Team38_Sound_Noise_around_the_campus", "node_1", sensor_string);
    //Serial.println(s);
    delay(1000);
    Serial.println();
//*************    delay(15000);
}
