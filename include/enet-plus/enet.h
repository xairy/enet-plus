// Copyright (c) 2013 Andrey Konovalov

#ifndef ENET_PLUS_ENET_HPP_
#define ENET_PLUS_ENET_HPP_

#include <enet-plus/base/macros.hpp>
#include <enet-plus/base/pstdint.hpp>

#include "enet-plus/host.h"
#include "enet-plus/server_host.h"
#include "enet-plus/client_host.h"
#include "enet-plus/event.h"
#include "enet-plus/peer.h"

#include "enet-plus/dll.h"

namespace enet {

// You need to create and 'Initialize()' a 'Enet' instance to work with ENet.
class Enet {
 public:
  ENET_PLUS_DECL Enet();
  ENET_PLUS_DECL ~Enet();

  // Initializes ENet.
  // Returns 'true' on success, returns 'false' on error.
  ENET_PLUS_DECL bool Initialize();

  // Cleans up.
  // Automatically called in the destructor.
  ENET_PLUS_DECL void Finalize();

  // Creates 'ServerHost' bound to 'port'.
  // You may specify 'channel_count' - number of channels to be used.
  // You may specify incoming and outgoing bandwidth of the server in bytes
  // per second. Specifying '0' for these two options will cause ENet to rely
  // entirely upon its dynamic throttling algorithm to manage bandwidth.
  // Returned 'ServerHost' should be deallocated manually using 'delete'.
  // Returns 'NULL' on error.
  ENET_PLUS_DECL ServerHost* CreateServerHost(
    uint16_t port,
    size_t peer_count = 32,
    size_t channel_count = 1,
    uint32_t incoming_bandwith = 0,
    uint32_t outgoing_bandwith = 0);

  // Creates 'ClientHost'.
  // You may specify 'channel_count' - number of channels to be used.
  // You may specify incoming and outgoing bandwidth of the server in bytes
  // per second. Specifying '0' for these two options will cause ENet to rely
  // entirely upon its dynamic throttling algorithm to manage bandwidth.
  // Returned 'ClientHost' should be deallocated manually using 'delete'.
  // Returns 'NULL' on error.
  ENET_PLUS_DECL ClientHost* CreateClientHost(
    size_t channel_count = 1,
    uint32_t incoming_bandwith = 0,
    uint32_t outgoing_bandwith = 0);

  // Creates an empty Event.
  // Returned 'Event' should be deallocated manually using 'delete'.
  ENET_PLUS_DECL Event* CreateEvent();

 private:
  enum {
    STATE_FINALIZED,
    STATE_INITIALIZED,
  } _state;

  DISALLOW_COPY_AND_ASSIGN(Enet);
};

}  // namespace enet

#endif  // ENET_PLUS_ENET_HPP_
