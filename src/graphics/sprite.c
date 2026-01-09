#include "graphics/sprite.h"
#include "raylib.h"

Sprite sprite_new(const char *tex_path) {
  Sprite s;
  s.texture = LoadTexture(tex_path);
  s.position = (Vector2){0.0f, 0.0f};
  s.offset = (Vector2){0.0f, 0.0f};
  s.origin = (Vector2){s.texture.width / 2.0f, s.texture.height / 2.0f};
  s.scale = 1.0f;
  s.rotation = 0.0f;
  s.flip_h = 0;
  s.flip_v = 0;
  s.alpha = 255;
  return s;
}

void sprite_render(Sprite *s) {
  s->origin.x = s->position.x + s->offset.x;
  s->origin.y = s->position.y + s->offset.y;

  Rectangle source = {0.0f, 0.0f,
                      s->flip_h ? -s->texture.width : s->texture.width,
                      s->flip_v ? -s->texture.height : s->texture.height};

  Rectangle dest = {s->position.x + s->offset.x, s->position.y + s->offset.y,
                    s->texture.width * s->scale, s->texture.height * s->scale};

  Vector2 origin = {s->texture.width / 2.0f, s->texture.height / 2.0f};

  Color tint = (Color){255, 255, 255, s->alpha};
  DrawTexturePro(s->texture, source, dest, origin, s->rotation, tint);
}
