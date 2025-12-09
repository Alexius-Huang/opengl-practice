#include "examples/01-hello-world.h"
#include "examples/02-element-buffer-object.h"
#include "examples/03-draw-two-triangles.h"
#include "examples/04-using-multiple-vao-and-vbo.h"
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
    ctx.selectedExampleIndex = 0;

    ctx.totalExamplesCount = 4;

    while (!glfwWindowShouldClose(ctx.window)) {
        if (ctx.selectedExampleIndex == 0) {
            _01_helloWorld(&ctx);
        } else if (ctx.selectedExampleIndex == 1) {
            _02_elementBufferObject(&ctx);
        } else if (ctx.selectedExampleIndex == 2) {
            _03_drawTwoTriangles(&ctx);
        } else if (ctx.selectedExampleIndex == 3) {
            _04_usingMultipleVAOAndVBO(&ctx);
        } else {
            cout << "Index  " << ctx.selectedExampleIndex << " not existed" << endl;
            break;
        }
    }

    gui.dispose();
    glfwTerminate();

    return 0;
}
