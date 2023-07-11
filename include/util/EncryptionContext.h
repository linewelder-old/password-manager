#pragma once

#include <string>

class EncryptionContext
{
	const std::string password;
	int offset = 0;

public:
	EncryptionContext(const std::string password);
	int next_encryption_value();
};
