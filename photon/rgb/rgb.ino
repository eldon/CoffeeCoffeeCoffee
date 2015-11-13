#define led D0
#define photocell A1

int photocell_val = 0;

void setup() {
    pinMode(D0, OUTPUT);
    pinMode(A1, INPUT);
    Particle.function("setRGB", setColor);
    Particle.function("brightness", setBrightness);
    Particle.variable("photocell", &photocell_val, INT);
    Particle.function("disableRGBControl", freeRGB);
}

void loop() {
    photocell_val = analogRead(photocell);
    delay(200);
}

int setBrightness(String val) {
    analogWrite(led, parse(val));
    return 1;
}

int freeRGB(String val) {
    RGB.control(false);
    return 1;
}

int setColor(String command) {
    RGB.control(true);
    if (command != "") {
        char inputStr[64];
        command.toCharArray(inputStr,64);
        char *p = strtok(inputStr,",");
        int red = atoi(p);
        p = strtok(NULL," ,");
        int grn = atoi(p);
        p = strtok(NULL," ,");
        int blu = atoi(p);
        p = strtok(NULL," ,");
        RGB.color(red,grn,blu);
        return 1;
    }
    else {
        return 0;
    }
}

int parse(String command) {
    if (command != NULL) {
        char inputStr[64];
        command.toCharArray(inputStr,64);
        int value = atoi(inputStr);
        return value;
    }
    else {
        return 0;
    }
}
