#include "pch.h"
#include "Window.hpp"
#include "Logging.hpp"
#include "Widgets.hpp"

GLFWwindow* StartApp()
{
    /* Initialize logging */
    SP_INIT_LOG();
    SP_LOG_TRACE("Starting StegPass v.{}.{}.{}",
        SP_VERSION_MAJOR, SP_VERSION_MINOR, SP_VERSION_PATCH);

    GLFWwindow* window = nullptr;

    /* Initialize the library */
    if (!glfwInit())
        return nullptr;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(
        WindowSpecs::WIDTH, WindowSpecs::HEIGHT,
        WindowSpecs::TITLE, NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    SP_LOG_TRACE("Window created successfully.");

    // Load all OpenGL functions using the glfw loader function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::exception("Failed to initialize glad extension");
        return nullptr;
    }
    SP_LOG_INFO("OpenGL Version: {}", std::string((char*)glGetString(GL_VERSION)));
    
    // Initialize ImGui
    InitGui(window);
    SP_LOG_TRACE("ImGui initialized successfully.");
    
    return window;
}

void CloseApp(GLFWwindow* window)
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    SP_LOG_TRACE("ImGui shutdown successfully.");

    glfwDestroyWindow(window);
    glfwTerminate();
    SP_LOG_TRACE("Window destroyed successfully.");

    SP_DESTROY_LOG();
}

void InitGui(GLFWwindow* window)
{
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Initialize gui resources
    sp::FontManager::Init();

    // Load and add fonts
    (void)sp::FontManager::AddFont("OpenSans", "res/Open_Sans/OpenSans-VariableFont_wdth,wght.ttf", 20.0f);
}

void RunAppLoop(GLFWwindow* window)
{
    ImFont* openSans = sp::FontManager::GetFont("OpenSans", 20.0f);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::PushFont(openSans);
        sp::DrawMenuBar();

        // Create ImGui window
        ImGui::Begin("Hello, ImGui!");
        ImGui::Text("This is a simple example.");
        ImGui::End();

        ImGui::PopFont();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}
