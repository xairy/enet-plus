#include <enet-plus/event.hpp>

#include <string>
#include <vector>

#include <enet/enet.h>

#include <enet-plus/base/pstdint.hpp>

#include <enet-plus/host.hpp>
#include <enet-plus/peer.hpp>

namespace enet {

Event::~Event() {
  _DestroyPacket();
  delete _event;
}

Event::EventType Event::GetType() const {
  if(_event->type == ENET_EVENT_TYPE_CONNECT) {
    return Event::TYPE_CONNECT;
  }
  if(_event->type == ENET_EVENT_TYPE_DISCONNECT) {
    return Event::TYPE_DISCONNECT;
  }
  if(_event->type == ENET_EVENT_TYPE_RECEIVE) {
    return Event::TYPE_RECEIVE;
  }
  return Event::TYPE_NONE;
}

uint8_t Event::GetChannelId() const {
  CHECK(_event->type != ENET_EVENT_TYPE_NONE);
  return _event->channelID;
}

void Event::GetData(std::vector<char>* output) const {
  CHECK(_event->type == ENET_EVENT_TYPE_RECEIVE);
  CHECK(_is_packet_destroyed == false);
  output->assign(_event->packet->data, _event->packet->data +
    _event->packet->dataLength);
}

Peer* Event::GetPeer() {
  CHECK(_event->type != ENET_EVENT_TYPE_NONE);
  CHECK(_host != NULL);
  Peer* peer = _host->_GetPeer(_event->peer);
  CHECK(peer != NULL);
  return peer;
}

Event::Event() : _is_packet_destroyed(true) {
  _event = new ENetEvent();
  CHECK(_event != NULL);
}

void Event::_DestroyPacket() {
  if(!_is_packet_destroyed && _event->type == ENET_EVENT_TYPE_RECEIVE) {
    enet_packet_destroy(_event->packet);
    _is_packet_destroyed = true;
  }
}

} // namespace enet
