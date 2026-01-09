#include "graphics/sprite.h"
#include "network/client.h"
#include "network/packet.h"
#include <emscripten/emscripten.h>
#include <raylib.h>
#include <stdbool.h>

static Sprite player;
static const float speed = 200.0f;

static Camera2D camera;

static Vector2 smooth_pos[MAX_PLAYERS];
static bool smooth_init[MAX_PLAYERS] = {0};

static const float lerp_speed = 35.0f;

static void init(void);
static void input(void);
static void render(void);
static void update(void);

void GameLoop(void) {
  input();
  update();
  BeginDrawing();
  render();
  EndDrawing();
}

int main(void) {
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 600, "Bebis Online!");

  init();
  client_create();

  emscripten_set_main_loop(GameLoop, 0, 1);
  return 0;
}

static void draw_grid(int size, int spacing, Color color) {
  for (int x = -size; x <= size; x += spacing)
    DrawLine(x, -size, x, size, color);

  for (int y = -size; y <= size; y += spacing)
    DrawLine(-size, y, size, y, color);
}

static void init(void) {
  player = sprite_new("resource/bebis.png");
  player.position = (Vector2){0, 0};
  player.scale = 1.0f;
  player.rotation = 0.0f;
  player.flip_h = 0;
  player.flip_v = 0;
  player.alpha = 255;

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
  } else if (IsKeyDown(KEY_A)) {
    player.position.x -= speed * dt;
    player.flip_h = 1;
    isMoving = true;
  } else if (IsKeyDown(KEY_S)) {
    player.position.y += speed * dt;
    isMoving = true;
  } else if (IsKeyDown(KEY_W)) {
    player.position.y -= speed * dt;
    isMoving = true;
  }

  float wheel = GetMouseWheelMove();
  if (wheel != 0.0f) {
    camera.zoom += wheel * 0.1f;
    if (camera.zoom < 0.5f)
      camera.zoom = 0.5f;
    if (camera.zoom > 2.0f)
      camera.zoom = 2.0f;
  }

  if (isMoving)
    send_player_position(player.position, player.flip_h);
}

static void update(void) {
  // client_has_connection();
  camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  camera.target = player.origin;
}

static void render(void) {
  float dt = GetFrameTime();

  ClearBackground(BLACK);
  BeginMode2D(camera);

  draw_grid(5000, 64, DARKGRAY);

  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (!g_client->connected)
      return;
    if (!remote_players[i].active)
      continue;

    if (!smooth_init[i]) {
      smooth_pos[i] = remote_players[i].position;
      smooth_init[i] = true;
    }

    smooth_pos[i].x +=
        (remote_players[i].position.x - smooth_pos[i].x) * lerp_speed * dt;
    smooth_pos[i].y +=
        (remote_players[i].position.y - smooth_pos[i].y) * lerp_speed * dt;

    DrawCircleV(smooth_pos[i], 8, RED);

    Sprite temp = player;
    temp.position = smooth_pos[i];
    temp.flip_h = remote_players[i].flip_h;

    sprite_render(&temp);
  }

  sprite_render(&player);
  EndMode2D();
}
