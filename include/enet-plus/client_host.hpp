#ifndef ENET_PLUS_CLIENT_HOST_HPP_
#define ENET_PLUS_CLIENT_HOST_HPP_

#include <string>

#include <enet-plus/base/macros.hpp>
#include <enet-plus/base/pstdint.hpp>

#include <enet-plus/host.hpp>

#include <enet-plus/dll.hpp>

struct _ENetHost;

namespace enet {

class Enet;
class Event;
class Peer;

// A client host for communicating with a server host.
// You can create a 'ClientHost' by using 'Enet::CreateClientHost'.
class ClientHost : public Host {
  friend class Enet;

 public:
  ENET_PLUS_DECL ~ClientHost();

  // Initializes 'ClientHost'.
  // You may specify 'channel_count' - number of channels to be used.
  // You may specify incoming and outgoing bandwidth of the server in bytes
  // per second. Specifying '0' for these two options will cause ENet to rely
  // entirely upon its dynamic throttling algorithm to manage bandwidth.
  ENET_PLUS_DECL bool Initialize(
    size_t channel_count = 1,
    uint32_t incoming_bandwidth = 0,
    uint32_t outgoing_bandwidth = 0
  );

  // Cleans up. Automatically called in the destructor.
  ENET_PLUS_DECL void Finalize();

  // Initiates connection procedure to another host. To complete connection,
  // an event 'EVENT_CONNECT' should be dispatched using 'Service()'.
  // 'channel_count' is the number of channels to be used.
  // Returns 'Peer' on success, returns 'NULL' on error.
  // Returned 'Peer' should be deallocated manually using 'delete'.
  ENET_PLUS_DECL Peer* Connect(
    std::string server_ip,
    uint16_t port,
    size_t channel_count = 1
  );

 private:
  // Creates an uninitialized 'ClientHost'.
  ClientHost();

  enum {
    STATE_FINALIZED,
    STATE_INITIALIZED,
  } _state;

  DISALLOW_COPY_AND_ASSIGN(ClientHost);
};

} // namespace enet

#endif // ENET_PLUS_CLIENT_HOST_HPP_
