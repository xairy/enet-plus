#include <string>

#include <enet/enet.h>

#include <enet-plus/base/error.hpp>
#include <enet-plus/base/pstdint.hpp>

#include <enet-plus/event.hpp>
#include <enet-plus/peer.hpp>
#include <enet-plus/client_host.hpp>

namespace enet {

ClientHost* ClientHost::Create (
  size_t channel_count,
  uint32_t incoming_bandwidth,
  uint32_t outgoing_bandwidth
) {
  ClientHost* client = new ClientHost();
  CHECK(client != NULL);

  client->_client = enet_host_create(NULL, 1, channel_count, incoming_bandwidth, outgoing_bandwidth);
  if(client->_client == NULL) {
    THROW_ERROR("Unable to create enet host!");
    client->_state = STATE_DESTROYED;
    delete client;
    return NULL;
  }

  client->_state = STATE_INITIALIZED;
  return client;
}

ClientHost::~ClientHost() {
  if(_state == STATE_INITIALIZED) {
    Destroy();
  }
}

bool ClientHost::Service(Event* event, uint32_t timeout) {
  CHECK(_state == STATE_INITIALIZED);

  if(event != NULL) {
    event->_DestroyPacket();
  }

  int rv = enet_host_service(_client, (event == NULL) ? NULL : event->_event, timeout);

  if(rv < 0) {
    THROW_ERROR("Unable to service enet host!");
    return false;
  }
  if(rv > 0) {
    event->_is_packet_destroyed = false;
  }

  return true;
}

Peer* ClientHost::Connect(
  std::string server_ip,
  uint16_t port,
  size_t channel_count
)	{
  CHECK(_state == STATE_INITIALIZED);

  ENetAddress address;
  if(enet_address_set_host(&address, server_ip.c_str()) != 0) {
    THROW_ERROR("Unable to set enet host address!");
    return NULL;
  }
  address.port = port;

  ENetPeer* enet_peer = enet_host_connect(_client, &address, channel_count, 0);
  if(enet_peer == NULL) {
    THROW_ERROR("Enet host is unable to connect!");
    return NULL;
  }

  Peer* peer = new Peer(enet_peer);
  if(peer == NULL) {
    return NULL;
  }

  return peer;
}

void ClientHost::Flush() {
  CHECK(_state == STATE_INITIALIZED);
  enet_host_flush(_client);
}

void ClientHost::Destroy() {
  CHECK(_state == STATE_INITIALIZED);
  enet_host_destroy(_client);
  _state = STATE_DESTROYED;
};

ClientHost::ClientHost() : _state(STATE_CREATED), _client(NULL) { };

} // namespace enet
