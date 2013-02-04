#include <enet-plus/enet.hpp>

#include <enet/enet.h>
// XXX: windows sucks
#undef CreateEvent

#include <enet-plus/base/error.hpp>
#include <enet-plus/base/macros.hpp>
#include <enet-plus/base/pstdint.hpp>

#include <enet-plus/host.hpp>
#include <enet-plus/server_host.hpp>
#include <enet-plus/client_host.hpp>
#include <enet-plus/event.hpp>

namespace enet {

Enet::Enet() : _state(STATE_FINALIZED) { }

Enet::~Enet() {
  if(_state == STATE_INITIALIZED) {
    Finalize();
  }
}

bool Enet::Initialize() {
  CHECK(_state == STATE_FINALIZED);
  if(enet_initialize() != 0) {
    THROW_ERROR("Unable to initialize enet!");
    return false;
  }
  _state = STATE_INITIALIZED;
  return true;
}

void Enet::Finalize() {
  CHECK(_state == STATE_INITIALIZED);
  enet_deinitialize();
  _state = STATE_FINALIZED;
}

ServerHost* Enet::CreateServerHost(
  uint16_t port,
  size_t peer_count,
  size_t channel_count,
  uint32_t incoming_bandwith,
  uint32_t outgoing_bandwith
) {
  CHECK(_state == STATE_INITIALIZED);
  ServerHost* server = new ServerHost();
  CHECK(server != NULL);
  bool rv = server->Initialize(port, peer_count,
    channel_count, incoming_bandwith, outgoing_bandwith);
  return rv ? server : NULL;
}

ClientHost* Enet::CreateClientHost(
  size_t channel_count,
  uint32_t incoming_bandwith,
  uint32_t outgoing_bandwith
) {
  CHECK(_state == STATE_INITIALIZED);
  ClientHost* client = new ClientHost();
  CHECK(client != NULL);
  bool rv = client->Initialize(channel_count,
    incoming_bandwith, outgoing_bandwith);
  return rv ? client : NULL;
}

Event* Enet::CreateEvent() {
  Event* event = new Event;
  CHECK(event != NULL);
  return event;
}

} // namespace enet
