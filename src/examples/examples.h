#ifndef _EXAMPLES_H
#define _EXAMPLES_H

#include <vector>
#include "context.h"

#include "01-hello-world.h"
#include "02-element-buffer-object.h"
#include "03-draw-two-triangles.h"
#include "04-using-multiple-vao-and-vbo.h"

using namespace std;

typedef void (*ExampleFunc)(Context*);

extern vector<ExampleFunc> examples;

#endif
