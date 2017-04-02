#pragma config(Sensor, S1,     leftBumper,    sensorEV3_Touch)
#pragma config(Sensor, S2,     rightBumper,   sensorEV3_Touch)
#pragma config(Sensor, S3,     colourSensor,  sensorEV3_Color)
#pragma config(Sensor, S4,     sonar,          sensorEV3_Ultrasonic)
//*!!Code automatically generated by 'ROBOTC' configuration wizard        !!*//

/**
* returns true if the reflected colour sensor sensors light levels less than
* the threshold.
*/
bool black(int threshold) {
	return getColorReflected(colourSensor) < threshold;
}

/**
* returns true if the reflected colour sensor sensors light levels greater than
* the threshold.
*/
bool white(int threshold) {
	return getColorReflected(colourSensor) > threshold;
}

/**
* Pivots clockwise until the sonar sensor detects something closer than the
* threshold distance, or has done a full 360. If an object has been detected
* close by the robot aligns itself with that object. If the robot has done a
* full 30 and not found anything, it moves forward, and looks again.
*/
void locateTower(int distanceThreshold) {
	 int turnSpeed = 10;
	resetMotorEncoder(motorB);

	while(getUSDistance(sonar) > distanceThreshold) {
		setMotorSyncEncoder(motorB, motorC, 100, 10, turnSpeed);
		waitUntilMotorStop(motorB);

		if(getMotorEncoder(motorB) > 700) {
			playSound(soundBeepBeep);
			setMotorSyncEncoder(motorB, motorC, 0, 700, 50);
			waitUntilMotorStop(motorB);
			resetMotorEncoder(motorB);
		}
	}
	stopAllMotors();

	int distanceToTower = getUSDistance(sonar);

	// turn left until tower is gone
	setMotorSync(motorB, motorC, -100, turnSpeed);
	waitUntil(getUSDistance(sonar) > 10 + distanceToTower);
	stopAllMotors();

	// turn right until tower is there again
	setMotorSync(motorB, motorC, 100, turnSpeed);
	waitUntil(getUSDistance(sonar) < 10 + distanceToTower);
	stopAllMotors();

	// start counting
	resetMotorEncoder(motorC);

	// turn right until tower is gone
	setMotorSync(motorB, motorC, 100, turnSpeed);
	waitUntil(getUSDistance(sonar) > 10 + distanceToTower);
	stopAllMotors();

	float angleToPivotBack = getMotorEncoder(motorC) * 0.6;
	setMotorSyncEncoder(motorB, motorC, -100, angleToPivotBack, 10);
	waitUntilMotorStop(motorB);
}

void straightenUp(){
	int threshold = 30;
	int turnSpeed = 10;
	// inch forward a bit first
	setMotorSyncEncoder(motorB, motorC, 0, 30, turnSpeed);
	waitUntilMotorStop(motorB);
	resetMotorEncoder(motorB);

	// turn right until black then stop
	setMotorSync(motorB, motorC, 100, turnSpeed);
	waitUntil(black(threshold));
	stopAllMotors();

	float firstAngle = getMotorEncoder(motorB);
	displayTextLine(1, "first angle %d", firstAngle);

	// turn left until white
	setMotorSync(motorB, motorC, -100, turnSpeed);
	waitUntil(white(threshold));
	resetMotorEncoder(motorC);

	// turn left until black
	setMotorSync(motorB, motorC, -100, turnSpeed);
	waitUntil(black(threshold));

	// turn right until white then stop
	setMotorSync(motorB, motorC, 100, turnSpeed);
	waitUntil(white(threshold));
	stopAllMotors();

	float secondAngle = getMotorEncoder(motorC);
	displayTextLine(2, "second angle %d", secondAngle);

	float encoderCounts;

	if(secondAngle > 120){
		encoderCounts = secondAngle / 2;
	} else {
		float ratio = (firstAngle/secondAngle) * 100;
		if(ratio < 70) {
			encoderCounts = secondAngle * 0.75;
		} else {
			encoderCounts = secondAngle / 4;
		}
	}
	setMotorSyncEncoder(motorB, motorC, 100, encoderCounts, turnSpeed);
	waitUntilMotorStop(motorB);
}

void goForwardFor(int tileTarget) {
	int tilesBeen = 0;
	while(tilesBeen < tileTarget) {
		setMotorSync(motorB, motorC, 0, 15);
		waitUntil(black(30));
		playSound(soundBlip);
		tilesBeen++;
		waitUntil(white(30));
		stopAllMotors();
		straightenUp();
	}
	stopAllMotors();
}


task main()
{
	setMotorSyncEncoder(motorB, motorC, 0, 250, 15);
	waitUntilMotorStop(motorB);

	setMotorSync(motorB, motorC, 50, 10);
	playSound(soundBlip);
	waitUntil(white(30));
	stopAllMotors();
	straightenUp();

	goForwardFor(13);

	setMotorSync(motorB, motorC, 0, 15);
	waitUntil(black(30));
	playSound(soundBlip);

	setMotorSyncEncoder(motorB, motorC, 50, 350, 10);
	waitUntilMotorStop(motorB);

	playSound(soundDownwardTones);

	setMotorSyncEncoder(motorB, motorC, 0, 4440, 70);
	waitUntilMotorStop(motorB);

	locateTower(200);

	resetMotorEncoder(motorB);
	while(getUSDistance(sonar) > 10 && SensorValue[rightBumper] == 0 && SensorValue[leftBumper] == 0){
		if(getMotorEncoder(motorB) > 300) {
			stopAllMotors();
			locateTower(60);
		}
		setMotorSync(motorB, motorC, 0, 50);
	}
	setMotorSync(motorB, motorC, 0, 100);
	wait1Msec(1500);
	stopAllMotors();

	playSound(soundUpwardTones);
	wait1Msec(1500);
}
