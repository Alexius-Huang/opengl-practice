#ifndef _EXAMPLES_H
#define _EXAMPLES_H

#include <vector>
#include <string>
#include "context.h"

#include "01-hello-world/main.h"
#include "02-element-buffer-object/main.h"
#include "03-draw-two-triangles/main.h"
#include "04-using-multiple-vao-and-vbo/main.h"
#include "05-using-multiple-shaders/main.h"
#include "06-pass-data-between-shaders/main.h"

using namespace std;

typedef void (*ExampleFunc)(Context*);

extern vector<ExampleFunc> examples;

extern vector<string> exampleTitles;

#endif
