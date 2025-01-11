#include <iostream>
using namespace std;

// Was going to make this abstract, but it is all shared functionality
// so far. Maybe a future refactor.
class OnScreenObject {
private:
  int xPos;
  int yPos;

public:
  OnScreenObject(int x, int y);

  virtual float move(int delX, int delY);

  virtual string info();

  virtual int getX() { return xPos; }
  virtual int getY() { return yPos; }

  // destructor
  virtual ~OnScreenObject();
};

class ALilGuy : public OnScreenObject {
private:
  string name;

public:
  ALilGuy(string name, int x, int y);

  string info();
};
