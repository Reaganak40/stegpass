#include "pch.h"
#include "Window.hpp"
#include "Logging.hpp"
#include "Widgets.hpp"
#include "Pages.hpp"

/// <summary>
/// Holds global window information.
/// </summary>
struct Window
{
    GLFWwindow* context = nullptr;
    std::string title = "";
};
static Window app_window;

void InitGui();

bool StartApp()
{
    /* Initialize logging */
    SP_INIT_LOG();
    SP_LOG_TRACE("Starting StegPass v.{}.{}.{}",
        SP_VERSION_MAJOR, SP_VERSION_MINOR, SP_VERSION_PATCH);

    /* Initialize the library */
    if (!glfwInit())
    {
        SP_LOG_CRITICAL("Failed to initialize GLFW.");
        return false;
    }

    /* Create a windowed mode window and its OpenGL context */
    app_window.context = glfwCreateWindow(
        WindowSpecs::WIDTH, WindowSpecs::HEIGHT,
        WindowSpecs::TITLE, NULL, NULL);
    app_window.title = WindowSpecs::TITLE;

    if (!app_window.context)
    {
        glfwTerminate();
        SP_LOG_CRITICAL("Failed to create window.");
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(app_window.context);
    SP_LOG_TRACE("Window created successfully.");

    // Load all OpenGL functions using the glfw loader function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SP_LOG_CRITICAL("Failed to initialize GLAD.");
        return false;
    }
    SP_LOG_INFO("OpenGL Version: {}", std::string((char*)glGetString(GL_VERSION)));

    // Initialize ImGui
    InitGui();
    SP_LOG_TRACE("ImGui initialized successfully.");

    return true;
}

void CloseApp()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    SP_LOG_TRACE("ImGui shutdown successfully.");

    glfwDestroyWindow(app_window.context);
    glfwTerminate();
    SP_LOG_TRACE("Window destroyed successfully.");

    SP_DESTROY_LOG();
}

void InitGui()
{
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(app_window.context, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Initialize gui resources
    sp::FontManager::Init();

    // Load and add fonts
    (void)sp::FontManager::AddFont("OpenSans", "res/Open_Sans/OpenSans-VariableFont_wdth,wght.ttf", SP_DEFAULT_FONT_SIZE);
    (void)sp::FontManager::AddPasswordFont("PasswordDots", "res/Open_Sans/OpenSans-VariableFont_wdth,wght.ttf", SP_DEFAULT_FONT_SIZE);
    (void)sp::FontManager::AddIconFont("FontAwesome", "res/awesome/forkawesome-webfont.ttf", SP_DEFAULT_FONT_SIZE);
    
}

void RunAppLoop()
{
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(app_window.context))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Draw the current page
        sp::DrawAddPasswordPage();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(app_window.context);

        /* Poll for and process events */
        glfwPollEvents();
    }
}

void SetWindowTitle(const char* title)
{
    if (app_window.title != title)
	{
		glfwSetWindowTitle(app_window.context, title);
		app_window.title = title;
	}
}
