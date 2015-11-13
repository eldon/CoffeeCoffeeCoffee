#define BREW_SWITCH D0
#define TEMP A5
#define STATUS_LED D7
#define FILL_25 A1
#define FILL_50 A2
#define FILL_75 A3

#include <math.h>

const int FILL_THRESH = 600;

int brewing = 0;
int brewing_time = 0;
double fill_perc = 0.0;
double temperature = 0.0;
int temp_raw = 0;
int fill_25 = 0;
int fill_50 = 0;
int fill_75 = 0;

void setup()
{
    pinMode(BREW_SWITCH, OUTPUT);
    pinMode(STATUS_LED, OUTPUT);
    pinMode(TEMP, INPUT);
    pinMode(FILL_25, INPUT);
    pinMode(FILL_50, INPUT);
    pinMode(FILL_75, INPUT);

    digitalWrite(BREW_SWITCH, LOW);
    digitalWrite(STATUS_LED, LOW);

    Particle.function("brew", controlBrew);
    Particle.variable("brew_status", &brewing, INT);
    Particle.variable("temp_raw", &temp_raw, INT);
    Particle.variable("temp", &temperature, DOUBLE);
    Particle.variable("fill_perc", &fill_perc, DOUBLE);
    Particle.variable("fill25", &fill_25, INT);
    Particle.variable("fill50", &fill_50, INT);
    Particle.variable("fill75", &fill_75, INT);
}

void loop()
{
    temp_raw = analogRead(TEMP);
    temperature = getTemperature(TEMP);
    fill_25 = sensor_tripped(FILL_25, FILL_THRESH);
    fill_50 = sensor_tripped(FILL_50, FILL_THRESH);
    fill_75 = sensor_tripped(FILL_75, FILL_THRESH);
    fill_perc = 0.30 * (fill_25 + fill_50 + fill_75);

    if (brewing && millis() - brewing_time >= 1.8e+6) {
        stopBrew();
    }
    delay(200);
}

int sensor_tripped(int pin, int thresh) {
    if (analogRead(pin) < thresh) {
        return 1;
    }
    return 0;
}

double getTemperature(int pin) {
    int v = analogRead(pin);   //voltage of the thermpin
    double res = (4096.0/v)-1;
    res = 10000 * res;
    double temp = (1/(0.00096564 +(0.00021068 * log(res) ) +(0.000000085826*( pow( log(res) ,3)))))-273.15;
    temp = (temp * 9.0/5.0)+32;
    return temp;
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
