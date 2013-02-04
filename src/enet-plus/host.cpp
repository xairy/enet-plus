#include <enet/enet.h>

#include <enet-plus/base/error.hpp>
#include <enet-plus/base/pstdint.hpp>

#include <enet-plus/host.hpp>
#include <enet-plus/event.hpp>

namespace enet {

Host::~Host() {
  if(_state == STATE_INITIALIZED) {
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
  ENetAddress address_ptr = NULL;

  ENetAddress address;
  if(ip != "" || port != 0) {
    if(ip != "") {
      if(enet_address_set_host(&address, ip.c_str()) != 0) {
        THROW_ERROR("Unable to set enet host address!");
        return NULL;
      }
    }
    address.port = port; // XXX: type cast.
    address_ptr = &address;
  }

  _host = enet_host_create(address_ptr, peer_count, channel_count,
    incoming_bandwidth, outgoing_bandwidth);
  if(_host == NULL) {
    THROW_ERROR("Unable to create enet host!");
    return false;
  }

  _state = STATE_INITIALIZED;
  return true;
}

void Host::Finalize() {
  CHECK(_state == STATE_INITIALIZED);
  enet_host_destroy(_host);
  _state = STATE_FINALIZED;
};

bool Host::Service(Event* event, uint32_t timeout) {
  CHECK(_state == STATE_INITIALIZED);

  if(event != NULL) {
    event->_DestroyPacket();
  }

  int rv = enet_host_service(_host,
    (event == NULL) ? NULL : event->_event, timeout);

  if(rv < 0) {
    THROW_ERROR("Unable to service enet host!");
    return false;
  }
  if(rv > 0) {
    event->_is_packet_destroyed = false;
  }

  return true;
}

void Host::Flush() {
  CHECK(_state == STATE_INITIALIZED);
  enet_host_flush(_host);
}

Host::Host() : _state(STATE_FINALIZED), _host(NULL) { }

} // namespace enet
