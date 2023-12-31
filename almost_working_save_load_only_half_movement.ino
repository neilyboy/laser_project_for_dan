#include <AccelStepper.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>

// Define the connections for the first stepper motor
#define STEPPER1_PIN_1 32
#define STEPPER1_PIN_2 33
#define STEPPER1_PIN_3 25
#define STEPPER1_PIN_4 26

// Define the connections for the second stepper motor
#define STEPPER2_PIN_1 19
#define STEPPER2_PIN_2 21
#define STEPPER2_PIN_3 22
#define STEPPER2_PIN_4 23

// Define the connections for the third stepper motor
#define STEPPER3_PIN_1 15
#define STEPPER3_PIN_2 5
#define STEPPER3_PIN_3 4
#define STEPPER3_PIN_4 16

// Define the connections for the third stepper motor
#define STEPPER4_PIN_1 18
#define STEPPER4_PIN_2 17
#define STEPPER4_PIN_3 14
#define STEPPER4_PIN_4 27

// Define the limit switch pins for the first and second motors
#define LIMIT_SWITCH_PIN_1 34 // GPIO for the first motor's limit switch
#define LIMIT_SWITCH_PIN_2 35 // GPIO for the second motor's limit switch
#define LIMIT_SWITCH_PIN_3 36 // GPIO for the third motor's limit switch
#define LIMIT_SWITCH_PIN_4 39 // GPIO for the fourth motor's limit switch

// Start address in EEPROM
int motorPosition1 = 0;
int motorPosition2 = 0;
int motorPosition3 = 0;
int motorPosition4 = 0;
int eepromAddress = 0;

// Create AccelStepper objects for both motors
AccelStepper stepper1(AccelStepper::FULL4WIRE, STEPPER1_PIN_1, STEPPER1_PIN_3, STEPPER1_PIN_2, STEPPER1_PIN_4);
AccelStepper stepper2(AccelStepper::FULL4WIRE, STEPPER2_PIN_1, STEPPER2_PIN_3, STEPPER2_PIN_2, STEPPER2_PIN_4);
AccelStepper stepper3(AccelStepper::FULL4WIRE, STEPPER3_PIN_1, STEPPER3_PIN_3, STEPPER3_PIN_2, STEPPER3_PIN_4);
AccelStepper stepper4(AccelStepper::FULL4WIRE, STEPPER4_PIN_1, STEPPER4_PIN_3, STEPPER4_PIN_2, STEPPER4_PIN_4);

// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "Password";

// Create an instance of the AsyncWebServer
AsyncWebServer server(80);

