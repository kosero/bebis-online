#include "graphics/sprite.h"
#include "network/client.h"
#include "network/packet.h"
#include <emscripten/emscripten.h>
#include <raylib.h>

Sprite player;
float speed = 200.0f;

static void init(void);
static void input(void);
static void render(void);

void GameLoop(void) {
  input();
  BeginDrawing();
  render();
  EndDrawing();
}

int main(void) {
  InitWindow(1024, 800, "Bebis Online!");
  init();

  client_create();

  emscripten_set_main_loop(GameLoop, 60, 1);

  client_destroy();
  UnloadTexture(player.texture);
  return 0;
}

static void init(void) {
  player = sprite_new("resource/bebis.png");
  player.position = (Vector2){400, 300};
  player.scale = 1.0f;
  player.rotation = 0.0f;
  player.flip_h = 0;
  player.flip_v = 0;
  player.alpha = 255;
}

static void input(void) {
  float dt = GetFrameTime();
  if (IsKeyDown(KEY_D))
    player.position.x += speed * dt;
  if (IsKeyDown(KEY_A))
    player.position.x -= speed * dt;
  if (IsKeyDown(KEY_S))
    player.position.y += speed * dt;
  if (IsKeyDown(KEY_W))
    player.position.y -= speed * dt;
  client_poll(0);
  send_player_position(player.position);
}

static void render(void) {
  ClearBackground(BLACK);
  sprite_render(&player);

  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (remote_players[i].active) {
      Sprite temp;
      temp.texture = player.texture;
      temp.position = remote_players[i].position;
      temp.offset = (Vector2){0.0f, 0.0f};
      temp.origin = player.origin;
      temp.scale = 1.0f;
      temp.rotation = 0.0f;
      temp.flip_h = 0;
      temp.flip_v = 0;
      temp.alpha = 255;
      sprite_render(&temp);
    }
  }
}
