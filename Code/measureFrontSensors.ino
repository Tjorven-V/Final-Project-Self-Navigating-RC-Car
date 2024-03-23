Adafruit_VL53L0X sensorLeft = Adafruit_VL53L0X();
Adafruit_VL53L0X sensorRight = Adafruit_VL53L0X();

word lastMeasurementR;
word lastMeasurementL;

// pinSensorLeft
// pinSensorRight

void setup_measureFrontSensors() {
  pinMode(pinSensorXSHUTLeft, OUTPUT);
  pinMode(pinSensorXSHUTRight, OUTPUT);

  digitalWrite(pinSensorXSHUTLeft, HIGH);
  digitalWrite(pinSensorXSHUTRight, LOW);
  
  sensorRight.begin();
  sensorRight.setAddress(0x30);
  
  digitalWrite(pinSensorXSHUTLeft, HIGH);
  digitalWrite(pinSensorXSHUTRight, HIGH);

  sensorLeft.begin();

  // Adafruit Sensor Addresses
  // sensorLeft  0x29
  // sensorRight 0x30
}

VL53L0X_RangingMeasurementData_t measurementLeft;
VL53L0X_RangingMeasurementData_t measurementRight;

void loop_measureFrontSensors() {
  sensorLeft.rangingTest(&measurementLeft, false); // pass in 'true' to get debug data printout!
  sensorRight.rangingTest(&measurementRight, false); // pass in 'true' to get debug data printout!

  // print sensor one reading
  if (debug) Serial.print(F("L: "));
  if(measurementLeft.RangeStatus != 4 && measurementLeft.RangeMilliMeter < 8000) {     // if not out of range
    if (debug) Serial.print(measurementLeft.RangeMilliMeter);
    __distMMLeft = measurementLeft.RangeMilliMeter;
    lastMeasurementL =__distMMLeft;
  } else {
    if (debug) Serial.print(F("OOR"));
    __distMMLeft = sensorRangeReactionMM + 1;
  }
  
  if (debug) Serial.print(F(" "));

  // print sensor two reading
  if (debug) Serial.print(F("R: "));
  if(measurementRight.RangeStatus != 4 && measurementRight.RangeMilliMeter < 8000) {
    if (debug) Serial.print(measurementRight.RangeMilliMeter);
    __distMMRight = measurementRight.RangeMilliMeter;
    lastMeasurementR =__distMMRight;
  } else {
    if (debug) Serial.print(F("OOR"));
    __distMMRight = sensorRangeReactionMM + 1;
  }
  
  if (debug) Serial.println();
}
