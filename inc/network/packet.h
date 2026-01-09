#ifndef PACKET_H
#define PACKET_H

#include "network/client.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum PacketInfo {
  ID_ASSIGNMENT = 0,
  _PLAYER_POSITION = 10,
  PLAYERS_UPDATE = 11,
} PacketInfo;

typedef struct {
  bool active;
  uint8_t id;
  Vector2 position;
  int flip_h;
} RemotePlayerData;

extern RemotePlayerData remote_players[MAX_PLAYERS];

void id_assignment(uint8_t id);
void handle_player_position(uint8_t id, Vector2 position, bool flip_h);
void send_player_position(Vector2 position, int flip_h);
void remove_player(uint8_t id);

#endif // !PACKET_H
