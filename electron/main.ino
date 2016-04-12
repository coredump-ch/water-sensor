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
const int SHT21_TRIG_RH_MEASUREMENT_HM = 0xe5;
const int SHT21_TRIG_T_MEASUREMENT_NHM = 0xf3;

void loop() {
    Serial.println("mi wile toki!");
    Wire.beginTransmission(shtAddress);
    Wire.write(SHT21_TRIG_T_MEASUREMENT_NHM);
    Wire.endTransmission();
    delay(100);
    Serial.println("mi wile sona!");
    int bytes = Wire.requestFrom(shtAddress, 3);

    for(int n=0; n<bytes; ++n){
        digitalWrite(led, HIGH);
        delay(200);
        digitalWrite(led, LOW);
        delay(200);
    }

    if (Wire.available()) {
        char hi = Wire.read();
        char lo = Wire.read();
        char crc = Wire.read();

        int32_t val = hi << 8 | lo;
        int32_t temp = ((val * 21965) >> 13) - 46850;

        Serial.print("hi=");
        Serial.print(hi, HEX);
        Serial.print(" lo=");
        Serial.print(lo, HEX);
        Serial.print("\nval=");
        Serial.print(val, HEX);
        Serial.print("\ntemp=");
        Serial.print(temp);
        Serial.print('\n');
    }

    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
}
