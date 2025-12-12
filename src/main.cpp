#include "examples/examples.h"
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

    GUI gui { window, exampleTitles };

    Context ctx;
    ctx.window = window;
    ctx.gui = &gui;
    ctx.selectedExampleIndex = 0;

    ctx.totalExamplesCount = examples.size();

    vector<Example*> examples = {
        new _01_HelloWorld(&ctx),
        new _02_ElementBufferObject(&ctx)
    };

    while (!glfwWindowShouldClose(ctx.window)) {
        if (ctx.selectedExampleIndex < 0 || ctx.selectedExampleIndex >= examples.size()) {
            cout << "Index  " << ctx.selectedExampleIndex << " not existed" << endl;
            break;
        }
        examples.at(ctx.selectedExampleIndex)->run();
    }

    gui.dispose();
    glfwTerminate();

    for (const auto* example : examples) {
        delete &example;
    }

    return 0;
}
