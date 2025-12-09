#ifndef _02_ELEMENT_BUFFER_OBJECT_H
#define _02_ELEMENT_BUFFER_OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../initialize.h"
#include "../helpers/read-shader-file.h"
#include "../helpers/shader-program.h"
#include "../helpers/close-window-on-esc-pressed.h"
#include "../helpers/toggle-polygon-mode-on-key-pressed.h"
#include "../helpers/gui.h"

#include "type.h"

ReturnType _02_elementBufferObject(Context ctx);

#endif // _02_ELEMENT_BUFFER_OBJECT_H
