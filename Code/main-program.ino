
    /*
   GIP Project   |   Zelfrijdende RC Auto
   Tjorven --- & Deano ---


   - Adafruit_VL35L0X: https://github.com/adafruit/Adafruit_VL53L0X
      Library om via I2C te-interfacen met de sensoren gemonteerd
      op de voorbumper. Deze library is mogelijk tijdelijk, aangezien
      dat deze sensoren wat te traag zijn.  */
#include <Adafruit_VL53L0X.h>

/*
   Print debug data to the Serial Monitor. You should definitely disable
   this if you're using it in the car, otherwise generated PWM signals may be
   severely distorted.
*/
const bool debug = true;

/*
   Set 'calibration' to true to print the raw PWM timings to the Serial Monitor
   These values should be input in sturingLoHi & snelheidLoHi. The first value of
   each array should be the lowest value shown in the serial monitor for each action,
   ex. lowest value of 410 when braking, so snelheidLoHi's first value should be 410
   and the same for the second value, but with the highest value shown in the Serial Monitor
   for the associated action.

   If the high/low values in the Serial Monitor are jittery, it's recommended you round them;
   For the lowest, round it to the highest average number you see
   For the highest, round it to the lowest average number you see
*/
const bool calibration = false;
const int steeringRange[2] = {1050, 1850};
const int throttleRange[2] = {950, 1950};

// 21 AND 22 CANNOT BE USED ON ESP32 - THESE ARE SDA AND SCL FOR I2C

// MUST BE INTERRUPT PINS (2 & 3 on Arduino UNO)
// CAN BE ANY ON ESP32
const byte pwmInThrottle = 12;
const byte pwmInMode = 13;
const byte pwmInSteering = 14;

// MUST BE PINS ON TIMER1 (9 & 10 on Arduino UNO)
// CAN BE ANY ON ESP32
const byte pwmOutThrottle = 26;
const byte pwmOutSteering = 27;

// ANY FREE DIGITAL PINS
const byte pinSensorXSHUTLeft = 32;
const byte pinSensorXSHUTRight = 33;

// Time in microseconds of each PWM cycle we'll be measuring & generating
// Our transmitter is 50Hz -> 1s/50Hz -> 0.02s -> 20ms -> 20'000µs
const int _PWMCycleTime = 20000;

// Range we start turning away, in millimeters
// If the ranged distance is shorter than this,
// the car will start turning away
const int sensorRangeReactionMM = 500;

// in microseconds how much the PWM Throttle signal
// should max be generated as high. The car will not
// be able to accelerate beyond this value.
// Set to 0 to disable max throttle.
const int pwmMaxThrottle = 1700;

/*
   ❗❗❗==========================================❗❗❗
        These variables should NOT be edited!
   They are used by the program throughout runtime
   ❗❗❗==========================================❗❗❗
*/
word __bias = 0; // Steering bias - program -1024 to 1024
float __biasMultiplier = 2.5; // <1 steers less, >1 steers more. 2.5 seems good.
word __pwmThrottle = 0; // Time Throttle signal is high in microseconds
bool __manualSteer = false; // Manual or automatic steering
word __pwmSteering = 0; // Time Steer signal is high in microseconds
word __distMMLeft = 0; // Distance between left corner of bumper and course in millimeters
word __distMMRight = 0; // Distance between right corner of bumper and course in millimeters

void setup() {
  if (debug == true || calibration == true) {
    Serial.begin(115200);
    while (!Serial); // Wait for Serial to become available
    Serial.println("SERIAL READY");
  }
  if (calibration == true) {
    Serial.println(F("CALIBRATION MODE ACTIVE - NO PWM SIGNALS WILL BE GENERATED"));
    Serial.println(F("Continuing in 5s..."));
    delay(5000);
  }
  
  // Functions on other tabs
  if (debug) Serial.println("setup_measureFrontSensors()");
  setup_measureFrontSensors();
  if (debug) Serial.println("OK");
  
  if (debug) Serial.println("setup_measurePWMThrottle()");
  setup_measurePWMThrottle();
  if (debug) Serial.println("OK");

  if (debug) Serial.println("setup_measurePWMMode()");
  setup_measurePWMMode();
  if (debug) Serial.println("OK");
  
  if (debug) Serial.println("setup_measurePWMSteering()");
  setup_measurePWMSteering();
  if (debug) Serial.println("OK");
}

void loop() {
  // Functions on other tabs
  loop_measureFrontSensors();
  loop_measurePWMThrottle();
  loop_measurePWMMode();
  loop_measurePWMSteering();
  
  calculateCourseCorrection();
}
