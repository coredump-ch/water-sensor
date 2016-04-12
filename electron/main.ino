int led = D7;

void setup() {
    Particle.disconnect();

    pinMode(led, OUTPUT);

    Wire.setSpeed(50000);
    Wire.stretchClock(true);
    Wire.begin();

    Serial.begin(9600);
}

int shtAddress = 0x40;
int sht21_trigger_RH_measurement = 229;
const int SHT21_TRIG_RH_MEASUREMENT_HM = 0xe5;

void loop() {
    Serial.print("mi wile toki!\n");
    Wire.beginTransmission(shtAddress);
    Wire.write(SHT21_TRIG_RH_MEASUREMENT_HM);
    Wire.endTransmission();
    delay(100);
    Serial.print("mi wile sona!\n");
    int bytes = Wire.requestFrom(shtAddress, 6);

    for(int n=0; n<bytes; ++n){
        digitalWrite(led, HIGH);
        delay(200);
        digitalWrite(led, LOW);
        delay(200);
    }

    while(Wire.available()){
        char c = Wire.read();
        Serial.print((int)c);
        Serial.print('\n');
    }

    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
}
