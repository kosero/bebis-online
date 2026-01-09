#include "network/packet.h"
#include "network/client.h"
#include "raylib.h"
#include <stdio.h>

RemotePlayerData remote_players[MAX_PLAYERS] = {0};

void id_assignment(int id) {
  if (g_client->client_id != -1)
    return;

  g_client->client_id = id;
  TraceLog(LOG_INFO, "Assigned client id: %d\n", id);
}

static RemotePlayerData *get_or_create_player(int id) {
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

void handle_player_position(int id, Vector2 position) {
  if (id == g_client->client_id)
    return;

  RemotePlayerData *p = get_or_create_player(id);
  if (p)
    p->position = position;
}

void send_player_position(Vector2 position) {
  if (!g_client || !g_client->connected || g_client->client_id == -1)
    return;

  char msg[128];
  snprintf(msg, sizeof(msg),
           "{ \"type\": \"pos\", \"id\": %d, \"player_position\": [ %f, %f ] }",
           g_client->client_id, position.x, position.y);

  client_send_text(msg);
}

void remove_player(int id) {
  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (remote_players[i].active && remote_players[i].id == id) {
      remote_players[i].active = 0;
      printf("Remove remote player %d\n", id);
      return;
    }
  }
}
