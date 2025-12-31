#include "examples/examples.h"
using namespace std;

int main() {
    OpenGLVersion version;
    version.major = 3;
    version.minor = 3;

    // TODO: sync window size into Context
    OpenGLWindowSize windowSize;
    windowSize.width = 1200;
    windowSize.height = 960;

    GLFWwindow* window = initialize(version, windowSize);

    GUI gui{window};

    Context ctx;
    ctx.window = window;
    ctx.gui = &gui;
    ctx.selectedExampleIndex = 0;

    vector<Example*> examples = {
        new _01_HelloWorld(&ctx),
        new _02_ElementBufferObject(&ctx),
        new _03_DrawTwoTriangles(&ctx),
        new _04_UsingMultipleVAOAndVBO(&ctx),
        new _05_UsingMultipleShaders(&ctx),
        new _06_PassDataBetweenShaders(&ctx),
        new _07_PassDataUsingVertexAttributes(&ctx),
        new _08_PassDataUsingUniforms(&ctx),
        new _09_UsingTexture(&ctx),
        new _10_UsingMultipleTextures(&ctx),
        new _11_Transformation(&ctx),
        new _12_CoordinateSystem(&ctx),
        new _13_3DCube(&ctx),
        new _14_Multiple3DCubes(&ctx),
        new _15_CameraLookAtMatrix(&ctx),
        new _16_CameraControl(&ctx),
        new _17_LightScene(&ctx),
        new _18_MovingLightSource(&ctx),
        new _19_Material(&ctx),
        new _20_DifferentMaterials(&ctx),
        new _21_LightMaps(&ctx),
        new _22_DirectionalLight(&ctx),
        new _23_PointLight(&ctx),
        new _24_SpotLight(&ctx),
        new _25_LoadingModelUsingAssimp(&ctx),
        new _26_DepthTest(&ctx),
        new _27_StencilTest(&ctx),
        new _28_DiscardingFragment(&ctx),
        new _29_Blending(&ctx),
        new _30_FaceCulling(&ctx),
        new _31_FrameBuffer(&ctx),
        new _32_Postprocessing(&ctx),
        new _33_KernelEffects(&ctx),
        new _34_CreateSkyboxUsingCubemap(&ctx)
    };
    ctx.totalExamplesCount = examples.size();

    vector<string> exampleTitles;
    int i = 1;
    for (const auto* example : examples) {
        exampleTitles.push_back(to_string(i++) + ". " + example->getTitle());
    }
    gui.setExampleTitles(exampleTitles);

    while (!glfwWindowShouldClose(ctx.window)) {
        if (ctx.selectedExampleIndex < 0 || ctx.selectedExampleIndex >= examples.size()) {
            cout << "Index  " << ctx.selectedExampleIndex << " not existed" << endl;
            break;
        }

        MouseMoveEvent::reset();
        examples.at(ctx.selectedExampleIndex)->run();
    }
    gui.dispose();
    glfwTerminate();

    // TODO: investigate why there is pointer being freed was not allocated
    // int i = 0;
    // for (const auto* example : examples) {
    //     cout << "Deleting example " << i++ << ": " << example->getTitle() << endl;
    //     delete &example;
    // }
    return 0;
}
