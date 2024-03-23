volatile unsigned long pwmDeltaMode = 0;
volatile unsigned long pwmDutyTimeMode = 0;
volatile bool measurementFinishedMode = false;

void setup_measurePWMMode() {
  pinMode(pwmInMode, INPUT);
  attachInterrupt(digitalPinToInterrupt(pwmInMode), ISR_Mode_RISING, RISING);
}

void ISR_Mode_RISING() {
  attachInterrupt(digitalPinToInterrupt(pwmInMode), ISR_Mode_FALLING, FALLING); 

  pwmDeltaMode = micros();
}

void ISR_Mode_FALLING() {
  attachInterrupt(digitalPinToInterrupt(pwmInMode), ISR_Mode_RISING, RISING);  

  pwmDutyTimeMode = micros() - pwmDeltaMode;
  measurementFinishedMode = true;
}

void loop_measurePWMMode() {
  if (measurementFinishedMode) {
    measurementFinishedMode = false;
    float val = pwmDutyTimeMode;

    // Remap read raw value between 1 and 2ms
    val = _max(steeringRange[0], val);
    val = _min(steeringRange[1], val);
    val = map(val, steeringRange[0], steeringRange[1], 1000, 2000);
    if (1500 <= val && val <= 1550) val = 1500;
    float PWM_HiTime = (float) val;

    __manualSteer = (bool) (PWM_HiTime < 1500); // Globally available
  
    if (debug == true) {
      Serial.print("md\t");
      Serial.println((int)(__manualSteer));   
    }
    
  }
}
