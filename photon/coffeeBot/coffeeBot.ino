#define BREW_SWITCH D0
#define TEMP A1
#define STATUS_LED D7

int brewing = 0;
int brewing_time = 0;
double temperature = 0;

void setup()
{
    pinMode(BREW_SWITCH, OUTPUT);
    pinMode(STATUS_LED, OUTPUT);
    pinMode(TEMP, INPUT);

    digitalWrite(BREW_SWITCH, LOW);
    digitalWrite(STATUS_LED, LOW);

    Particle.function("brew", controlBrew);
    Particle.variable("temp", &temperature, DOUBLE);
}

void loop()
{
    temperature = analogRead(TEMP);
    if (millis() - brewing_time >= 1.8e+6) {
        stopBrew();
    }
    delay(200);
}

int controlBrew(String command) {
    if (command == "start") {
        startBrew();
        return 1;
    } else if (command == "stop") {
        stopBrew();
        return 0;
    }
    return -1;
}

void startBrew() {
    digitalWrite(BREW_SWITCH, HIGH);
    digitalWrite(STATUS_LED, HIGH);
    brewing = 1;
    brewing_time = millis();
}

void stopBrew() {
    digitalWrite(BREW_SWITCH, LOW);
    digitalWrite(STATUS_LED, LOW);
    brewing = 0;
    brewing_time = 0;
}
