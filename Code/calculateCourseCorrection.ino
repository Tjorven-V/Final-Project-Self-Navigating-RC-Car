unsigned long tLast = 0;

void calculateCourseCorrection() {
  int pwmThrottle = __pwmThrottle;
  int pwmSteering = __pwmSteering;
  int distMMLeft = __distMMLeft;
  int distMMRight = __distMMRight;
  int bias = __bias;

  int offsetLeft = _min(sensorRangeReactionMM, __distMMLeft);
  int offsetRight = _min(sensorRangeReactionMM, __distMMRight);

  ledcWrite(0, ((float)(__pwmThrottle)/(float)(20000))*pow(2, 12));
  if (__manualSteer) {
    ledcWrite(1, ((float)(pwmSteering)/(float)(20000))*pow(2, 12));
  } else {  
    bias = map(offsetRight, 0, sensorRangeReactionMM, 1, 1024) - map(offsetLeft, 0, sensorRangeReactionMM, 1, 1024);
    bias *= __biasMultiplier;
  
    ledcWrite(1, ((float)(1500 + map(bias, -1024, 1024, -500, 500))/(float)(20000))*pow(2, 12));
  }

  if (debug) {
    Serial.print("dT ");
    Serial.print(millis() - tLast);
    tLast = millis();
    
    Serial.print(F("\tThrtl    "));Serial.print(__pwmThrottle);
    Serial.print(F("\tStrng    "));Serial.print(__pwmSteering);
    Serial.print(F("\tLeft MM  "));Serial.print(offsetLeft);
    Serial.print(F("\tRight MM "));Serial.print(offsetRight);
    Serial.print(F("\tbias     "));Serial.print(bias);Serial.print("\t");Serial.print(bias == 0 ? F("STRAIGHT") : (bias < 0 ? F("TURN L") : F("TURN R")));
    Serial.print(F("\tCalc PWM: "));Serial.print(1500 + map(bias, -1024, 1024, -500, 500));
    Serial.println(); 
  }
}
