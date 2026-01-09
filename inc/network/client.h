#ifndef CLIENT_H
#define CLIENT_H

#include <emscripten/emscripten.h>

typedef struct Client {
  int ws_id;
  int client_id;
  int connected;
  int interrupted;
} Client;

extern Client *g_client;

#define MAX_PLAYERS 64

void client_create(void);
void client_poll(int timeout_ms);
void client_send_text(const char *text);
void client_destrooy(void);

extern void EMSCRIPTEN_KEEPALIVE on_ws_open_c(void);
extern void EMSCRIPTEN_KEEPALIVE on_ws_message_c(const char *msg);
extern void EMSCRIPTEN_KEEPALIVE on_ws_close_c(void);

#endif // !CLIENT_H
