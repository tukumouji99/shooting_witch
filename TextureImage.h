#ifndef __TEXTURE_IMAGE_H__
#define __TEXTURE_IMAGE_H__

#include "ImageData.h"

typedef struct {
  unsigned int texID;
} TextureImage;

void InitTexture(TextureImage *tex);

int IsTextureAvailable(TextureImage *tex);

int MakeTextureFromImage(TextureImage *tex, ImageData *img);

int MakeTextureFromFile(TextureImage *tex, const char *filename);

void DeleteTexture(TextureImage *tex);

void BindTexture(TextureImage *tex);

void ReleaseTexture(void);

void DrawTexturedQuad(TextureImage *tex, int xi, int yi, int w, int h);

#endif

