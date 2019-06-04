//#include <stdio.h>
#include <iostream>
#include "req.h"
#include "gui.h"
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "json.hpp"
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imstb_rectpack.h"
#include "imstb_truetype.h"

#define ARRAY_SIZE(x)(sizeof(x)/sizeof(x)[0])
using json = nlohmann::json;

unsigned int loadTexture(const char *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 4);
    if (data)
    {
        GLenum format;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


struct user {
  std::string uid;
  std::string name;
  std::string bio;
  int distance_mi;
  std::vector<std::string> photos;
  std::vector<GLuint> textures;
};
typedef struct user Struct;

Struct next_user(int pos, const json &resp) {
  Struct s;
  s.uid = resp[pos]["_id"];
  s.name = resp[pos]["name"];
  s.bio = resp[pos]["bio"];
  s.distance_mi = resp[pos]["distance_mi"];

  for (int i=0; i<resp[pos]["photos"].size(); i++) {
    std::cout << resp[pos]["photos"][i]["url"] <<std::endl;
    s.photos.push_back(resp[pos]["photos"][i]["url"]);
    s.textures.push_back(loadTexture(get_img(resp[pos]["photos"][i]["url"]).c_str()));
  }
  return s;
}

int main()
{

  GLFWwindow *window = NULL;
  window = initglfwWindow();

  Struct s;
  int pos = 0;
  json resp = req()["results"];
  s = next_user(pos, resp);

  bool menu_bar = true;
  while (!glfwWindowShouldClose(window))
  {
    pollWindow(window);

    ImGui::Begin("Tinder Profile", NULL);
    ImGui::SameLine(ImGui::GetWindowWidth()*.05);
    if (pos >= 1) {
      if (ImGui::Button("PREV")) {
        glDeleteTextures(s.textures.size(), s.textures.data());
        std::cout << pos << std::endl;
        s = next_user(--pos, resp);
      }
    }
    ImGui::SameLine(ImGui::GetWindowWidth()*.85f);
    if (ImGui::Button("NEXT")) {
      glDeleteTextures(s.textures.size(), s.textures.data());
      std::cout << pos << std::endl;
      s = next_user(++pos, resp);
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::TextColored(rgba(255,255,255,1), "%s\n", s.name.c_str());
    ImGui::Indent();
    ImGui::Text("%d miles\n", s.distance_mi);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextWrapped("%s\n", s.bio.c_str());
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Unindent();
    ImGui::BeginChild("Image Box");
    for (auto texture : s.textures) {
      ImGui::Image((void*)(intptr_t)texture, ImVec2(540, 675));
    }
    ImGui::EndChild();
    ImGui::End();

    ImGui::Render();

    renderWindow(window);
  }
  cleanWindow(window);
  return 0;
}
