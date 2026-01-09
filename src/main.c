#include "network/client.h"
#include <emscripten/emscripten.h>
#include <raylib.h>
#include <stdio.h>

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
  InitWindow(GetScreenWidth(), GetScreenHeight(), "Bebis Online!");
  init();

  client_create();

  emscripten_set_main_loop(GameLoop, 60, 1);

  return 0;
}

static void init(void) { printf("a"); }

static void input(void) {
  if (IsKeyDown(KEY_A)) {
    printf("a");
  }
}

static void render(void) { ClearBackground(BLACK); }
