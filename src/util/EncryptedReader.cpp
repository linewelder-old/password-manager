#include "util/EncryptedReader.h"

#include <sstream>
#include <string>
#include <optional>
#include "data/exceptions/InvalidDataException.h"
#include "util/exceptions/InvalidFileFormatException.h"
#include "util/consts.h"

EncryptedReader::EncryptedReader(std::istream& stream, std::string password)
    : stream(stream), context(password) {}

time_t EncryptedReader::get_read_last_opening_time()
{
    return read_last_opening_time;
}

static std::optional<std::string> to_optional_string(const std::string& str)
{
    if (str.empty())
    {
        return std::nullopt;
    }
    else
    {
        return str;
    }
}

void EncryptedReader::read_to_base(PasswordBase& base)
{
    uint8_t signature[2];
    signature[0] = read_byte();
    signature[1] = read_byte();
    if (signature[0] != FILE_SIGNATURE[0] || signature[1] != FILE_SIGNATURE[1])
    {
        throw InvalidFileFormatException(
            InvalidFileFormatExceptionType::INVALID_SIGNATURE);
    }

    const uint8_t format_version = read_byte();
    if (format_version != FILE_FORMAT_VERSION)
    {
        throw InvalidFileFormatException(
            InvalidFileFormatExceptionType::UNSUPPORTED_FILE_FORMAT_VERSION);
    }

    read_last_opening_time = read_int64();

    while(true)
    {
        const std::string category = read_string();
        if (category.empty())
        {
            break;
        }
        const CategoryId id = read_int16();

        base.insert_category(id, category);
    }

    while (true)
    {
        const std::optional<Password> password = try_read_password();
        if (!password.has_value())
        {
            break;
        }

        base.add_password(password.value());
    }
}

std::optional<Password> EncryptedReader::try_read_password()
{
    Password password;

    password.name = read_string();
    if (password.name.empty())
    {
        return std::nullopt;
    }

    password.value       = read_string();
    password.category_id = read_int16();
    password.service     = to_optional_string(read_string());
    password.login       = to_optional_string(read_string());

    return password;
}

std::string EncryptedReader::read_string()
{
    std::ostringstream ss;

    while (true)
    {
        uint8_t ch = read_byte();
        if (ch == '\0')
        {
            break;
        }

        ss.put(decrypt_value(ch, MIN_STRING_CHAR, MAX_STRING_CHAR));
    }

    return ss.str();
}

uint64_t EncryptedReader::read_int(size_t length_in_bytes)
{
    uint64_t value = 0;
    for (size_t i = 0; i < length_in_bytes; i++)
    {
        value |= (uint64_t)read_byte() << (i * 8);
    }

    return value;
}

uint64_t EncryptedReader::read_int64()
{
    return read_int(8);
}

uint16_t EncryptedReader::read_int16()
{
    return read_int(2);
}

int EncryptedReader::decrypt_value(int value, int min, int max)
{
    if (value < min || value >= max)
    {
        throw InvalidFileFormatException(
            InvalidFileFormatExceptionType::VALUE_OUT_OF_BOUNDS);
    }

    const int range = max - min;
    return euclidean_modulo(value - min - context.next_encryption_value(), range) + min;
}

uint8_t EncryptedReader::read_byte()
{
    const int value = stream.get();
    if (value == -1)
    {
        throw InvalidFileFormatException(InvalidFileFormatExceptionType::UNEXPECTED_EOF);
    }

    return value;
}
