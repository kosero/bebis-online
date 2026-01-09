#include "network/client.h"
#include "network/packet.h"
#include <stdio.h>
#include <string.h>

Client *g_client = NULL;

EM_JS(void, js_connect_ws, (const char *url), {
  let wsUrl = UTF8ToString(url);
  console.log("Connecting to:", wsUrl);

  Module.ws = new WebSocket(wsUrl);

  Module.ws.onopen = function() { _on_ws_open_c(); };

  Module.ws.onmessage = function(event) {
    if (typeof event.data === 'string') {
      let len = lengthBytesUTF8(event.data) + 1;
      let c_str = _malloc(len);
      stringToUTF8(event.data, c_str, len);

      _on_ws_message_c(c_str);

      _free(c_str);
    }
  };

  Module.ws.onclose = function() { _on_ws_close_c(); };
})

EM_JS(void, js_send_ws, (const char *msg), {
  if (Module.ws &&Module.ws.readyState === WebSocket.OPEN) {
    Module.ws.send(UTF8ToString(msg));
  } else {
    console.error("WebSocket not open.");
  }
})

EM_JS(void, is_close_ws, (void), {
  if (Module.ws) {
    Module.ws.close();
  }
})

void EMSCRIPTEN_KEEPALIVE on_ws_open_c(void) {
  if (g_client) {
    g_client->connected = 1;
    printf("Connected ws");
  }
}

void EMSCRIPTEN_KEEPALIVE on_ws_message_c(const char *msg) {
  int id;
  int flip;
  float x, y;

  if (strstr(msg, "\"type\": \"id\"")) {
    sscanf(msg, "{ \"type\": \"id\", \"id\": %d }", &id);
    id_assignment(id);
  }

  if (strstr(msg, "\"type\": \"pos\"")) {
    sscanf(
        msg,
        "{ \"type\": \"pos\", \"id\": %d, \"player_position\": [ %f , %f ], \"flip\": %d }",
        &id, &x, &y, &flip);

    handle_player_position(id, (Vector2){x, y}, flip);
  }

  if (strstr(msg, "\"type\": \"leave\"")) {
    sscanf(msg, "{ \"type\": \"leave\", \"id\": %d }", &id);
    remove_player(id);
  }
}

void EMSCRIPTEN_KEEPALIVE on_ws_close_c(void) {
  if (g_client) {
    g_client->connected = 0;
    g_client->interrupted = 1;
    printf("Disconnected ws");
  }
}

void client_create(void) {
  Client *client = calloc(1, sizeof(Client));
  client->client_id = -1;
  g_client = client;

  js_connect_ws("wss://ws.atchannel.top");
}

void client_send_text(const char *text) {
  if (g_client && g_client->connected) {
    js_send_ws(text);
  }
}

void client_destroy(void) {
  if (g_client) {
    is_close_ws();
    free(g_client);
    g_client = NULL;
  }
}
