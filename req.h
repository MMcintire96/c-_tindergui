#pragma once
#include "json.hpp"
using json = nlohmann::json;
json req();
typedef struct
{
  unsigned char *data;
  size_t size;
} buffer_t;

std::string get_img(std::string url);

