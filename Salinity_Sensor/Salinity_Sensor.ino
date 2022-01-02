#include <Servo.h>
#include <conductivity.h>
#include <Sleep_n0m1.h>
#include <EEPROM.h>
#include <eepromhelper.h>

Servo serv;

int angleS;
int outAngle;
int inAngle;

Sleep sleep;

unsigned long timeStamp = 0;
unsigned long elapsedTime;
unsigned long sleepVal = 0;
double salinity;
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const double slope = 0; // pink: 0.4107534247 | white: 0.4018972603
const double intercept = 0; // pink: 280.4068493 | white: 407.6245205
//--------------------------------------------------------------------
const unsigned long collectionInterval = 600000; // 10 minutes = 600000
const unsigned long runTime = 14400000; // 4 hours = 14400000

const bool collectData = true; // Change true/false - Change collection/extraction
//--------------------------------------------------------------------
const int calibrateOutWater = 0;
const int calibrateInWater = 145; // For calibration

const int outWater = 0;
const int inWater = 145; // For salinity data collection
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const bool calibrate = (slope == 0.0 && intercept == 0.0) && collectData;
const unsigned int numberOfDataSamples = runTime / collectionInterval;

void setup()
{
	// put your setup code here, to run once:
	sleep.pwrDownMode();
	Serial.begin(9600);
	serv.write(0);
	serv.attach(9);
	Serial.println();
	if (!calibrate) {
		RunSalinitySensor();
	}
	else {
		Serial.println("Calibrating:");
		outAngle = calibrateOutWater;
		inAngle = calibrateInWater;
	}
}

void loop()
{
	//  put your main code here, to run repeatedly:
	if (calibrate) {
		CalibrationRun();
		delay(15000);
	}
}

void RunSalinitySensor() {
	outAngle = outWater;
	inAngle = inWater;

	if (collectData) {
		//resetSampleCount();
		Serial.println("Collecting Data:");
		for (int i = 0; i < numberOfDataSamples; i++) {
			double ADCReading = ReadADC();
			salinity = myCalibrationFunction(ADCReading);
			elapsedTime = millis();
			Datastorage();

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

			Serial.print(" -> salinity = ");
			salinity = ((double)getIntValueAtAddress(l)) / 100000;
			Serial.print(10 * salinity, 5);
			Serial.println("%");
			l += 2;
		}
	}
}

void CalibrationRun() {
	double ADCReading = ReadADC();
	Serial.print("ADC Reading: ");
	Serial.println(ADCReading);
}

double ReadADC() {
	double ADCReading;
	double sum = 0;

	angleS = serv.read();
	while (angleS > outAngle) {
		angleS--;
		serv.write(angleS);
		delay(50);
	}
	while (angleS < inAngle) {
		angleS++;
		serv.write(angleS);
		delay(50);
	}
	delay(5000);
	for (int j = 0; j < 3; j++) {
		sum += (double)getConductivity();
	}
	while (angleS > outAngle) {
		angleS--;
		serv.write(angleS);
		delay(50);
	}
	ADCReading = 0;
	ADCReading = (sum / 3.0);
	return ADCReading;
}

double myCalibrationFunction(double adc) {
	double value = slope / (adc - intercept);
	//Serial.print("Salinity: ");
	//Serial.println(value);
	return value;
}

void Datastorage() {
	int address;
	Serial.print("Time: ");
	Serial.println(ConvertTime((timeStamp + elapsedTime) / 1000));
	address = appendIntValueToEEPROM((timeStamp + elapsedTime) / 1000);

	Serial.print("Salinity: ");
	Serial.print(10 * salinity, 5);
	Serial.println("%");
	address = appendIntValueToEEPROM((int)(salinity * 100000));
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