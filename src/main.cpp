#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "initialize.h"
#include "helpers/read-shader-file.h"
#include "helpers/shader-program.h"
#include "helpers/close-window-on-esc-pressed.h"
#include "examples/01-hello-world.h"
#include "examples/02-element-buffer-object.h"

int main() {
    while (true) {
        std::cout << "There are currently following examples:" << std::endl << std::endl;
        std::cout << "1. Hello World: shows the basics of shader program, VBO, VAO to draw simple triangle" << std::endl << std::endl;
        std::cout << "2. Element Buffer Object: reuse vertices to draw shaps" << std::endl << std::endl;

        int option;
        std::cout << "Choose the example (0 to exit): ";
        std::cin >> option;
        
        if (std::cin.fail()) {
            std::cout << "Expect input to be integer" << std::endl;
            return -1;
        }

        if (option == 0) break;
        else if (option == 1) _01_helloWorld();
        else if (option == 2) _02_elementBufferObject();
        else {
            std::cout << "Option " << option << " not existed" << std::endl;
            return -1;
        }

        std::cout << "Example is detected closed..." << std::endl << std::endl;
    }

    return 0;
}
