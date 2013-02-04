#ifndef ENET_PLUS_SERVER_HOST_HPP_
#define ENET_PLUS_SERVER_HOST_HPP_

#include <enet-plus/base/macros.hpp>
#include <enet-plus/base/pstdint.hpp>

#include <enet-plus/host.hpp>

#include <enet-plus/dll.hpp>

struct _ENetHost;

namespace enet {

class Enet;
class Event;

// A server host for communicating with client hosts.
// You can create a 'ServerHost' by using 'Enet::CreateServerHost'.
class ServerHost : public Host {
  friend class Enet;

 public:
  ENET_PLUS_DECL ~ServerHost();

  // Initializes 'ServerHost'. 'ServerHost' starts on port 'port'.
  // You may specify 'channel_count' - number of channels to be used.
  // You may specify incoming and outgoing bandwidth of the server in bytes
  // per second. Specifying '0' for these two options will cause ENet to rely
  // entirely upon its dynamic throttling algorithm to manage bandwidth.
  ENET_PLUS_DECL bool Initialize(
    uint16_t port,
    size_t peer_count = 32,
    size_t channel_count = 1,
    uint32_t incoming_bandwidth = 0,
    uint32_t outgoing_bandwidth = 0
  );

  // Cleans up. Automatically called in the destructor.
  ENET_PLUS_DECL void Finalize();

  // Broadcast data from 'data' with length of 'length' to all Peer's on
  // selected channel, associated with 'channel_id'.
  // Returns 'true' on success, returns 'false' on error.
  ENET_PLUS_DECL bool Broadcast(
    const char* data,
    size_t length,
    bool reliable = true,
    uint8_t channel_id = 0
  );

 private:
  // Creates an uninitialized 'ServerHost'.
  ServerHost();

  enum {
    STATE_INITIALIZED,
    STATE_FINALIZED
  } _state;

  DISALLOW_COPY_AND_ASSIGN(ServerHost);
};

} // namespace enet

#endif // ENET_PLUS_SERVER_HOST_HPP_
