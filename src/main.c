#include "graphics/sprite.h"
#include "network/client.h"
#include "network/packet.h"
#include <emscripten/emscripten.h>
#include <raylib.h>

static Sprite player;
static const float speed = 200.0f;
static Camera2D camera;
static Vector2 smooth_pos[MAX_PLAYERS];
static const float lerp_speed = 35.0f;

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
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  InitWindow(1024, 800, "Bebis Online!");
  init();

  client_create();

  emscripten_set_main_loop(GameLoop, 0, 1);

  client_destroy();
  UnloadTexture(player.texture);
  return 0;
}

static void draw_grid(int size, int spacing, Color color) {
  for (int x = -size; x <= size; x += spacing) {
    DrawLine(x, -size, x, size, color);
  }
  for (int y = -size; y <= size; y += spacing) {
    DrawLine(-size, y, size, y, color);
  }
}

static void init(void) {
  player = sprite_new("resource/bebis.png");
  player.position = (Vector2){400, 300};
  player.scale = 1.0f;
  player.rotation = 0.0f;
  player.flip_h = 0;
  player.flip_v = 0;
  player.alpha = 255;

  int w = GetScreenWidth();
  int h = GetScreenHeight();

  camera.offset = (Vector2){w / 2.0f, h / 2.0f};

  camera.target = player.position;
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
}

static void input(void) {
  float dt = GetFrameTime();
  bool isMoving = false;

  if (IsKeyDown(KEY_D)) {
    player.position.x += speed * dt;
    player.flip_h = 0;
    isMoving = true;
  }
  if (IsKeyDown(KEY_A)) {
    player.position.x -= speed * dt;
    player.flip_h = 1;
    isMoving = true;
  }
  if (IsKeyDown(KEY_S)) {
    player.position.y += speed * dt;
    isMoving = true;
  }
  if (IsKeyDown(KEY_W)) {
    player.position.y -= speed * dt;
    isMoving = true;
  }

  client_poll(0);

  if (isMoving) {
    send_player_position(player.position, player.flip_h);
  }

  camera.target = player.position;
}

static void render(void) {
  float dt = GetFrameTime();

  ClearBackground(BLACK);
  BeginMode2D(camera);

  draw_grid(10000, 32, DARKGRAY);

  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (!remote_players[i].active)
      continue;

    if (smooth_pos[i].x == 0 && smooth_pos[i].y == 0)
      smooth_pos[i] = remote_players[i].position;

    smooth_pos[i].x +=
        (remote_players[i].position.x - smooth_pos[i].x) * lerp_speed * dt;
    smooth_pos[i].y +=
        (remote_players[i].position.y - smooth_pos[i].y) * lerp_speed * dt;

    Sprite temp;
    temp.texture = player.texture;
    temp.position = smooth_pos[i];
    temp.offset = (Vector2){0, 0};
    temp.origin = player.origin;
    temp.scale = 1.0f;
    temp.rotation = 0.0f;
    temp.flip_h = remote_players[i].flip_h;
    temp.flip_v = 0;
    temp.alpha = 255;

    sprite_render(&temp);
  }

  sprite_render(&player);
  EndMode2D();
}
