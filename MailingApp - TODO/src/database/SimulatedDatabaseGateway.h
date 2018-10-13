#pragma once

#include "IDatabaseGateway.h"
#include <vector>

class SimulatedDatabaseGateway :
	public IDatabaseGateway
{
public:

	// Constructor and destructor

	SimulatedDatabaseGateway();

	~SimulatedDatabaseGateway();


	// Virtual methods from IDatabaseGateway
	virtual IDatabaseGateway::Client getClientInfo(const std::string client) override;

	void insertMessage(const Message &message) override;

	void insertClient(const std::string client) override;

	void blockClient(const std::string client, const std::string blocked) override;

	void unblockClient(const std::string client, const std::string unblocked) override;

	std::vector<Message> getAllMessagesReceivedByUser(const std::string &username) override;

	virtual void updateGUI() override;

private:

	std::vector<Message> allMessages;
	std::vector<IDatabaseGateway::Client> allClients;
};
