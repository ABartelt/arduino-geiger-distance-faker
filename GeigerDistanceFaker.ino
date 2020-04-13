#define SOUND_PIN 3
#define trigPin 13
#define echoPin 12


volatile float maxDelay = 1000.0;
volatile float currentDelay = 1000.0;
volatile boolean isNeedScan = false;

unsigned long lastBeep = 0;

void generateCurrentDelay() {
    currentDelay = random(1, maxDelay);
}

float signalToMaxDelay(int signal) {
    return exp((signal - 15.) / 10.) * 5.;
}

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(SOUND_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}
// return distance in mm
long distance() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}
void beep() {
    digitalWrite(SOUND_PIN, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    delay(5);
    digitalWrite(SOUND_PIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
}

void onCompleteDistance() {
    float strongestSignal = 10;
    int currentSignal = distance();
    if (currentSignal > strongestSignal) {
        strongestSignal = currentSignal;
    }
    Serial.print(maxDelay);    maxDelay = signalToMaxDelay(strongestSignal);
    generateCurrentDelay();
}

void loop() {
  unsigned long now = millis();
  long delay = now - lastBeep;
  
  if (delay > currentDelay) {
      generateCurrentDelay();
      lastBeep = now;
      beep();
      onCompleteDistance();
  }
  
  //Serial.print(distance());
  //Serial.println(" cm");
}
