// Copyright (c) 2013 Andrey Konovalov

#include <vector>

#include "enet-plus/enet.h"

int main() {
  enet::Enet enet;
  bool rv = enet.Initialize();
  CHECK(rv == true);

  enet::ClientHost* client = enet.CreateClientHost();
  CHECK(client != NULL);

  enet::Peer* peer = client->Connect("127.0.0.1", 4242);
  CHECK(peer != NULL);

  enet::Event* event = enet.CreateEvent();

  uint32_t timeout = 100;

  rv = client->Service(event, timeout);
  CHECK(rv == true);
  CHECK(event->GetType() == enet::Event::TYPE_CONNECT);

  printf("Connected to %s:%u.\n",
    event->GetPeer()->GetIp().c_str(),
    event->GetPeer()->GetPort());

  rv = client->Service(event, timeout);
  CHECK(rv == true);
  CHECK(event->GetType() == enet::Event::TYPE_RECEIVE);

  std::vector<char> msg;
  event->GetData(&msg);
  msg.push_back(0);

  printf("Message received: %s\n", &msg[0]);

  char message[] = "Hello world!";
  rv = peer->Send(message, sizeof(message), true);
  CHECK(rv == true);

  rv = client->Service(event, timeout);
  CHECK(rv == true);

  printf("Message sent: %s\n", message);

  peer->Disconnect();

  rv = client->Service(event, timeout);
  CHECK(rv == true);
  CHECK(event->GetType() == enet::Event::TYPE_DISCONNECT);

  printf("Disconnected from %s:%u.\n",
    event->GetPeer()->GetIp().c_str(),
    event->GetPeer()->GetPort());

  delete event;

  delete client;

  return 0;
}
