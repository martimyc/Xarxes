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
		std::string sqlStatement("SELECT * FROM "+ std::string(bufMySqlDatabase) +".Clients WHERE idClient = '" + client + "';");
		DBResultSet res = db.sql(sqlStatement.c_str());

		if (!res.rows.empty())
		{
			query_client.name = res.rows[0].columns[0];

			std::string blocked_list(res.rows[0].columns[1]);

			if (blocked_list != "null")
			{
				//Split string into substringd for each blocked.
				size_t len = blocked_list.find_first_of(","), start_pos = 0;

				while (start_pos != std::string::npos)
				{
					if(len == std::string::npos)
						query_client.blocked.push_back(blocked_list.substr(start_pos));
					else
						query_client.blocked.push_back(blocked_list.substr(start_pos, len));

					 if (len == std::string::npos)
						 start_pos = std::string::npos;
					 else
					 {
						 start_pos += len + 1;
						 len = blocked_list.substr(start_pos).find_first_of(",");
					 }					
				}
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

		sqlStatement = "INSERT " + std::string(bufMySqlDatabase) + ".Messages VALUES ('" + message.receiverUsername + "', '" + message.senderUsername + "', '" + message.subject + "', '" + message.body + "');";

		// insert some messages
		db.sql(sqlStatement.c_str());
	}
}

void MySqlDatabaseGateway::insertClient(const std::string client)
{
	DBConnection db(bufMySqlHost, bufMySqlPort, bufMySqlDatabase, bufMySqlUsername, bufMySqlPassword);

	if (db.isConnected() && client != "null" && client != "")
	{
		std::string sqlStatement("SELECT * FROM " + std::string(bufMySqlDatabase) + ".Clients WHERE idClient = '" + client + "';");
		DBResultSet res = db.sql(sqlStatement.c_str());
		
		if (!res.rows.empty())
		{		
			printf("Client already created");
			return;
		}
		
		std::string sqlStatement2;
		
		sqlStatement2 = "INSERT " + std::string(bufMySqlDatabase) + ".Clients VALUES ('" + client + "', 'null');";

		db.sql(sqlStatement2.c_str());
	}
}

void MySqlDatabaseGateway::blockClient(const std::string client, const std::string blocked)
{
	DBConnection db(bufMySqlHost, bufMySqlPort, bufMySqlDatabase, bufMySqlUsername, bufMySqlPassword);

	if (db.isConnected() && client != "null" && client != "" && blocked != "null" && blocked != "")
	{
		std::string sqlStatement("SELECT * FROM " + std::string(bufMySqlDatabase) + ".Clients WHERE idClient = '" + client + "';");
		DBResultSet res = db.sql(sqlStatement.c_str());

		std::string blocked_list(res.rows[0].columns[1]);
		
		if(blocked_list != "null")
			sqlStatement = "UPDATE " + std::string(bufMySqlDatabase) + ".Clients SET Blocked = '" + blocked_list + "," + blocked + "' WHERE idClient = '" + client + "';";
		else
			sqlStatement = "UPDATE " + std::string(bufMySqlDatabase) + ".Clients SET Blocked = '" + blocked + "' WHERE idClient = '" + client + "';";

		// insert some messages
		db.sql(sqlStatement.c_str());
	}
}

void MySqlDatabaseGateway::unblockClient(const std::string client, const std::string unblocked)
{
	DBConnection db(bufMySqlHost, bufMySqlPort, bufMySqlDatabase, bufMySqlUsername, bufMySqlPassword);

	if (db.isConnected() && client != "null" && client != "" && unblocked != "null" && unblocked != "")
	{
		std::string sqlStatement("SELECT * FROM " + std::string(bufMySqlDatabase) + ".Clients WHERE idClient = '" + client + "';");
		DBResultSet res = db.sql(sqlStatement.c_str());

		std::string blocked_list(res.rows[0].columns[1]);

		if (blocked_list == "null")
		{
			printf("There are no blocked users to unblock");
			return;
		}

		std::string new_list;

		//Split string into substringd for each blocked.
		size_t len = blocked_list.find_first_of(","), start_pos = 0;

		while (start_pos != std::string::npos)
		{
			std::string str;

			if (len == std::string::npos)
			{
				str = blocked_list.substr(start_pos);
				
				if (str != unblocked)
					new_list += str;
			}
			else
			{
				str = blocked_list.substr(start_pos, len);
				
				if (str != unblocked)
				{
					new_list += str;
					new_list += ",";
				}				
			}			

			if (len == std::string::npos)
				start_pos = std::string::npos;
			else
			{
				start_pos += len + 1;
				len = blocked_list.substr(start_pos).find_first_of(",");
			}
		}

		sqlStatement = "UPDATE " + std::string(bufMySqlDatabase) + ".Clients SET Blocked = '" + new_list + "' WHERE idClient = '" + client + "';";

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

		sqlStatement = "SELECT * FROM " + std::string(bufMySqlDatabase) + ".Messages WHERE Receiver = '" + username + "';";

		// consult all messages
		DBResultSet res = db.sql(sqlStatement.c_str());

		// fill the array of messages
		for (auto & messageRow : res.rows)
		{
			Message message;
			message.receiverUsername = messageRow.columns[0];
			message.senderUsername = messageRow.columns[1];			
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
