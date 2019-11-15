
int sensor = 34;
int valeur;
void setup()
{
  Serial.begin(9600); // For debugging
  pinMode(sensor, INPUT);
}

void loop()
{
  float sample = analogRead(sensor);
  unsigned long startMillis = millis(); // Start of sample window
  double peakToPeak = 0;                // peak-to-peak level

  double signalMax = 0;
  double signalMin = 4096;
  unsigned int sampleWindow = 50;
  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(sensor);
    if (sample > signalMax)
      signalMax = sample; // save just the max levels
    else if (sample < signalMin)
      signalMin = sample; // save just the min levels
  }
  peakToPeak = signalMax - signalMin;    // max - min = peak-peak amplitude
  double db = (peakToPeak * 5.0) / 4096; // convert to volts*/
  double first = log10(db / 0.00631) * 20;
  double second = first + 94 - 44 - 25 - 10;
  Serial.println(second);
}