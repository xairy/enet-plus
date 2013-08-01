#ifndef ENET_PLUS_HOST_HPP_
#define ENET_PLUS_HOST_HPP_

#include <map>
#include <string>

#include <enet-plus/base/macros.hpp>
#include <enet-plus/base/pstdint.hpp>

#include <enet-plus/dll.hpp>

struct _ENetHost;
struct _ENetPeer;

namespace enet {

class Enet;
class Event;
class Peer;

// Internally used class. Use 'ServerHost' and 'ClientHost' instead.
class Host {
  friend class Event;

 public:
  ENET_PLUS_DECL virtual ~Host();

  // Initializes 'Host'.
  // 'ip' and 'port' are the address at which other peers may connect to this
  // host. If 'ip' is an empty string and 'port' is '0', then no peers may
  // connect to the host.
  // 'peer_count' is the maximum number of peers that should be allocated for
  // the host. 'channel_count' is the maximum number of channels allowed. If
  // '0', then this is equivalent to 'ENET_PROTOCOL_MAXIMUM_CHANNEL_COUNT'.
  // 'incoming_bandwidth' and 'outgoing_bandwidth' are incoming and outgoing
  // bandwidth of the server in bytes per second. Specifying '0' for these two
  // options will cause ENet to rely entirely upon its dynamic throttling
  // algorithm to manage bandwidth.
  ENET_PLUS_DECL bool Initialize(
    const std::string& ip,
    uint16_t port,
    size_t peer_count,
    size_t channel_count,
    uint32_t incoming_bandwidth,
    uint32_t outgoing_bandwidth
  );

  // Cleans up. Automatically called in the destructor.
  ENET_PLUS_DECL void Finalize();

  // Checks for events with a timeout. Should be called to send all queued
  // with 'Peer::Send()' packets. 'event' is an 'Event' class where event
  // details will be placed if one occurs.
  // If a timeout of '0' is specified, 'Service()' will return immediately
  // if there are no events to dispatch. If 'event' is 'NULL' then no events
  // will be delivered.
  // An 'Event' with type 'EVENT_NONE' will be placed in 'event' if no event
  // occured within the specified time limit.
  // Returns 'true' on success, returns 'false' on error.
  ENET_PLUS_DECL virtual bool Service(Event* event, uint32_t timeout);

  // This function need only be used in circumstances where one wishes to send
  // queued packets earlier than in a call to 'Service()'.
  ENET_PLUS_DECL virtual void Flush();

 protected:
  // Creates an uninitialized 'Host'.
  Host();

  // Returns 'Peer' associated with ENet's peer 'enet_peer'.
  Peer* _GetPeer(_ENetPeer* enet_peer);

  enum {
    STATE_FINALIZED,
    STATE_INITIALIZED
  } _state;

  _ENetHost* _host;

  std::map<_ENetPeer*, Peer*> _peers;

 private:
  DISALLOW_COPY_AND_ASSIGN(Host);
};

} // namespace enet

#endif // ENET_PLUS_HOST_HPP_
