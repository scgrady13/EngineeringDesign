#include <conductivity.h>
// Salinity Sensor code

// ten samples may be way too overkill here, I guess we can just go with 3 for this sprint - 30% 60% 90% salinity, then hopefully transition to 10 samples for better accuracy
// I bet if we did ten samples for this sprint though we would get bonus points for putting in the effort to do so.
  int samples[2][10] { { 10, 15, 20, 25, 30, 40, 50, 60, 70, 80}, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }; //array holding corresponding sample salinity % and adc return code
                                                                                                   //return codes to be filled in as calibration completes
 // write lowerPlates and raisePlates functionality
 // perhaps for this sprint simplify samples to only 3 samples, make different array and comment out extra readings in setup

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  int conductivityReading = 0;
  
  // fill full calibration data array - test 10 different samples and store adc codes in 2d array
  // lowerPlates;
  conductivityReading = getConductivity();
  calibrationFunction(0, conductivityReading);
  // raisePlates;
  // delay(5000); //just enough delay time to swap out the beaker with the sample in it.
  
  // lowerPlates;
  conductivityReading = getConductivity();
  calibrationFunction(1, conductivityReading);
  // raisePlates;
  // delay(5000);

  // lowerPlates;
  conductivityReading = getConductivity();
  calibrationFunction(2, conductivityReading);
  // raisePlates;
  // delay(5000);

  // lowerPlates;
  conductivityReading = getConductivity();
  calibrationFunction(3, conductivityReading);
  // raisePlates;
  // delay(5000);

  // lowerPlates;
  conductivityReading = getConductivity();
  calibrationFunction(4, conductivityReading);
  // raisePlates;
  // delay(5000);

  // lowerPlates;
  conductivityReading = getConductivity();
  calibrationFunction(5, conductivityReading);
  // raisePlates;
  // delay(5000);

  // lowerPlates;
  conductivityReading = getConductivity();
  calibrationFunction(6, conductivityReading);
  // raisePlates;
  // delay(5000);

  // lowerPlates;
  conductivityReading = getConductivity();
  calibrationFunction(7, conductivityReading);
  // raisePlates;
  // delay(5000);

  // lowerPlates;
  conductivityReading = getConductivity();
  calibrationFunction(8, conductivityReading);
  // raisePlates;
  // delay(5000);

  // lowerPlates;
  conductivityReading = getConductivity();
  calibrationFunction(9, conductivityReading);
  // raisePlates;
  // delay(5000);
}

void calibrationFunction (int col, int reading) {
  samples[1][col] = reading;
}

double calculatePercentSalinity (int unknownADC) {  // test array functionality in CLion. Algorithm works when given values, now need to find out if it holds up for real data.
  
  int leftPercent = 0, rightPercent = 0;
  int leftADC = 0, rightADC = 0;

  // If-else statements to assign left and right %s and ADC values from samples array
  // Pretty brute force but should work
  if (0 <= unknownADC && unknownADC <= samples[1][0]) { // between 0 and 10% salinity
      leftPercent = 0;
      leftADC = 0;
      rightPercent = samples[0][0];
      rightADC = samples[1][0];
  }
  else if (samples[1][0] <= unknownADC && unknownADC <= samples[1][1] - 1) { //between 10 and 15% salinity
      leftPercent = samples[0][0];
      leftADC = samples[1][0];
      rightPercent = samples[0][1];
      rightADC = samples[1][1];
  }
    else if (samples[1][1] <= unknownADC && unknownADC <= samples[1][2] - 1) { //between 15 and 20% salinity
      leftPercent = samples[0][1];
      leftADC = samples[1][1];
      rightPercent = samples[0][2];
      rightADC = samples[1][2];
  }
  else if (samples[1][2] <= unknownADC && unknownADC <= samples[1][3] - 1) { //between 20 and 25% salinity
      leftPercent = samples[0][2];
      leftADC = samples[1][2];
      rightPercent = samples[0][3];
      rightADC = samples[1][3];
  }
  else if (samples[1][3] <= unknownADC && unknownADC <= samples[1][4] - 1) { // between 25 and 30% salinity
      leftPercent = samples[0][3];
      leftADC = samples[1][3];
      rightPercent = samples[0][4];
      rightADC = samples[1][4];
  }
  else if (samples[1][4] <= unknownADC && unknownADC <= samples[1][5] - 1) { // between 30 and 40% salinity
      leftPercent = samples[0][4];
      leftADC = samples[1][4];
      rightPercent = samples[0][5];
      rightADC = samples[1][5];
  }
  else if (samples[1][5] <= unknownADC && unknownADC <= samples[1][6] - 1) { // between 40 and 50% salinity
      leftPercent = samples[0][5];
      leftADC = samples[1][5];
      rightPercent = samples[0][6];
      rightADC = samples[1][6];
  }
  else if (samples[1][6] <= unknownADC && unknownADC <= samples[1][7] - 1) { // between 50 and 60% salinity 
      leftPercent = samples[0][6];
      leftADC = samples[1][6];
      rightPercent = samples[0][7];
      rightADC = samples[1][7];
  }
  else if (samples[1][7] <= unknownADC && unknownADC <= samples[1][8] - 1) { // between 60 and 70% salinity
      leftPercent = samples[0][7];
      leftADC = samples[1][7];
      rightPercent = samples[0][8];
      rightADC = samples[1][8];
  }
  else if (samples[1][8] <= unknownADC && unknownADC <= samples[1][9] - 1) { // between 70 and 80% salinity
      leftPercent = samples[0]8];
      leftADC = samples[1][8];
      rightPercent = samples[0][9];
      rightADC = samples[1][9];
  }
  else if (samples[1][9] <= unknownADC && unknownADC <= 1023) { // greater than 80% salinity. less accurate than statements above, right is assumed to be max possible % and ADC
      leftPercent = samples[0][9];
      leftADC = samples[1][9];
      rightPercent = 100;
      rightADC = 1023;
  }
  else {
      Serial.println("unknown ADC code not in range of samples array.");
  }
  
  double ADCdiff = 0.0;
  double rightMinusUnknownADC = 0.0;
  double rightMinusUnknownADCoverADCdiff = 0.0;
  double percentDifferenceFromLeft = 0.0;
  double percentBetweenAdjacentSamples = 0.0;
  double percentToBeAdded = 0.0;
  double salinityPercentGuess = 0.0;
  
  ADCdiff = fabs((double)rightADC - (double)leftADC); // check if math.h is needed for fabs functions
  rightMinusUnknownADC = (double)rightADC - (double)unknownADC;
  rightMinusUnknownADCoverADCdiff = rightMinusUnknownADC / ADCdiff;
  percentDifferenceFromLeft = 1 - rightMinusUnknownADCoverADCdiff;
  percentBetweenAdjacentSamples = fabs((double)rightPercent - (double)leftPercent); // check if math.h is needed for fabs functions
  percentToBeAdded = percentDifferenceFromLeft * percentBetweenAdjacentSamples;
  salinityPercentGuess = (double)leftPercent + percentToBeAdded;
  
  return salinityPercentGuess;
}


void loop() {
  // put your main code here, to run repeatedly:
  conductivityReading = getConductivity();
  Serial.println("Unknown ADC code:");
  Serial.println(conductivityReading);
  salinity = calculatePercentSalinity(conductivityReading);
  Serial.println("Salinity % of unknown:");
  Serial.println(salinity);
  delay(30000);
}
