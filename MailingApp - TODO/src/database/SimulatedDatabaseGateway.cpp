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
	allClients.push_back(client);
}

void SimulatedDatabaseGateway::blockClient(const std::string client, const std::string blocked)
{
	for (std::vector<IDatabaseGateway::Client>::iterator it = allClients.begin(); it != allClients.end(); ++it)
	{
		if (it->name == client)
		{
			for (int i = 0; i < 5; i++)
			{
				if (it->blocked[i] == "null")
				{
					it->blocked[i] = blocked;
					break;
				}
				else if(i == 4)
					printf("Allready blocked 5 Clients");
			}
		}
	}
}

void SimulatedDatabaseGateway::unblockClient(const std::string client, const std::string unblocked)
{
	for (std::vector<IDatabaseGateway::Client>::iterator it = allClients.begin(); it != allClients.end(); ++it)
	{
		if (it->name == client)
		{
			for (int i = 0; i < 5; i++)
			{
				if (it->blocked[i] == unblocked)
				{
					it->blocked[i] = "null";
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
