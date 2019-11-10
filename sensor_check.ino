
int sensor = 34;
int valeur ;

void setup() {                
  Serial.begin(9600); // For debugging
  pinMode (sensor, INPUT);   
  
}

void loop() {
  
   float sample = analogRead(sensor);
    //float db = (sample-3.3)*pow(2,32)/1.7; //20*log(sample/0.00002);
    float db = (500* sample) / pow(2,12) ;
    Serial.printf("%f\n", db); // Voltage value * 100
}