void setup() {
  // Set the motor speed and acceleration for both motors
  stepper1.setMaxSpeed(1000); // Adjust as needed
  stepper1.setAcceleration(500); // Adjust as needed

  stepper2.setMaxSpeed(1000); // Adjust as needed
  stepper2.setAcceleration(500); // Adjust as needed

  stepper3.setMaxSpeed(1000); // Adjust as needed
  stepper3.setAcceleration(500); // Adjust as needed

  stepper4.setMaxSpeed(1000); // Adjust as needed
  stepper4.setAcceleration(500); // Adjust as needed

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set the limit switch pins as inputs with pull-up resistors
  pinMode(LIMIT_SWITCH_PIN_1, INPUT);
  pinMode(LIMIT_SWITCH_PIN_2, INPUT);
  pinMode(LIMIT_SWITCH_PIN_3, INPUT);
  pinMode(LIMIT_SWITCH_PIN_4, INPUT);

  // Home the first motor in the clockwise direction using its limit switch
  while (digitalRead(LIMIT_SWITCH_PIN_1) == HIGH) {
    stepper1.setSpeed(100); // Set the initial speed (clockwise)
    stepper1.runSpeed();
  }

  // Limit switch triggered for the first motor, move 200 steps away (center position)
  stepper1.setSpeed(-100); // Set the speed to move away (counter-clockwise)
  stepper1.move(-200); // Move 200 steps
  while (stepper1.distanceToGo() != 0) {
    stepper1.run();
  }

  // Set the current position to zero for the first motor
  stepper1.setCurrentPosition(0);

  // Home the second motor in the clockwise direction using its limit switch
  while (digitalRead(LIMIT_SWITCH_PIN_2) == HIGH) {
    stepper2.setSpeed(100); // Set the initial speed (clockwise)
    stepper2.runSpeed();
  }

  // Limit switch triggered for the second motor, move 200 steps away (center position)
  stepper2.setSpeed(-100); // Set the speed to move away (counter-clockwise)
  stepper2.move(-200); // Move 200 steps
  while (stepper2.distanceToGo() != 0) {
    stepper2.run();
  }

  // Set the current position to zero for the second motor
  stepper2.setCurrentPosition(0);

    // Home the third motor in the clockwise direction using its limit switch
  while (digitalRead(LIMIT_SWITCH_PIN_3) == HIGH) {
    stepper3.setSpeed(100); // Set the initial speed (clockwise)
    stepper3.runSpeed();
  }

  // Limit switch triggered for the third motor, move 200 steps away (center position)
  stepper3.setSpeed(-100); // Set the speed to move away (counter-clockwise)
  stepper3.move(-200); // Move 200 steps
  while (stepper3.distanceToGo() != 0) {
    stepper3.run();
  }

  // Set the current position to zero for the second motor
  stepper3.setCurrentPosition(0);

    // Home the fourth motor in the clockwise direction using its limit switch
  while (digitalRead(LIMIT_SWITCH_PIN_4) == HIGH) {
    stepper4.setSpeed(100); // Set the initial speed (clockwise)
    stepper4.runSpeed();
  }

  // Limit switch triggered for the third motor, move 200 steps away (center position)
  stepper4.setSpeed(-100); // Set the speed to move away (counter-clockwise)
  stepper4.move(-200); // Move 200 steps
  while (stepper4.distanceToGo() != 0) {
    stepper4.run();
  }

  // Set the current position to zero for the second motor
  stepper4.setCurrentPosition(0);


  // Serve the web interface for both motors
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body>";
    html += "<h1>Stepper Motor Control</h1>";

    // Control for the first motor
    html += "<h2>Motor 1</h2>";
    html += "Slider Value: <input type='range' min='0' max='100' value='50' id='slider1'><br>";
    html += "<span id='sliderValue1'>50</span>";

    // Control for the second motor
    html += "<h2>Motor 2</h2>";
    html += "Slider Value: <input type='range' min='0' max='100' value='50' id='slider2'><br>";
    html += "<span id='sliderValue2'>50</span>";

    // Control for the third motor
    html += "<h2>Motor 3</h2>";
    html += "Slider Value: <input type='range' min='0' max='100' value='50' id='slider3'><br>";
    html += "<span id='sliderValue3'>50</span>";

    // Control for the fourth motor
    html += "<h2>Motor 4</h2>";
    html += "Slider Value: <input type='range' min='0' max='100' value='50' id='slider4'><br>";
    html += "<span id='sliderValue4'>50</span>";

    // Save Positions Button
    html += "<button onclick='savePositions()'>Save Positions</button><br>";

    // Load Positions Button
    html += "<button onclick='loadPositions()'>Load Positions</button><br>";


    html += "<script>";
    html += "var slider1 = document.getElementById('slider1');";
    html += "slider1.oninput = function() {";
    html += "document.getElementById('sliderValue1').innerHTML = slider1.value;";
    html += "var xhttp = new XMLHttpRequest();";
    html += "xhttp.open('GET', '/slider1?value=' + slider1.value, true);";
    html += "xhttp.send();";
    html += "};";

    html += "var slider2 = document.getElementById('slider2');";
    html += "slider2.oninput = function() {";
    html += "document.getElementById('sliderValue2').innerHTML = slider2.value;";
    html += "var xhttp = new XMLHttpRequest();";
    html += "xhttp.open('GET', '/slider2?value=' + slider2.value, true);";
    html += "xhttp.send();";
    html += "};";

    html += "var slider3 = document.getElementById('slider3');";
    html += "slider3.oninput = function() {";
    html += "document.getElementById('sliderValue3').innerHTML = slider3.value;";
    html += "var xhttp = new XMLHttpRequest();";
    html += "xhttp.open('GET', '/slider3?value=' + slider3.value, true);";
    html += "xhttp.send();";
    html += "};";

    html += "var slider4 = document.getElementById('slider4');";
    html += "slider4.oninput = function() {";
    html += "document.getElementById('sliderValue4').innerHTML = slider4.value;";
    html += "var xhttp = new XMLHttpRequest();";
    html += "xhttp.open('GET', '/slider4?value=' + slider4.value, true);";
    html += "xhttp.send();";
    html += "};";

    html += "function savePositions() {";
    html += "var xhttp = new XMLHttpRequest();";
    html += "xhttp.open('GET', '/save', true);";
    html += "xhttp.send();";
    html += "};";

    html += "function loadPositions() {";
    html += "var xhttp = new XMLHttpRequest();";
    html += "xhttp.open('GET', '/load', true);";
    html += "xhttp.send();";
    html += "};";


    html += "</script>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });

  // Handle slider value changes for both motors
  server.on("/slider1", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("value")) {
      String sliderValue = request->getParam("value")->value();
      int targetPosition = map(sliderValue.toInt(), 0, 100, -150, 150); // Map to stepper steps
      stepper1.moveTo(targetPosition);
      request->send(200, "text/plain", "Motor 1 Slider value set to " + sliderValue);
    } else {
      request->send(400, "text/plain", "Missing value parameter");
    }
  });

  server.on("/slider2", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("value")) {
      String sliderValue = request->getParam("value")->value();
      int targetPosition = map(sliderValue.toInt(), 0, 100, -150, 150); // Map to stepper steps
      stepper2.moveTo(targetPosition);
      request->send(200, "text/plain", "Motor 2 Slider value set to " + sliderValue);
    } else {
      request->send(400, "text/plain", "Missing value parameter");
    }
  });

  server.on("/slider3", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("value")) {
      String sliderValue = request->getParam("value")->value();
      int targetPosition = map(sliderValue.toInt(), 0, 100, -150, 150); // Map to stepper steps
      stepper3.moveTo(targetPosition);
      request->send(200, "text/plain", "Motor 3 Slider value set to " + sliderValue);
    } else {
      request->send(400, "text/plain", "Missing value parameter");
    }
  });  

  server.on("/slider4", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("value")) {
      String sliderValue = request->getParam("value")->value();
      int targetPosition = map(sliderValue.toInt(), 0, 100, -150, 150); // Map to stepper steps
      stepper4.moveTo(targetPosition);
      request->send(200, "text/plain", "Motor 3 Slider value set to " + sliderValue);
    } else {
      request->send(400, "text/plain", "Missing value parameter");
    }
  });


  server.on("/save", HTTP_GET, [](AsyncWebServerRequest *request){
    // Save motor positions to EEPROM
    EEPROM.put(eepromAddress, motorPosition1);
    eepromAddress += sizeof(motorPosition1);
    EEPROM.put(eepromAddress, motorPosition2);
    eepromAddress += sizeof(motorPosition2);
    EEPROM.put(eepromAddress, motorPosition3);
    eepromAddress += sizeof(motorPosition3);
    EEPROM.put(eepromAddress, motorPosition4);
    EEPROM.commit(); // Save the data to EEPROM
    request->send(200, "text/plain", "Positions saved to EEPROM");
});

  server.on("/load", HTTP_GET, [](AsyncWebServerRequest *request){
    // Load motor positions from EEPROM
    EEPROM.get(eepromAddress, motorPosition1);
    eepromAddress += sizeof(motorPosition1);
    EEPROM.get(eepromAddress, motorPosition2);
    eepromAddress += sizeof(motorPosition2);
    EEPROM.get(eepromAddress, motorPosition3);
    eepromAddress += sizeof(motorPosition3);
    EEPROM.get(eepromAddress, motorPosition4);

    // Move motors to loaded positions and update sliders
    stepper1.moveTo(motorPosition1);
    stepper2.moveTo(motorPosition2);
    stepper3.moveTo(motorPosition3);
    stepper4.moveTo(motorPosition4);

    request->send(200, "text/plain", "Positions loaded from EEPROM");
});


  // Start the web server
  server.begin();
}

void loop() {
  // Run both stepper motors
  stepper1.run();
  stepper2.run();
  stepper3.run();
  stepper4.run();
}
