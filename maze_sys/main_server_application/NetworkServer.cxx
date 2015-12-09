NetServer::NetServer()
{
	NetworkServer* server;
	robotList = new vector<Robot>();
	server = new NetworkServer();
	if ( server->GetInitCode() || !server->Create(1024) || !server->Good())
	{
		std::cout << "An error occurred while trying to create an ENet object." << std::endl;
		exit(EXIT_FAILURE);
	}
}

ENetEvent NetServer::Receive()
{
	if (server->Pull())
	{
		ENetEvent event = server->Event();
		switch (event.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n",
					event.peer->address.host,
					event.peer->address.port);
				/* Store any relevant client information here. */
				event.peer->data = "Client information";
				Robot robot = new Robot(event.peer);
				robotList.push_back(robot);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("%s\n", event.packet->data);
				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", event.peer->data);
				/* Reset the peer's client information. */
				event.peer->data = NULL;
				break;
		}
		if (event.type) {
			return event.type;
		}
	}
	return NULL;
}

bool NetServer::Send(Robot robot, QueueItem item)
{
	if (robot && item)
	{
		ENetPeer* peer = &(robot->GetPeer());
		if (server->Send(	peer
						item,
						sizeof item,
						ENET_PACKET_FLAG_RELIABLE))
		{
			return true;
		}
	}
	return false;
}
