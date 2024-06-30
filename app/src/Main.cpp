#include "pch.h"
#include "Window.hpp"

int main(void)
{
    /* Build up */
    if (!StartApp()) {
		return -1;
    }

    /* Loop until the user closes the window */
    RunAppLoop();

    /* Tear down */
    CloseApp();

    return 0;
}

#ifdef SP_RELEASE
#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return main();
}

#endif