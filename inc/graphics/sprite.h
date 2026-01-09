#ifndef SPRITE_H
#define SPRITE_H

#include <raylib.h>
#include <stdint.h>

typedef struct Sprite {
  Texture2D texture;
  Vector2 position;
  Vector2 origin;
  Vector2 offset;
  float scale;
  float rotation;
  int flip_h;
  int flip_v;
  uint8_t alpha;
} Sprite;

Sprite sprite_new(const char *tex_path);
void sprite_render(Sprite *s);

#endif
