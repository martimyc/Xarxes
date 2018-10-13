#pragma once

#include "DatabaseTypes.h"
#include <vector>

class IDatabaseGateway
{
public:

	struct Client {
		std::string name;
		std::string blocked[5];

		Client(const std::string str): name(str)
		{
			for (int i = 5; i < 5; i++)
				blocked[i] = "null";
		}
	};

	// Constructor and destructor

	IDatabaseGateway() { }
	
	virtual ~IDatabaseGateway() { }


	// Virtual methods

	virtual Client getClientInfo(const std::string client) = 0;

	virtual void insertMessage(const Message &message) = 0;

	virtual void insertClient(const std::string client) = 0;

	virtual void blockClient(const std::string client, const std::string blocked) = 0;

	virtual void unblockClient(const std::string client, const std::string unblocked) = 0;

	virtual std::vector<Message> getAllMessagesReceivedByUser(const std::string &username) = 0;

	virtual void updateGUI() { }
};

