#include <Arduino.h>
#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
          myControllers[i] == ctl;
          Serial.printf("controller connected, index=%d\n")
          return;
        }
    }
      Serial.println("Controller connected, no empty slot");
}



void onDisconnectedController(ControllerPtr ctl) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
          myControllers[i] = nullptr;
          Serial.println("Controller disconnected, index=%d\n")
          return;
        }
    }
      Serial.println("Controller disconnected, not found");
    
}

// Button Presses

void processGamepad(ControllerPtr ctl) {
    if (ctl->a()) onButtonA();
    if (ctl->b()) onButtonB();
    if (ctl->x()) onButtonC();
    if(ctl->brake() > 500) onButtonLT();
}



void processControllers() {
    for (auto ctl : myControllers) {
        if (ctl && ctl->isConnected() && ctl->hasData()) {
          if (ctl->isGamepad()) {
            processGamepad(ctl);
          }
        }
    }
}

//Setup

void setup() {
    Serial.begin(115200);

    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.forgetBluetoothKeys();
    BP32.enableVirtualDevice(false);
}

// Loop

void loop() {
    bool dataUpdated = BP32.update();
    if (dataUpdated)
        processControllers();

    delay(150);
}

