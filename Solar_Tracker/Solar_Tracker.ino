#include <Servo.h>5
#include <Sleep_n0m1.h>
#include <EEPROM.h>
#include <eepromhelper.h>

int res1 = A13; // N - 1
int res2 = A14; // E - 2
int res3 = A11; // S - 3
int res4 = A12; // W - 4
int res5 = A10; // Up - 5
int resVals[] = { res1, res2, res3, res4, res5 };
int resPins[] = { res1, res2, res3, res4, res5 };

int cellPin = A0; // Photo-voltaic Cell
Servo baseServo;
Servo panelServo;

int firMax;
int firMaxRes;
int secMax;
int secMaxRes;
int leastVal;

Sleep sleep;

unsigned long timeStamp = 0;
unsigned long elapsedTime;
unsigned long sleepVal = 0;
double volts;
//--------------------------------------------------------------------
const unsigned long collectionInterval = 600000; // 10 minutes = 600000
const unsigned long runTime = 28800000; // 8 hours = 28800000

const bool collectData = true; // Change true/false - Change collection/extraction
//--------------------------------------------------------------------
const unsigned int numberOfDataSamples = runTime / collectionInterval;

void setup()
{
	// put your setup code here, to run once:

	sleep.pwrDownMode();
	Serial.begin(9600);
	baseServo.attach(9);
	panelServo.attach(8);
	baseServo.write(0);
	panelServo.write(0);
	//delay(10000);
	Serial.println();
	RunSolarTracker();
}

void loop()
{
	//  put your main code here, to run repeatedly:
}

void RunSolarTracker() {
	if (collectData) {
		//resetSampleCount();
		Serial.println("Collecting Data:");
		for (int i = 0; i < numberOfDataSamples; i++) {
			ReadResistors();
			StatsCreator();
			SetServo();
			ReadVoltage();
			Datastorage();
			delay(1000);
			elapsedTime = millis();
			sleepVal = collectionInterval - elapsedTime;
			sleep.sleepDelay(sleepVal);
			timeStamp += collectionInterval;
		}
	}
	else {
		Serial.println("Extracting Collected Data:");
		int l = 2;
		for (int i = 0; i < getCountOfDataSamples(); i += 2) {
			Serial.print("Time = ");
			timeStamp = getIntValueAtAddress(l);
			Serial.print(ConvertTime(timeStamp));
			l += 2;

			Serial.print(" -> Volts = ");
			volts = ((double)getIntValueAtAddress(l)) / 10000;
			Serial.println(volts, 4);
			l += 2;
		}
	}
}

void ReadResistors()
{
	//Serial.print("Photo-resistors: ");
	for (int j = 0; j < 5; j++)
	{
		resVals[j] = analogRead(resPins[j]);
	}
	//for (int j = 0; j < 5; j++)
	//{
	//	Serial.print(resVals[j]);
	//	Serial.print(" ");
	//}
	//Serial.println();
}

void ReadVoltage() {
	double value = analogRead(cellPin);
	value = (value / 1023.0) * 5.0;
	value *= 1.51;
	//Serial.print("Voltage: ");
	//Serial.println(value);
	volts = value;
	elapsedTime = millis();
}

void StatsCreator() {
	ReverseRes();

	firMax = 0;
	firMaxRes = 0;
	secMax = 0;
	secMaxRes = 0;
	leastVal = resVals[0];
	for (int k = 0; k < 4; k++) {
		if (resVals[k] > firMax) {
			if (firMax > secMax) {
				secMax = firMax;
				secMaxRes = firMaxRes;
			}
			firMax = resVals[k];
			firMaxRes = k;
		}
		else if (resVals[k] > secMax) {
			secMax = resVals[k];
			secMaxRes = k;
		}
		if (resVals[k] < leastVal) {
			leastVal = resVals[k];
		}
	}
}

void SetServo()
{
	int hAngle;
	int vAngle;
	bool leftSide = true;
	int diff;

	if ((firMaxRes == 0) && (secMaxRes == 1)) { //N, E
		diff = resVals[0] - resVals[1];
		hAngle = map(diff, 0, (firMax - leastVal), 135, 180);
		leftSide = false;
	}
	else if ((firMaxRes == 1) && (secMaxRes == 0)) { //E, N
		diff = resVals[1] - resVals[0];
		hAngle = map(diff, 0, (firMax - leastVal), 135, 90);
		leftSide = false;
	}
	else if ((firMaxRes == 1) && (secMaxRes == 2)) { //E, S
		diff = resVals[1] - resVals[2];
		hAngle = map(diff, 0, (firMax - leastVal), 45, 90);
		leftSide = false;
	}
	else if ((firMaxRes == 2) && (secMaxRes == 1)) { //S, E
		diff = resVals[2] - resVals[1];
		hAngle = map(diff, 0, (firMax - leastVal), 45, 0);
		leftSide = false;
	}
	else if ((firMaxRes == 2) && (secMaxRes == 3)) { //S, W
		diff = resVals[2] - resVals[3];
		hAngle = map(diff, 0, (firMax - leastVal), 135, 180);
	}
	else if ((firMaxRes == 3) && (secMaxRes == 2)) { //W, S
		diff = resVals[3] - resVals[2];
		hAngle = map(diff, 0, (firMax - leastVal), 135, 90);
	}
	else if ((firMaxRes == 3) && (secMaxRes == 0)) { //W, N
		diff = resVals[3] - resVals[0];
		hAngle = map(diff, 0, (firMax - leastVal), 45, 90);
	}
	else if ((firMaxRes == 0) && (secMaxRes == 3)) { //N, W
		diff = resVals[0] - resVals[3];
		hAngle = map(diff, 0, (firMax - leastVal), 45, 0);
	}
	else {
		Serial.println("Error");
	}

	if (leftSide) {
		vAngle = map(resVals[4], 0, 1023, 0, 90);
	}
	else if (!leftSide) {
		vAngle = map(resVals[4], 0, 1023, 180, 90);
	}
	baseServo.write(hAngle);
	panelServo.write(vAngle);
}

void ReverseRes() {
	for (int h = 0; h < 5; h++)
	{
		resVals[h] = map(resVals[h], 0, 1023, 1023, 0);
	}
}

void Datastorage() {
	int address;
	Serial.print("Time: ");
	Serial.println(ConvertTime((timeStamp + elapsedTime) / 1000));
	address = appendIntValueToEEPROM((timeStamp + elapsedTime) / 1000);

	Serial.print("Volts: ");
	Serial.println(volts, 4);
	address = appendIntValueToEEPROM((int)(volts * 10000));
	Serial.flush();
}

String ConvertTime(int time) {
	int hours, minutes, seconds;
	hours = time / 3600;
	minutes = (time % 3600) / 60;
	seconds = time % 60;
	String colon = ":";
	return hours + colon + minutes + colon + seconds;
}
