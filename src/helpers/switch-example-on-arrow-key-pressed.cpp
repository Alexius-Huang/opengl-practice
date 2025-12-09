#include "./switch-example-on-arrow-key-pressed.h"

// TODO: Program currently crashed if we pressed at least two keys at the same time
//       Investigate to prevent this behaviour
// TODO: Current implementation didn't handle the case of preventing further event
//       emission
bool switchExampleOnArrowKeyPressed(Context* ctx) {
    /* Navigate to Previous Example */
    if (
        ctx->selectedExampleIndex != 0 && (
            glfwGetKey(ctx->window, GLFW_KEY_UP) == GLFW_PRESS ||
            glfwGetKey(ctx->window, GLFW_KEY_LEFT) == GLFW_PRESS
        )
    ) {
        ctx->selectedExampleIndex -= 1;
        return true;
    }    

    /* Navigate to Next Example */
    if (
        ctx->selectedExampleIndex != ctx->totalExamplesCount - 1 && (
            glfwGetKey(ctx->window, GLFW_KEY_DOWN) == GLFW_PRESS ||
            glfwGetKey(ctx->window, GLFW_KEY_RIGHT) == GLFW_PRESS
        )
    ) {
        ctx->selectedExampleIndex += 1;
        return true;
    }

    return false;
}
