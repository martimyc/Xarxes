#include "MySqlDatabaseGateway.h"
#include "DBConnection.h"
#include "../imgui/imgui.h"
#include <cstdarg>

// You can use this function to create the SQL statements easily, works like the printf function
std::string stringFormat(const char *fmt, ...)
{
	// Find out the final string length
	va_list ap;
	va_start(ap, fmt);
	int size = vsnprintf(nullptr, 0, fmt, ap);
	va_end(ap);

	// Format the actual resulting string
	std::string resultString;
	resultString.resize(size + 1, '\0');
	va_start(ap, fmt);
	vsnprintf(&resultString[0], resultString.size(), fmt, ap);
	va_end(ap);

	return resultString;
}


MySqlDatabaseGateway::MySqlDatabaseGateway()
{}


MySqlDatabaseGateway::~MySqlDatabaseGateway()
{}

IDatabaseGateway::Client MySqlDatabaseGateway::getClientInfo(const std::string client)
{
	IDatabaseGateway::Client query_client("null");

	DBConnection db(bufMySqlHost, bufMySqlPort, bufMySqlDatabase, bufMySqlUsername, bufMySqlPassword);

	if (db.isConnected())
	{
		std::string sqlStatement("SELECT * FROM martimy.Clients WHERE idClients = '" + client + "';");
		DBResultSet res = db.sql(sqlStatement.c_str());

		if (!res.rows.empty())
		{
			query_client.name = res.rows[0].columns[0];
			
			for (int i = 0; i < 5; i++)
			{
				query_client.blocked[i] = res.rows[0].columns[i+1];
			}
		}
	}
	return query_client;
}

void MySqlDatabaseGateway::insertMessage(const Message & message)
{
	DBConnection db(bufMySqlHost, bufMySqlPort, bufMySqlDatabase, bufMySqlUsername, bufMySqlPassword);

	if (db.isConnected())
	{
		DBResultSet res;

		std::string sqlStatement;
		// TODO: Create the SQL statement to insert the passed message into the DB (INSERT)

		sqlStatement = "INSERT martimy.Messages VALUES ('" + message.receiverUsername + "', '" + message.senderUsername + "', '" + message.subject + "', '" + message.body + "');";

		// insert some messages
		db.sql(sqlStatement.c_str());
	}
}

void MySqlDatabaseGateway::insertClient(const std::string client)
{
	DBConnection db(bufMySqlHost, bufMySqlPort, bufMySqlDatabase, bufMySqlUsername, bufMySqlPassword);

	if (db.isConnected() && client != "null" && client != "")
	{
		DBResultSet res;

		std::string sqlStatement;
		
		sqlStatement = "INSERT martimy.Clients VALUES ('" + client + "', 'null','null','null','null','null');";

		db.sql(sqlStatement.c_str());
	}
}

void MySqlDatabaseGateway::blockClient(const std::string client, const std::string blocked)
{
	DBConnection db(bufMySqlHost, bufMySqlPort, bufMySqlDatabase, bufMySqlUsername, bufMySqlPassword);

	if (db.isConnected() && client != "null" && client != "" && blocked != "null" && blocked != "")
	{
		std::string sqlStatement("SELECT * FROM martimy.Clients WHERE idClients = '" + client + "';");
		DBResultSet res = db.sql(sqlStatement.c_str());

		for (auto & b : res.rows)
		{
			if (b.columns[1] == "null")
				sqlStatement = "UPDATE martimy.Clients SET Blocked1 = '" + blocked + "' WHERE idClients = '" + client + "';";
			else if (b.columns[2] == "null")
				sqlStatement = "UPDATE martimy.Clients SET Blocked2 = '" + blocked + "' WHERE idClients = '" + client + "';";
			else if (b.columns[3] == "null")
				sqlStatement = "UPDATE martimy.Clients SET Blocked3 = '" + blocked + "' WHERE idClients = '" + client + "';";
			else if (b.columns[4] == "null")
				sqlStatement = "UPDATE martimy.Clients SET Blocked4 = '" + blocked + "' WHERE idClients = '" + client + "';";
			else if (b.columns[4] == "null")
				sqlStatement = "UPDATE martimy.Clients SET Blocked5 = '" + blocked + "' WHERE idClients = '" + client + "';";
			else
				printf("Allready blocked 5 Clients");
		}

		// insert some messages
		db.sql(sqlStatement.c_str());
	}
}

void MySqlDatabaseGateway::unblockClient(const std::string client, const std::string unblocked)
{
	DBConnection db(bufMySqlHost, bufMySqlPort, bufMySqlDatabase, bufMySqlUsername, bufMySqlPassword);

	if (db.isConnected() && client != "null" && client != "" && unblocked != "null" && unblocked != "")
	{
		std::string sqlStatement("SELECT * FROM martimy.Clients WHERE idClients = '" + client + "';");
		DBResultSet res = db.sql(sqlStatement.c_str());

		for (auto & b : res.rows)
		{
			if (b.columns[1] == unblocked)
				sqlStatement = "UPDATE martimy.Clients SET Blocked1 = 'null' WHERE idClients = '" + client + "';";
			else if (b.columns[2] == unblocked)
				sqlStatement = "UPDATE martimy.Clients SET Blocked2 = 'null' WHERE idClients = '" + client + "';";
			else if (b.columns[3] == unblocked)
				sqlStatement = "UPDATE martimy.Clients SET Blocked3 = 'null' WHERE idClients = '" + client + "';";
			else if (b.columns[4] == unblocked)
				sqlStatement = "UPDATE martimy.Clients SET Blocked4 = 'null' WHERE idClients = '" + client + "';";
			else if (b.columns[4] == unblocked)
				sqlStatement = "UPDATE martimy.Clients SET Blocked5 = 'null' WHERE idClients = '" + client + "';";
			else
				printf("Client was not blocked");
		}

		// insert some messages
		db.sql(sqlStatement.c_str());
	}
}

std::vector<Message> MySqlDatabaseGateway::getAllMessagesReceivedByUser(const std::string & username)
{
	std::vector<Message> messages;

	DBConnection db(bufMySqlHost, bufMySqlPort, bufMySqlDatabase, bufMySqlUsername, bufMySqlPassword);

	if (db.isConnected() && username != "null" && username != "")
	{
		std::string sqlStatement;
		// TODO: Create the SQL statement to query all messages for the given user (SELECT)

		sqlStatement = "SELECT * FROM martimy.Messages WHERE Receiver = '" + username + "';";

		// consult all messages
		DBResultSet res = db.sql(sqlStatement.c_str());

		// fill the array of messages
		for (auto & messageRow : res.rows)
		{
			Message message;
			message.senderUsername = messageRow.columns[0];
			message.receiverUsername = messageRow.columns[1];
			message.subject = messageRow.columns[2];
			message.body = messageRow.columns[3];
			messages.push_back(message);
		}
	}

	return messages;
}

void MySqlDatabaseGateway::updateGUI()
{
	ImGui::Separator();

	ImGui::Text("MySQL Server info");
	ImGui::InputText("Host", bufMySqlHost, sizeof(bufMySqlHost));
	ImGui::InputText("Port", bufMySqlPort, sizeof(bufMySqlPort));
	ImGui::InputText("Database", bufMySqlDatabase, sizeof(bufMySqlDatabase));
	ImGui::InputText("Username", bufMySqlUsername, sizeof(bufMySqlUsername));
	ImGui::InputText("Password", bufMySqlPassword, sizeof(bufMySqlUsername), ImGuiInputTextFlags_Password);
}
