#ifndef PACKET_H
#define PACKET_H

#include "network/client.h"
#include "raylib.h"

typedef enum PacketInfo {
  ID_ASSIGNMENT = 0,
  _PLAYER_POSITION = 10,
  PLAYERS_UPDATE = 11,
} PacketInfo;

typedef struct {
  int active;
  int id;
  Vector2 position;
} RemotePlayerData;

extern RemotePlayerData remote_players[MAX_PLAYERS];

void id_assignment(int id);
void handle_player_position(int id, Vector2 position);
void send_player_position(Vector2 position);
void remove_player(int id);

#endif // !PACKET_H
