g++ -std=c++11 -DPLATFORM=Desktop -DSHARED=On -D IMGUI_IMPL_OPENGL_LOADER_GLAD req.cpp main.cpp gui.cpp imgui_widgets.cpp imgui.cpp imgui_impl_glfw.cpp imgui_draw.cpp imgui_impl_opengl3.cpp  glad/src/glad.c -I glad/include -l GL -l curl -l dl -l glfw3 -l X11 -l pthread -o run;