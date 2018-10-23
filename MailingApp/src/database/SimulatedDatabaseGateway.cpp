#include "SimulatedDatabaseGateway.h"



SimulatedDatabaseGateway::SimulatedDatabaseGateway()
{}


SimulatedDatabaseGateway::~SimulatedDatabaseGateway()
{}

IDatabaseGateway::Client SimulatedDatabaseGateway::getClientInfo(const std::string client)
{
	IDatabaseGateway::Client query_client("null");

	for (std::vector<IDatabaseGateway::Client>::const_iterator it = allClients.begin(); it != allClients.end(); ++it)
	{
		if (it->name == client)
			return *it;
	}

	return query_client;
}

void SimulatedDatabaseGateway::insertMessage(const Message & message)
{
	allMessages.push_back(message);
}

void SimulatedDatabaseGateway::insertClient(const std::string client)
{
	for (std::vector<IDatabaseGateway::Client>::iterator it = allClients.begin(); it != allClients.end(); ++it)
	{
		if (it->name == client)
		{
			printf("Client already added");
			return;
		}
	}

	allClients.push_back(client);
}

void SimulatedDatabaseGateway::blockClient(const std::string client, const std::string blocked)
{
	if (blocked == "null")
		return;

	for (std::vector<IDatabaseGateway::Client>::iterator it = allClients.begin(); it != allClients.end(); ++it)
	{
		if (it->name == client)
		{
			bool is_blocked = false;

			for (std::vector<std::string>::const_iterator it2 = it->blocked.begin(); it2 != it->blocked.end(); ++it2)
			{
				if (*it2 == blocked)
					is_blocked = true;
			}

			if (is_blocked == true)
			{
				printf("Usear already blocked");
				return;
			}

			it->blocked.push_back(blocked);
		}
	}
}

void SimulatedDatabaseGateway::unblockClient(const std::string client, const std::string unblocked)
{
	for (std::vector<IDatabaseGateway::Client>::iterator it = allClients.begin(); it != allClients.end(); ++it)
	{
		if (it->name == client)
		{
			for (std::vector<std::string>::iterator it2 = it->blocked.begin(); it2 != it->blocked.end(); ++it2)
			{
				if (*it2 == unblocked)
				{
					it->blocked.erase(it2);
					break;
				}
			}
		}
	}
}

std::vector<Message> SimulatedDatabaseGateway::getAllMessagesReceivedByUser(const std::string & username)
{
	std::vector<Message> messages;
	for (const auto & message : allMessages)
	{
		if (message.receiverUsername == username)
		{
			messages.push_back(message);
		}
	}
	return messages;
}

void SimulatedDatabaseGateway::updateGUI()
{
}
