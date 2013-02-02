#include <base/error.hpp>

#include <enet-plus/enet.hpp>

using namespace bm;

int main() {
  Enet enet;
  bool rv = enet.Initialize();
  CHECK(rv == true);

  ClientHost* client = enet.CreateClientHost();
  CHECK(client != NULL);

  Peer* peer = client->Connect("127.0.0.1", 4242);
  CHECK(peer != NULL);
 
  Event* event = enet.CreateEvent();

  uint32_t timeout = 5000;

  rv = client->Service(event, timeout);
  CHECK(rv);
  CHECK(event->GetType() == Event::EVENT_CONNECT);

  printf("Connected to %s:%u.\n", event->GetPeerIp().c_str(), event->GetPeerPort());

  rv = client->Service(event, timeout);
  CHECK(rv);
  CHECK(event->GetType() == Event::EVENT_RECEIVE);

  std::vector<char> msg;
  event->GetData(&msg);
  msg.push_back(0);

  printf("Message received: %s\n", &msg[0]);
  
  char message[] = "Hello world!";
  rv = peer->Send(message, sizeof(message));
  CHECK(rv);

  client->Flush();

  printf("Message sent: %s\n", message);

  peer->Disconnect();

  rv = client->Service(event, timeout);
  CHECK(rv);
  CHECK(event->GetType() == Event::EVENT_DISCONNECT);

  printf("Disconnected from %s:%u.\n", event->GetPeerIp().c_str(), event->GetPeerPort());

  delete event;
  delete peer;

  client->Destroy();
  delete client;

  return 0;
}
