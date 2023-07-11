#include "util/EncryptionContext.h"

EncryptionContext::EncryptionContext(const std::string password)
	: password(password)
{
	if (password.empty())
	{
		throw "Puste haslo";
	}
}

int EncryptionContext::next_encryption_value()
{
	const char next_char = password.at(offset++);
	if (offset >= password.size())
	{
		offset = 0;
	}

	return next_char;
}
