// Copyright (c) 2013 Andrey Konovalov

#include "enet-plus/peer.h"

#include <string>

#include <enet/enet.h>

#include "enet-plus/base/macros.h"
#include "enet-plus/base/pstdint.h"

namespace enet {

bool Peer::Send(
  const char* data,
  size_t length,
  bool reliable,
  uint8_t channel_id
) {
  enet_uint32 flags = 0;
  if (reliable) {
    flags = flags | ENET_PACKET_FLAG_RELIABLE;
  }
  ENetPacket* packet = enet_packet_create(data, length, flags);
  if (packet == NULL) {
    // THROW_ERROR("Unable to create enet packet!");
    return false;
  }
  if (enet_peer_send(_peer, channel_id, packet) != 0) {
    enet_packet_destroy(packet);
    // THROW_ERROR("Unable to send enet packet!");
    return false;
  }
  return true;
}

std::string Peer::GetIp() const {
  const size_t BUFFER_SIZE = 32;
  char buffer[BUFFER_SIZE];
  if (enet_address_get_host_ip(&_peer->address, buffer, BUFFER_SIZE) != 0) {
    // THROW_ERROR("Unable to get enet host ip!");
    buffer[0] = 0;
  }
  return std::string(buffer);
}

uint16_t Peer::GetPort() const {
  return _peer->address.port;  // XXX: type cast.
}

void Peer::Disconnect() {
  enet_peer_disconnect(_peer, 0);
}

void Peer::DisconnectNow() {
  enet_peer_disconnect_now(_peer, 0);
}

void Peer::DisconnectLater() {
  enet_peer_disconnect_later(_peer, 0);
}

void Peer::Reset() {
  enet_peer_reset(_peer);
}

void Peer::SetData(void* data) {
  _peer->data = data;
}

void* Peer::GetData() const {
  return _peer->data;
}

Peer::Peer(ENetPeer* peer) : _peer(peer) {
  CHECK(peer != NULL);
}

}  // namespace enet
