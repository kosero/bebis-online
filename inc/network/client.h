#ifndef CLIENT_H
#define CLIENT_H

#include <stdbool.h>
#include <emscripten/emscripten.h>
#include <stdint.h>

typedef struct Client {
  int ws_id;
  uint8_t client_id;
  bool connected;
  bool interrupted;
} Client;

extern Client *g_client;

#define MAX_PLAYERS 64

void client_create(void);
void client_poll(int timeout_ms);
void client_send_text(const char *text);
void client_destroy(void);

extern void EMSCRIPTEN_KEEPALIVE on_ws_open_c(void);
extern void EMSCRIPTEN_KEEPALIVE on_ws_message_c(const char *msg);
extern void EMSCRIPTEN_KEEPALIVE on_ws_close_c(void);

#endif // !CLIENT_H
