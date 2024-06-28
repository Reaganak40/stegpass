#include "pch.h"
#include "Window.hpp"

int main(void)
{
    /* Build up */
    GLFWwindow* window = StartApp();
    if (!window) {
		return -1;
    }

    /* Loop until the user closes the window */
    RunAppLoop(window);

    /* Tear down */
    CloseApp(window);

    return 0;
}