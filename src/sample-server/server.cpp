#include <enet-plus/base/error.hpp>

#include <enet-plus/enet.hpp>

using namespace enet;

int main() {
  Enet enet;
  bool rv = enet.Initialize();
  CHECK(rv == true);

  ServerHost* server = enet.CreateServerHost(4242);
  CHECK(server != NULL);

  Event* event = enet.CreateEvent();
  CHECK(event != NULL);

  printf("Server started.\n");

  uint32_t timeout = 100;
  std::vector<char> data;

  bool is_running = true;
  int counter = 0;

  while(is_running) {
    bool rv = server->Service(event, timeout);
    CHECK(rv);

    switch(event->GetType()) {
      case Event::TYPE_CONNECT: {
        printf("Client %s:%u connected.\n",
          event->GetPeer()->GetIp().c_str(),
          event->GetPeer()->GetPort());

        Peer* peer = event->GetPeer();
        CHECK(peer != NULL);
        char msg[] = "Ohaio!";
        data.assign(msg, msg + sizeof(msg));
        peer->Send(&data[0], data.size());

        break;
      }

      case Event::TYPE_RECEIVE: {
        event->GetData(&data);
        data.push_back(0);
        printf("From %s:%u: %s\n",
          event->GetPeer()->GetIp().c_str(),
          event->GetPeer()->GetPort(), &data[0]);
        break;
      }

      case Event::TYPE_DISCONNECT: {
        printf("Client %s:%u disconnected.\n",
          event->GetPeer()->GetIp().c_str(),
          event->GetPeer()->GetPort());

        counter += 1;
        if(counter == 3) {
          is_running = false;
        }

        break;
      }
    }
  }

  printf("Server finished.\n");

  delete event;

  delete server;

  return EXIT_SUCCESS;
}
