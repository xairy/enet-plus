#ifndef ENET_PLUS_EVENT_HPP_
#define ENET_PLUS_EVENT_HPP_

#include <string>
#include <vector>

#include <enet-plus/base/macros.hpp>
#include <enet-plus/base/pstdint.hpp>

#include <enet-plus/dll.hpp>

struct _ENetEvent;

namespace enet {

class Enet;
class ClientHost;
class ServerHost;
class Peer;

// 'Event' class represents an event that can be delivered by
// 'ClientHost::Service' and 'ServerHost::Service' methods.
// You can create an empty 'Event' by using 'Enet::CreateEvent'.
class Event {
  friend class ClientHost;
  friend class ServerHost;
  friend class Enet;

public:
  enum EventType {
    // No event occurred within the specified time limit.
    TYPE_NONE,

    // A connection request initiated by 'ClientHost::Connect()' has completed.
    // You can use 'GetPeer()', 'GetPeerIp()', 'GetPeerPort()' methods to get
    // information about connected peer.
    TYPE_CONNECT,

    // A peer has disconnected. This event is generated on a successful
    // completion of a disconnect initiated by 'Peer::Disconnect()'.
    // You can use 'GetPeer()', 'GetPeerIp()', 'GetPeerPort()' methods to get
    // information about disconnected peer.
    TYPE_DISCONNECT,  

    // A packet has been received from a peer.
    // You can use 'GetPeer()', 'GetPeerIp()', 'GetPeerPort()' methods to get
    // information about peer which sent the packet.
    // 'GetChannelId()' returns the channel number upon which the packet was
    // received. 'GetData()' returns the data from received packet.
    // This packet must be destroyed with 'DestroyPacket()' after use.
    TYPE_RECEIVE
  };

  ENET_PLUS_DECL ~Event();

  // Returns the type of the event.
  ENET_PLUS_DECL EventType GetType() const;

  // Returns the channel id on the peer that generated the event.
  ENET_PLUS_DECL uint8_t GetChannelId() const;

  // Returns the data associated with the event. Data will be lost if
  // another event is delivered using 'ClientHost::Service()' or
  // 'ServerHost::Service()' using this instance of 'Event' class.
  // This method will fail on 'CHECK' in this case.
  ENET_PLUS_DECL void GetData(std::vector<char>* output) const;

  // TODO: Get rid of this method.
  // Returns 'Peer', which caused the event. Returned 'Peer' should be
  // deallocated manually using 'delete'.
  // WARNING: This method allocates new 'Peer' even if a 'Peer' associated
  // with a remote peer already exists.
  // XXX: fix it?
  ENET_PLUS_DECL Peer* GetPeer();

  // Use this instead of 'GetPeer()' if you need to call only 'Peer's getters.
  // The description of these methods can be found in 'Peer' declaration.
  ENET_PLUS_DECL std::string GetPeerIp() const;
  ENET_PLUS_DECL uint16_t GetPeerPort() const;
  ENET_PLUS_DECL void* GetPeerData() const;

private:
  DISALLOW_COPY_AND_ASSIGN(Event);

  // Creates an uninitialized 'Event'. Don't use it yourself.
  // You can create an 'Event' by using 'Enet::CreateEvent'.
  Event();

  // Destroys the packet that is hold by '_event'. If it hasn't been
  // received yet or has been destroyed already - nothing happens.
  void _DestroyPacket();

  _ENetEvent* _event;
  
  // 'False' if a packet received using 'ClientHost::Service()' or
  // 'ServerHost::Service()' hasn't been deallocated yet.
  bool _is_packet_destroyed;
};

} // namespace enet

#endif // ENET_PLUS_EVENT_HPP_
