#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "ImageData.h"
#include "ImageIO.h"
#include "TextureImage.h"

#define glError() { \
	GLenum err = glGetError(); \
	while (err != GL_NO_ERROR) { \
		fprintf(stderr, "glError: %s caught at %s:%u\n", (char *)gluErrorString(err), __FILE__, __LINE__); \
		err = glGetError(); \
	} \
}

#if 0
static int IsPowerOfTwo(int n)
{
  while (n > 1)
    {
      if ( n%2 == 1) return 0;
      n /= 2;
    }
  
  return 1;
}
#endif

void InitTexture(TextureImage *tex)
{
  tex->texID = 0;
}

int IsTextureAvailable(TextureImage *tex)
{
  return (tex->texID != 0);
}

int MakeTextureFromImage(TextureImage *tex, ImageData *img)
{
#if 0
  if ( ! (img->width == img->height && IsPowerOfTwo(img->width)) )
    {
      fprintf(stderr, "MakeTextureFromImage: image size must be power-of-two: %dx%d\n",
	      img->width, img->height);
      return 0;
    } 
#endif

  if ( img->channels != 1 && img->channels != 3 )
    {
      fprintf(stderr, "MakeTextureFromImage: invalid channels: %d\n",
	      img->channels);
      return 0;
    }

  glError();

  glGenTextures(1, &tex->texID);

  if (tex->texID == 0)
    {
      fprintf(stderr, "MakeTextureFromImage: texture cannot be created\n");
      return 0;
    }
  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  

  glBindTexture(GL_TEXTURE_2D, tex->texID);
  if (img->channels == 1)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, img->width, img->height, 0,
		     GL_LUMINANCE, GL_UNSIGNED_BYTE, img->data);
  else if (img->channels == 3)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->width, img->height, 0,
		     GL_RGB, GL_UNSIGNED_BYTE, img->data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  glError();

  fprintf(stderr, "MakeTextureFromImage: texture created\n");

  return 1;
}

int MakeTextureFromFile(TextureImage *tex, const char *filename)
{
  ImageData img;

  if ( LoadPPMImage(filename, &img) )
    {
      return MakeTextureFromImage(tex, &img);
    }

  return 0;
}

void DeleteTexture(TextureImage *tex)
{
  if (tex->texID)
    {
      glDeleteTextures(1, &tex->texID);
      tex->texID = 0;
    }
}

void BindTexture(TextureImage *tex)
{
  glBindTexture(GL_TEXTURE_2D, tex->texID);
}

void ReleaseTexture(void)
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawTexturedQuad(TextureImage *tex, int xi, int yi, int w, int h)
{  
  glError();

  glEnable(GL_TEXTURE_2D);
  BindTexture(tex);

  glBegin(GL_QUADS);
  glTexCoord2f(0, 0); glVertex2i(xi,   yi);
  glTexCoord2f(1, 0); glVertex2i(xi+w, yi);
  glTexCoord2f(1, 1); glVertex2i(xi+w, yi+h);
  glTexCoord2f(0, 1); glVertex2i(xi,   yi+h);
  glEnd();

  ReleaseTexture();
  glDisable(GL_TEXTURE_2D);

  glError();
}

