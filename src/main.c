#include <emscripten/emscripten.h>
#include <raylib.h>

void GameLoop(void) {
  float dt = GetFrameTime();
  BeginDrawing();
  ClearBackground(BLACK);

  EndDrawing();
}

int main(void) {
  InitWindow(GetScreenWidth(), GetScreenHeight(), "Bebis Online!");

  emscripten_set_main_loop(GameLoop, 60, 1);

  return 0;
}
