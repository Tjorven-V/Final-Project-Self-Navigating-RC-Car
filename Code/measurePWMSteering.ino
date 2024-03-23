volatile unsigned long pwmDeltaSteering = 0;
volatile unsigned long pwmDutyTimeSteering = 0;
volatile bool measurementFinishedSteering = false;

void setup_measurePWMSteering() {
  pinMode(pwmInSteering, INPUT);
  attachInterrupt(digitalPinToInterrupt(pwmInSteering), ISR_Steering_RISING, RISING);

  // Do not set OUTPUT pins in calibration mode!
  if (calibration == true) return;
  //pinMode(pwmOutSteering, OUTPUT);

  // channel, frequency, bit resolution
  ledcSetup(1, 50, 12);
  ledcAttachPin(pwmOutSteering, 1);
}

void ISR_Steering_RISING() {
  attachInterrupt(digitalPinToInterrupt(pwmInSteering), ISR_Steering_FALLING, FALLING); 

  pwmDeltaSteering = micros();
}

void ISR_Steering_FALLING() {
  attachInterrupt(digitalPinToInterrupt(pwmInSteering), ISR_Steering_RISING, RISING);  

  pwmDutyTimeSteering = micros() - pwmDeltaSteering;
  measurementFinishedSteering = true;
}

void loop_measurePWMSteering() {
  if (__manualSteer) return;
  
  if (measurementFinishedSteering) {
    measurementFinishedSteering = false;
    float val = pwmDutyTimeSteering;

    if (calibration == true) {
      Serial.print("Sturing: ");
      Serial.println(val);
      return; // Stop here when calibrating 
    };

    // Remap read raw value between 1 and 2ms
    val = _max(steeringRange[0], val);
    val = _min(steeringRange[1], val);
    val = map(val, steeringRange[0], steeringRange[1], 1000, 2000);
    if (1500 <= val && val <= 1550) val = 1500;
    float PWM_HiTime = (float) val;
  
    if (debug == true) {
      Serial.print("st\t");
      Serial.println((int)(PWM_HiTime));   
    }

    // PWM_HiTime is time in microseconds that signal was HIGH
    // The total signal time for this transmitter is 20'000 microseconds.

    __pwmSteering = (int) PWM_HiTime; // Globally available

    //Timer1.pwm(pwmOutSteering, (int)(PWM_HiTime/_PWMCycleTime*1023), _PWMCycleTime);

  }
}
