// Copyright (c) 2013 Andrey Konovalov

#ifndef ENET_PLUS_EVENT_HPP_
#define ENET_PLUS_EVENT_HPP_

#include <string>
#include <vector>

#include "enet-plus/base/macros.h"
#include "enet-plus/base/pstdint.h"

#include "enet-plus/dll.h"

struct _ENetEvent;

namespace enet {

class Enet;
class Host;
class Peer;

// 'Event' class represents an event that can be delivered by
// 'ClientHost::Service()' and 'ServerHost::Service()' methods.
// You can create an empty 'Event' by using 'Enet::CreateEvent()'.
class Event {
  friend class Host;
  friend class Enet;

 public:
  enum EventType {
    // No event occurred within the specified time limit.
    TYPE_NONE,

    // A connection request initiated by 'ClientHost::Connect()' has completed.
    // You can use 'GetPeer()' to get information about the connected peer.
    TYPE_CONNECT,

    // A peer has disconnected. This event is generated on a successful
    // completion of a disconnect initiated by 'Peer::Disconnect()'.
    // You can use 'GetPeer()' to get information about the disconnected peer.
    TYPE_DISCONNECT,

    // A packet has been received from a peer. You can use 'GetPeer()' to get
    // information about peer which sent the packet, 'GetChannelId()' to get
    // the channel number upon which the packet was received, 'GetData()' to
    // get the data from the received packet.
    TYPE_RECEIVE
  };

  ENET_PLUS_DECL ~Event();

  // Returns the type of the event.
  ENET_PLUS_DECL EventType GetType() const;

  // Returns the channel on which the received packet was sent.
  // Event type should not be 'TYPE_NONE' to use this method.
  ENET_PLUS_DECL uint8_t GetChannelId() const;

  // Returns the data associated with the event. Data will be lost if
  // another event is delivered using 'ClientHost::Service()' or
  // 'ServerHost::Service()' using this instance of 'Event' class.
  // Event type should be 'TYPE_RECEIVE' to use this method.
  ENET_PLUS_DECL void GetData(std::vector<char>* output) const;

  // Returns 'Peer', which caused the event. Returned 'Peer' will be
  // deallocated automatically.
  // Event type should not be 'TYPE_NONE' to use this method.
  ENET_PLUS_DECL Peer* GetPeer();

 private:
  // Creates an uninitialized 'Event'. Don't use it yourself.
  // You can create an 'Event' by using 'Enet::CreateEvent()'.
  Event();

  // Destroys the packet that is hold by '_event'. If it hasn't been
  // received yet or has been destroyed already - nothing happens.
  void _DestroyPacket();

  _ENetEvent* _event;

  // 'False' if a packet received using 'ClientHost::Service()' or
  // 'ServerHost::Service()' hasn't been deallocated yet.
  bool _is_packet_destroyed;

  // 'Host' that generated the event.
  Host* _host;

  DISALLOW_COPY_AND_ASSIGN(Event);
};

}  // namespace enet

#endif  // ENET_PLUS_EVENT_HPP_
