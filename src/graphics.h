#include "../lib/linmath.h"

typedef struct Vertex
{
    vec2 pos; // 2-space position
    vec3 col; // rgb color w/ float values
} Vertex;


struct TriangleData {
  bool doPause;
  float pausedAtTime;
  float elapsedPauseTime;
  float offsetX;
  float offsetY;
  float offsetZ;
};

void doEverything();
