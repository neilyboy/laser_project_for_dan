// Define a variable to keep track of the last time slider data was received
unsigned long lastSliderUpdateMillis = 0;
// Define the minimum interval between slider data updates in milliseconds
const unsigned long sliderUpdateInterval = 1000; // Adjust the interval as needed

// In your loop() function, check the interval before processing slider data
void loop() {
  // Check if it's time to process slider data
  unsigned long currentMillis = millis();
  if (currentMillis - lastSliderUpdateMillis >= sliderUpdateInterval) {
    // Process slider data here
    stepper1.run();
    stepper2.run();
    stepper3.run();
    stepper4.run();

    // Update the last update time
    lastSliderUpdateMillis = currentMillis;
  }
}
