#include "gui.h"

ImVec4 rgba(unsigned char r, unsigned char g, unsigned char b, float a)
{
    return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a);
}

void styleWindow(ImGuiStyle &style)
{
    style.WindowRounding = 5.0f;
    style.WindowPadding = ImVec2(15, 15);
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(28, 6);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 15.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 5.0f;
    style.GrabRounding = 3.0f;

    style.Colors[ImGuiCol_WindowBg] = rgba(41, 41, 41, .95f);
    //style.Colors[ImGuiCol_ChildWindowBg] = rgba(41, 41, 41, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = rgba(41,41,41, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = rgba(200,200,200, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = rgba(200,200, 200, .8f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = rgba(255, 255, 255, .8f);
    style.Colors[ImGuiCol_FrameBgActive] = rgba(0,0,0, 1.0f);
    style.Colors[ImGuiCol_Header] = rgba(0,0,0,1.0f);
    style.Colors[ImGuiCol_TitleBg] = rgba(255,255,255, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = rgba(0,0,0, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = rgba(41,41,41, .95f);
    style.Colors[ImGuiCol_MenuBarBg] = rgba(0,0,0, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = rgba(0,0,0, 1.0f);
    style.Colors[ImGuiCol_Text] = rgba(255, 255, 255, 1.0f);
    style.Colors[ImGuiCol_Button] = rgba(20, 20, 20, .90f);
    style.Colors[ImGuiCol_ButtonHovered] = rgba(20, 20, 20, .70f);
    style.Colors[ImGuiCol_Border] = rgba(100, 100, 100, .90f);
    style.Colors[ImGuiCol_BorderShadow] = rgba(100, 100, 100, .85f);
    style.Colors[ImGuiCol_Separator] = rgba(255, 255, 255, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = rgba(255, 255, 255, 1.0f);
    style.Colors[ImGuiCol_Column] = rgba(64, 64, 64, 1.0f);

}

GLFWwindow *initglfwWindow()
{
    if (!glfwInit())
        std::cout << "Failed to init" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //olpengl version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    std::cout << glfwGetVersionString() << std::endl;
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    GLFWwindow *window = glfwCreateWindow(540, 900, "imgui", NULL, NULL);

    if (!window)
        std::cout << "Failed to Create window" << std::endl;
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
        std::cout << "Failed to load window" << std::endl;

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("/home/michael/.local/share/fonts/RobotoMono-Regular.ttf",
        28.0f);
    (void)io;

    ImGuiStyle &style = ImGui::GetStyle();
    styleWindow(style);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    return window;
}

GLFWwindow *pollWindow(GLFWwindow *window)
{
    glfwPollEvents();
    glfwSwapBuffers(window);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiSetCond_Always);
    return window;
}

GLFWwindow *renderWindow(GLFWwindow *window)
{
    int display_w, display_h;
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
    return window;
}

GLFWwindow *cleanWindow(GLFWwindow *window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
