#include "examples/01-hello-world.h"
#include "examples/02-element-buffer-object.h"
using namespace std;

int main() {
    OpenGLVersion version;
    version.major = 3;
    version.minor = 3;

    OpenGLWindowSize windowSize;
    windowSize.width = 1200;
    windowSize.height = 960;

    GLFWwindow* window = initialize(
        version,
        windowSize
    );

    GUI gui;
    gui.init(window);

    Context ctx;
    ctx.window = window;
    ctx.gui = &gui;

    ReturnType result;
    result.selectedIndex = 0;

    while (!glfwWindowShouldClose(ctx.window)) {
        if (result.selectedIndex == 0) {
            result = _01_helloWorld(ctx);
        } else if (result.selectedIndex == 1) {
            result = _02_elementBufferObject(ctx);
        } else {    
            cout << "Index  " << result.selectedIndex << " not existed" << endl;
            break;
        }
    }

    gui.dispose();
    glfwTerminate();

    return 0;
}
