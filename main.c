#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <sys/time.h>

#define led 17
#define button 18
#define LOW 0
#define HIGH 1

unsigned long millis() {
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec * 1000) + (now.tv_usec / 1000);
}

int main() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    int reading;
    int ledState = LOW;
    int buttonState = HIGH;
    int lastButtonState = HIGH;
    long lastUpdateTime;
    long captureTime = 50;

    gpioSetMode(led, PI_OUTPUT);
    gpioSetMode(button, PI_INPUT);
    gpioSetPullUpDown(button, PI_PUD_UP);

    while (1) {
        reading = gpioRead(button);

        if (reading != lastButtonState) {
            lastUpdateTime = millis();
        }

        if (millis() - lastUpdateTime > captureTime) {
            if (reading != buttonState) {
                buttonState = reading;

                if (LOW == buttonState) {
                    printf("Button pressed ... \n");
                    ledState = !ledState;

                    if (ledState) {
                        printf("turn on LED ... \n");
                    } else {
                        printf("... turn off LED \n");
                    }
                } else {
                    printf("... Button released \n");
                }
            }
        }

        gpioWrite(led, ledState);
        lastButtonState = reading;
    }

    gpioTerminate();
    return 0;
}
