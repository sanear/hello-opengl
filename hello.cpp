#include <iostream>
#include <cstdio>
#include <sstream>
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

// class is "abstract" b/c only pure virtual functions
class OnScreenObject {
public:
  int xPos;
  int yPos;
  OnScreenObject(int x, int y) : xPos(x), yPos(y) {}

  virtual float move(int delX, int delY) {
    xPos = xPos + delX;
    yPos = yPos + delY;
    return distance(delX, delY);
  }

  virtual string info() {
    ostringstream s;
    s << "pos: " << posToString(xPos, yPos);
    return s.str();
  }

  // destructor
  virtual ~OnScreenObject() {}
};

class ALilGuy : public OnScreenObject {
public:
  string name;
  ALilGuy(string name, int x, int y) : OnScreenObject(x, y), name(name) {}

  string info() {
    ostringstream s;
    s << "name: " << name.c_str() << "; " << OnScreenObject::info().c_str();
    return s.str();
  }
};



int main() {
  ALilGuy *guy = new ALilGuy("Ava", 0, 0);

  cout << endl;
  cout << "There's a guy! " << guy->info() << endl;
  int delX = 16;
  int delY = 32;
  cout << "They're moving " << delX << " units right and " << delY << " units up!" << endl;
  float dist = guy->move(delX, delY);
  cout << "They moved " << dist << " units!" << endl;
  cout << guy->info() << endl;
  return 0;
}
