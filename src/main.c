#include "network/client.h"
#include "network/packet.h"
#include <emscripten/emscripten.h>
#include <raylib.h>

Texture2D tex;
Vector2 pos = {400, 300};
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
  return 0;
}

static void init(void) { tex = LoadTexture("resource/bebis.png"); }

static void input(void) {
  float dt = GetFrameTime();

  if (IsKeyDown(KEY_D))
    pos.x += speed * dt;
  if (IsKeyDown(KEY_A))
    pos.x -= speed * dt;
  if (IsKeyDown(KEY_S))
    pos.y += speed * dt;
  if (IsKeyDown(KEY_W))
    pos.y -= speed * dt;

  client_poll(0);
  send_player_position(pos);
}

static void render(void) {
  ClearBackground(BLACK);
  DrawTexture(tex, (int)pos.x, (int)pos.y, WHITE);

  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (remote_players[i].active) {
      DrawTexture(tex, (int)remote_players[i].position.x,
                  (int)remote_players[i].position.y, WHITE);
    }
  }
}
