#include <iostream>
#include <sstream>
#include "models.h"
using namespace std;

int recursivePow(int x, int y) {
  if (y == 1) {
    return x;
  }
  return x * recursivePow(x, y-1);
}

float distance(int delX, int delY) {
  return sqrt(static_cast<float>(recursivePow(delX, 2) + recursivePow(delY, 2)));
}

string posToString(int x, int y) {
  ostringstream s;
  s << "(" << x << "," << y << ")";
  return s.str();
}

OnScreenObject::OnScreenObject(int x, int y) : xPos(x), yPos(y) {}
float OnScreenObject::move(int delX, int delY) {
    xPos = xPos + delX;
    yPos = yPos + delY;
    return distance(delX, delY);
}
string OnScreenObject::info() {
    ostringstream s;
    s << "pos: " << posToString(xPos, yPos);
    return s.str();
  }
OnScreenObject::~OnScreenObject() {}

ALilGuy::ALilGuy(string name, int x, int y) : OnScreenObject(x, y), name(name) {}

string ALilGuy::info() {
    ostringstream s;
    s << "name: " << name.c_str() << "; " << OnScreenObject::info().c_str();
    return s.str();
}
