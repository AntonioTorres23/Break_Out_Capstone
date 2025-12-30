// C++ file that prevents some issues that can happen when stb_image isn't defined within a preprocessor and is used
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"