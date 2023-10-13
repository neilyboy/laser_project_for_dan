#include <AccelStepper.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

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

// Create AccelStepper objects for both motors
AccelStepper stepper1(AccelStepper::FULL4WIRE, STEPPER1_PIN_1, STEPPER1_PIN_3, STEPPER1_PIN_2, STEPPER1_PIN_4);
AccelStepper stepper2(AccelStepper::FULL4WIRE, STEPPER2_PIN_1, STEPPER2_PIN_3, STEPPER2_PIN_2, STEPPER2_PIN_4);
AccelStepper stepper3(AccelStepper::FULL4WIRE, STEPPER3_PIN_1, STEPPER3_PIN_3, STEPPER3_PIN_2, STEPPER3_PIN_4);
AccelStepper stepper4(AccelStepper::FULL4WIRE, STEPPER4_PIN_1, STEPPER4_PIN_3, STEPPER4_PIN_2, STEPPER4_PIN_4);

// Replace with your network credentials
const char* ssid = "Hagan";
const char* password = "3092648336";

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
    html += "Manual Input: <input type='text' id='manual1' value='50'><br>";
    html += "<span id='sliderValue1'>50</span>";

    // Control for the second motor
    html += "<h2>Motor 2</h2>";
    html += "Slider Value: <input type='range' min='0' max='100' value='50' id='slider2'><br>";
    html += "Manual Input: <input type='text' id='manual2' value='50'><br>";
    html += "<span id='sliderValue2'>50</span>";

    // Control for the third motor
    html += "<h2>Motor 3</h2>";
    html += "Slider Value: <input type='range' min='0' max='100' value='50' id='slider3'><br>";
    html += "Manual Input: <input type='text' id='manual3' value='50'><br>";
    html += "<span id='sliderValue3'>50</span>";

    // Control for the fourth motor
    html += "<h2>Motor 4</h2>";
    html += "Slider Value: <input type='range' min='0' max='100' value='50' id='slider4'><br>";
    html += "Manual Input: <input type='text' id='manual4' value='50'><br>";
    html += "<span id='sliderValue4'>50</span>";

    html += "<script>";
    // Modify JavaScript to handle slider and input field changes
    html += "var slider1 = document.getElementById('slider1');";
    html += "var manual1 = document.getElementById('manual1');";
    html += "slider1.oninput = manual1.oninput = function() {";
    html += "  var value = Math.min(slider1.value, manual1.value);";
    html += "  document.getElementById('sliderValue1').innerHTML = value;";
    html += "  var xhttp = new XMLHttpRequest();";
    html += "  xhttp.open('GET', '/slider1?value=' + value, true);";
    html += "  xhttp.send();";
    html += "};";

    html += "var slider2 = document.getElementById('slider2');";
    html += "var manual2 = document.getElementById('manual2');";
    html += "slider2.oninput = manual2.oninput = function() {";
    html += "  var value = Math.min(slider2.value, manual2.value);";
    html += "  document.getElementById('sliderValue2').innerHTML = value;";
    html += "  var xhttp = new XMLHttpRequest();";
    html += "  xhttp.open('GET', '/slider2?value=' + value, true);";
    html += "  xhttp.send();";
    html += "};";

    html += "var slider3 = document.getElementById('slider3');";
    html += "var manual3 = document.getElementById('manual3');";
    html += "slider3.oninput = manual3.oninput = function() {";
    html += "  var value = Math.min(slider3.value, manual3.value);";
    html += "  document.getElementById('sliderValue3').innerHTML = value;";
    html += "  var xhttp = new XMLHttpRequest();";
    html += "  xhttp.open('GET', '/slider3?value=' + value, true);";
    html += "  xhttp.send();";
    html += "};";

    html += "var slider4 = document.getElementById('slider4');";
    html += "var manual4 = document.getElementById('manual4');";
    html += "slider4.oninput = manual4.oninput = function() {";
    html += "  var value = Math.min(slider4.value, manual4.value);";
    html += "  document.getElementById('sliderValue4').innerHTML = value;";
    html += "  var xhttp = new XMLHttpRequest();";
    html += "  xhttp.open('GET', '/slider4?value=' + value, true);";
    html += "  xhttp.send();";
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
