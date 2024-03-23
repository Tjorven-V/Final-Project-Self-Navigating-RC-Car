volatile unsigned long pwmDeltaThrottle = 0;
volatile unsigned long pwmDutyTimeThrottle = 0;
volatile bool measurementFinishedThrottle = false;

void setup_measurePWMThrottle() {
  pinMode(pwmInThrottle, INPUT);
  attachInterrupt(digitalPinToInterrupt(pwmInThrottle), ISR_Throttle_RISING, RISING);

  // Do not set OUTPUT pins in calibration mode!
  if (calibration == true) return;
  //pinMode(pwmOutThrottle, OUTPUT);

  // channel, frequency, bit resolution
  ledcSetup(0, 50, 12);
  ledcAttachPin(pwmOutThrottle, 0);
}

void ISR_Throttle_RISING() {
  attachInterrupt(digitalPinToInterrupt(pwmInThrottle), ISR_Throttle_FALLING, FALLING); 

  pwmDeltaThrottle = micros();
}

void ISR_Throttle_FALLING() {
  attachInterrupt(digitalPinToInterrupt(pwmInThrottle), ISR_Throttle_RISING, RISING);  

  pwmDutyTimeThrottle = micros() - pwmDeltaThrottle;
  measurementFinishedThrottle = true;
}

void loop_measurePWMThrottle() {
  if (measurementFinishedThrottle) { 
    measurementFinishedThrottle = false;
    float val = pwmDutyTimeThrottle;

    if (calibration == true) {
      Serial.print("Snelheid: ");
      Serial.println(val);
      return; // Stop here when calibrating
    };

    // Remap read raw value between 1 and 2ms
    val = _max(throttleRange[0], val);
    val = _min(throttleRange[1], val);
    val = map(val, throttleRange[0], throttleRange[1], 1000, 2000);
    if (1400 <= val && val <= 1500) val = 1450;

    if (pwmMaxThrottle != 0) val = _min(pwmMaxThrottle, val);
    float PWM_HiTime = (float) val;
    if (debug == true) {
      Serial.print("sn\t");
      Serial.println((int)(PWM_HiTime));   
    }

    // PWM_HiTime is time in microseconds that signal was HIGH
    // The total signal time for this transmitter is 20'000 microseconds.

    __pwmThrottle = (int) PWM_HiTime; // Globally available

    //Timer1.pwm(pwmOutThrottle, (int)(PWM_HiTime/_PWMCycleTime*1023), _PWMCycleTime);
    
  }
}
