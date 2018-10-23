#pragma once

#include <cstdint>

enum class PacketType : int8_t
{
	LoginRequest,
	LoginInfo,
	QueryAllMessagesRequest,
	QueryAllMessagesResponse,
	SendMessageRequest,
	Block,
	Unblock
};
