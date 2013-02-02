#include <enet/enet.h>

#include <enet-plus/base/error.hpp>
#include <enet-plus/base/pstdint.hpp>

#include <enet-plus/server_host.hpp>
#include <enet-plus/event.hpp>

namespace bm {

ServerHost* ServerHost::Create(
  uint16_t port,
  size_t peer_count,
  size_t channel_count,
  uint32_t incoming_bandwidth,
  uint32_t outgoing_bandwidth
) {
  ServerHost* server = new ServerHost();
  CHECK(server != NULL);

  ENetAddress address;
  address.host = ENET_HOST_ANY;
  address.port = port;

  server->_server = enet_host_create(&address, peer_count, channel_count,
    incoming_bandwidth, outgoing_bandwidth);
  if(server->_server == NULL) {
    THROW_ERROR("Unable to create enet host!");
    server->_state = STATE_DESTROYED;
    delete server;
    return NULL;
  }

  server->_state = STATE_INITIALIZED;
  return server;
}

ServerHost::~ServerHost() {
  if(_state == STATE_INITIALIZED) {
    Destroy();
  }
};

bool ServerHost::Broadcast(
  const char* data,
  size_t length,
  bool reliable,
  uint8_t channel_id
) {
  CHECK(_state == STATE_INITIALIZED);
  CHECK(data != NULL);
  CHECK(length > 0);

  enet_uint32 flags = 0;
  if(reliable) {
    flags = flags | ENET_PACKET_FLAG_RELIABLE;
  }

  ENetPacket* packet = enet_packet_create(data, length, flags);
  if(packet == NULL) {
    THROW_ERROR("Unable to create enet packet!");
    return false;
  }

  enet_host_broadcast(_server, channel_id, packet);

  return true;
}

bool ServerHost::Service(Event* event, uint32_t timeout) {
  CHECK(_state == STATE_INITIALIZED);

  if(event != NULL) {
    event->_DestroyPacket();
  }

  int rv = enet_host_service(_server, (event == NULL) ? NULL : event->_event, timeout);

  if(rv < 0) {
    THROW_ERROR("Unable to service enet host!");
    return false;
  }
  if(rv > 0) {
    event->_is_packet_destroyed = false;
  }

  return true;
}

void ServerHost::Flush() {
  CHECK(_state == STATE_INITIALIZED);
  enet_host_flush(_server);
}

void ServerHost::Destroy() {
  CHECK(_state == STATE_INITIALIZED);
  enet_host_destroy(_server);
  _state = STATE_DESTROYED;
};

ServerHost::ServerHost() : _state(STATE_CREATED), _server(NULL) { }

} // namespace bm
