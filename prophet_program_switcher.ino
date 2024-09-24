#include "USBHost_t36.h"
#include <Bounce.h>

#define MIN_PROGRAM 0
#define MAX_PROGRAM 127
#define DEBOUNCE_MS 50

#define PIN_BUTTON_DOWN 10
#define PIN_BUTTON_UP 9
#define PIN_LED_CONNECTED 25
#define PIN_LED_READY 24

Bounce programDown = Bounce(PIN_BUTTON_DOWN, DEBOUNCE_MS);
Bounce programUp = Bounce(PIN_BUTTON_UP, DEBOUNCE_MS);

USBHost myusb;
MIDIDevice midiDevice(myusb);

bool deviceConnected = false;
bool deviceReady = false;
uint8_t midiChannel;
uint8_t currentProgram;

void setup() {
  delay(1500);  // Boot Recovery

  myusb.begin();
  midiDevice.setHandleProgramChange(OnProgramChange);

  pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);
  pinMode(PIN_BUTTON_UP, INPUT_PULLUP);
  pinMode(PIN_LED_READY, OUTPUT);
  pinMode(PIN_LED_CONNECTED, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);
  // digitalWrite(PIN_LED_READY, HIGH);
  // digitalWrite(PIN_LED_CONNECTED, HIGH);
}


void loop() {
  myusb.Task();
  midiDevice.read();
  if (deviceConnected && !midiDevice) {
    OnDisconnect();
  } else if (!deviceConnected && midiDevice) {
    OnConnect();
  }
  if (deviceConnected && deviceReady) {
    if (programDown.update() && programDown.fallingEdge() && (currentProgram > MIN_PROGRAM)) {
      midiDevice.sendProgramChange(--currentProgram, midiChannel);
      // Serial.println("DN");
    }
    if (programUp.update() && programUp.fallingEdge() && (currentProgram < MAX_PROGRAM)) {
      midiDevice.sendProgramChange(++currentProgram, midiChannel);
      // Serial.println("UP");
    }
  }
}

void OnProgramChange(uint8_t channel, uint8_t program) {
  midiChannel = channel;
  currentProgram = program;
  deviceReady = true;
  digitalWrite(PIN_LED_READY, HIGH);
  // Serial.print("Channel: ");
  // Serial.print(channel);
  // Serial.print(", Program: ");
  // Serial.println(program);
}

void OnConnect() {
  deviceConnected = true;
  digitalWrite(PIN_LED_CONNECTED, HIGH);
  // Serial.println("Connect");
}

void OnDisconnect() {
  deviceConnected = false;
  deviceReady = false;
  midiChannel = NULL;
  currentProgram = NULL;
  digitalWrite(PIN_LED_CONNECTED, LOW);
  digitalWrite(PIN_LED_READY, LOW);
  // Serial.println("Disconnect");
}
