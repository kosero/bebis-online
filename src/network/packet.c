#include "network/packet.h"
#include "network/client.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>

RemotePlayerData remote_players[MAX_PLAYERS] = {0};

void id_assignment(uint8_t id) {
  printf("ID PACKET RECEIVED: %d\n", id);

  if (g_client->client_id != -1)
    return;

  g_client->client_id = id;
  printf("CLIENT ID SET: %d\n", id);
}

static RemotePlayerData *get_or_create_player(uint8_t id) {
  TraceLog(LOG_INFO, "get_or_create_player id=%d", id);

  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (remote_players[i].active && remote_players[i].id == id)
      return &remote_players[i];
  }

  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (!remote_players[i].active) {
      remote_players[i].active = 1;
      remote_players[i].id = id;
      remote_players[i].position = (Vector2){0};
      TraceLog(LOG_INFO, "spawn remote player %d\n", id);
      return &remote_players[i];
    }
  }

  return NULL;
}

void handle_player_position(uint8_t id, Vector2 position, bool flip_h) {
  printf("POS PACKET id=%d pos=(%f,%f)\n", id, position.x, position.y);

  if (id == g_client->client_id) {
    printf("IGNORED (self)\n");
    return;
  }

  RemotePlayerData *p = get_or_create_player(id);
  if (!p) {
    printf("NO SLOT FOR PLAYER %d\n", id);
    return;
  }

  p->position = position;
  p->flip_h = flip_h;

  printf("UPDATED REMOTE %d\n", id);
}

void send_player_position(Vector2 position, int flip_h) {
  if (!g_client || !g_client->connected || g_client->client_id == -1)
    return;

  char msg[160];
  snprintf(msg, sizeof(msg),
           "{ \"type\": \"pos\", \"id\": %d, \"player_position\": [ %f, %f ], "
           "\"flip\": %d }",
           g_client->client_id, position.x, position.y, flip_h);

  client_send_text(msg);
}

void remove_player(uint8_t id) {
  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (remote_players[i].active && remote_players[i].id == id) {
      remote_players[i].active = 0;
      printf("Remove remote player %d\n", id);
      return;
    }
  }
}
