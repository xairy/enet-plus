// Copyright (c) 2013 Andrey Konovalov

#include "enet-plus/host.hpp"

#include <map>
#include <string>

#include <enet/enet.h>

#include "enet-plus/base/pstdint.h"

#include "enet-plus/event.h"
#include "enet-plus/peer.h"

namespace enet {

Host::~Host() {
  if (_state == STATE_INITIALIZED) {
    Finalize();
  }
};

bool Host::Initialize(
  const std::string& ip,
  uint16_t port,
  size_t peer_count,
  size_t channel_count,
  uint32_t incoming_bandwidth,
  uint32_t outgoing_bandwidth
) {
  ENetAddress* address_ptr = NULL;

  ENetAddress address;
  if (ip != "" || port != 0) {
    if (ip != "") {
      if (enet_address_set_host(&address, ip.c_str()) != 0) {
        // THROW_ERROR("Unable to set enet host address!");
        return false;
      }
    } else {
      address.host = ENET_HOST_ANY;
    }
    address.port = port;  // XXX: type cast.
    address_ptr = &address;
  }

  _host = enet_host_create(address_ptr, peer_count, channel_count,
    incoming_bandwidth, outgoing_bandwidth);
  if (_host == NULL) {
    // THROW_ERROR("Unable to create enet host!");
    return false;
  }

  _state = STATE_INITIALIZED;
  return true;
}

void Host::Finalize() {
  CHECK(_state == STATE_INITIALIZED);
  enet_host_destroy(_host);
  std::map<_ENetPeer*, Peer*>::iterator itr;
  for (itr = _peers.begin(); itr != _peers.end(); ++itr) {
    delete itr->second;
  }
  _state = STATE_FINALIZED;
};

bool Host::Service(Event* event, uint32_t timeout) {
  CHECK(_state == STATE_INITIALIZED);

  if (event != NULL) {
    event->_DestroyPacket();
  }

  int rv = enet_host_service(_host,
    (event == NULL) ? NULL : event->_event, timeout);

  if (rv < 0) {
    // THROW_ERROR("Unable to service enet host!");
    return false;
  }
  if (rv > 0) {
    event->_is_packet_destroyed = false;
  }

  if (event != NULL) {
    event->_host = this;
  }

  return true;
}

void Host::Flush() {
  CHECK(_state == STATE_INITIALIZED);
  enet_host_flush(_host);
}

Host::Host() : _state(STATE_FINALIZED), _host(NULL) { }

Peer* Host::_GetPeer(_ENetPeer* enet_peer) {
  CHECK(enet_peer != NULL);
  if (_peers.count(enet_peer) == 0) {
    Peer* peer = new Peer(enet_peer);
    CHECK(peer != NULL);
    _peers[enet_peer] = peer;
  }
  return _peers[enet_peer];
}

}  // namespace enet
