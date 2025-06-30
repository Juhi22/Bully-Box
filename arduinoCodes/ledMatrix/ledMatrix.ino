#include <ArduinoSTL.h>
#include <map>
#include <vector>
#include <MD_MAX72xx.h>
#include <SPI.h>
using namespace std;

class Cursor {
  public:
    int matrixNumber;
    int column;
    int row;
    
    Cursor(){}
    
    Cursor(int m, int c, int r) {
      matrixNumber = m;
      column = c;
      row = r;
    }

    reset() {
      matrixNumber = 0;
      column = 7;
      row = B00000001;
    }

    bool operator==(const Cursor &a) {
      return (a.matrixNumber == matrixNumber) && (a.column == column) && (a.row == row);
    }
};

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS
#define NUMBEROF(x)  (sizeof(x) / sizeof((x)[0]))

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

int x = A0;
int y = A1;
bool isReadyToSetPin = false;
bool isPinSet = false;
Cursor cursor;
int stepNumber = 0;
Cursor steps[] = {
  Cursor(0, 7, B00000010), Cursor(0, 7, B00000100), Cursor(0, 6, B00000100), Cursor(0, 5, B00000100),
  Cursor(0, 5, B00001000), Cursor(0, 5, B00010000), Cursor(0, 5, B00100000), Cursor(0, 5, B01000000),
  Cursor(0, 5, B10000000), Cursor(1, 5, B00000001), Cursor(1, 4, B00000001), Cursor(1, 3, B00000001),
  Cursor(1, 2, B00000001), Cursor(1, 1, B00000001), Cursor(1, 0, B00000001), Cursor(3, 7, B00000001),
  Cursor(3, 6, B00000001), Cursor(3, 6, B00000010), Cursor(3, 6, B00000100), Cursor(3, 5, B00000100),
  Cursor(3, 4, B00000100), Cursor(3, 4, B00000010), Cursor(3, 4, B00000001), Cursor(2, 4, B10000000),
  Cursor(2, 4, B01000000), Cursor(2, 4, B00100000), Cursor(2, 4, B00010000), Cursor(2, 5, B00010000),
  Cursor(2, 6, B00010000), Cursor(2, 7, B00010000), Cursor(0, 0, B00010000), Cursor(0, 1, B00010000)
};
uint8_t one[COL_SIZE] = {
    0b00000000,
    0b00010000,
    0b00011000,
    0b00010100,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00000000
};
uint8_t four[COL_SIZE] = {
    0b00000000,
    0b00100100,
    0b00100100,
    0b00100100,
    0b00111100,
    0b00100000,
    0b00100000,
    0b00000000
};
uint8_t five[COL_SIZE] = {
    0b00000000,
    0b00111100,
    0b00000100,
    0b00111100,
    0b00100000,
    0b00100000,
    0b00111100,
    0b00000000
};
uint8_t eight[COL_SIZE] = {
    0b00000000,
    0b00111100,
    0b00100100,
    0b00111100,
    0b00100100,
    0b00100100,
    0b00111100,
    0b00000000
};

//history: key is matrix number, value is another map, that contains column as key and rows as vector as value
std::map<int,std::map<int,std::vector<int>>> history;
typedef std::map<int,std::map<int,std::vector<int>>>::iterator historyOuterIt;
typedef std::map<int,std::vector<int>>::iterator historyInnerIt;
typedef std::vector<int>::iterator rowListIt;

void setup() {
  Serial.begin(9600);
  mx.begin();
  reset();
}

void reset() {
  mx.clear();
  cursor.reset();
  stepNumber = 0;
  history.clear();
  std::vector<int> rowVector{B00000001};
  history[0][7] = rowVector;
  mx.setRow(cursor.matrixNumber, cursor.column, cursor.row);
}

void loop()
{
  if(stepNumber != NUMBEROF(steps)) {
    controlLed(analogRead(x), analogRead(y));
  }
  
  if(stepNumber == NUMBEROF(steps) && !isPinSet) {
    isReadyToSetPin = true;
  }
  
  if(isReadyToSetPin) {
    mx.clear();
    displayPinCode();
    isPinSet = true;
    isReadyToSetPin = false;
  }
}

void displayPinCode() {
  displayNumber(0, four);
  displayNumber(1, one);
  displayNumber(2, eight);
  displayNumber(3, five);
}

void displayNumber(int matrixNumber, uint8_t *bitmap) {
  int columnIndex = 7;
  for(int i = 0; i < COL_SIZE; i++) {
    mx.setRow(matrixNumber, columnIndex, bitmap[i]);
    columnIndex--;
  }
}

void controlLed(int xValue, int yValue) {
  if(100 <= xValue && xValue <= 900 && 100 <= yValue && yValue <= 900) {
    return;
  }

  if(xValue < 100) {
    if(cursor.row == 1 && (cursor.matrixNumber == 1 || cursor.matrixNumber == 3)) {
      cursor.matrixNumber -= 1;
      cursor.row = B10000000;
    } else {
      cursor.row = cursor.row / 2;
    }
  }

  if(xValue > 900) {
    if(cursor.row == 128 && (cursor.matrixNumber == 0 || cursor.matrixNumber == 2)) {
      cursor.matrixNumber += 1;
      cursor.row = B00000001;
    } else {
      cursor.row = cursor.row * 2;
    }
  }

  if(yValue < 100) {
    if(cursor.column == 7 && (cursor.matrixNumber == 2 || cursor.matrixNumber == 3)) {
      cursor.matrixNumber -= 2;
      cursor.column = 0;
    } else {
      cursor.column += 1;
    }
  }

  if(yValue > 900) {
    if(cursor.column == 0 && (cursor.matrixNumber == 0 || cursor.matrixNumber == 1)) {
      cursor.matrixNumber += 2;
      cursor.column = 7;
    } else {
      cursor.column -= 1;
    }
  }
  
  move();
}

void move() {
  if(NUMBEROF(steps) > stepNumber) {
    if(steps[stepNumber] == cursor) {
      mx.setRow(cursor.matrixNumber, cursor.column, cursor.row);
      stepNumber += 1;
      addStepToHistory();
      refreshPath();
      delay(200);
    } else {
      reset();
    }
  }
}

void addStepToHistory() {
  history[cursor.matrixNumber][cursor.column].push_back(cursor.row);
}

void refreshPath() {
  int row = B00000000;
  historyOuterIt outerIt = history.find(cursor.matrixNumber);
  historyInnerIt innerIt = outerIt->second.find(cursor.column);
  Serial.println("- - - - - - - - - - - -");
  for(rowListIt rowIt = innerIt->second.begin(); rowIt != innerIt->second.end(); rowIt++) {
    row += *rowIt;
  }
  mx.setRow(cursor.matrixNumber, cursor.column, row);
}