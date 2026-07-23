#ifndef LOADER_H
#define LOADER_H

#include "Helper_Dieldie.h"
#define IMGUI_DEFINE
#include "ImGui/imgui.h"

// Add OpenGL headers
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>

#include "ImGui/stb_image.h"

extern bool isImageLoaded;
extern int width;
extern int height;
extern GLuint mIcon;
extern GLuint mLogoMenu;
extern GLuint mLogoNew;


bool LoadTextureFromMemory(const stbi_uc *data, size_t size, GLuint *out_texture, int *out_width, int *out_height);
void LoadImage();

#endif // LOADER_H