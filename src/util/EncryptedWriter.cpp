#include "util/EncryptedWriter.h"

#include <sstream>
#include <string>
#include "util/consts.h"

EncryptedWriter::EncryptedWriter(std::ostream& stream, std::string password)
    : stream(stream), context(password) {}

void EncryptedWriter::write_base(const PasswordBase& base)
{
    write_byte(FILE_SIGNATURE[0]);
    write_byte(FILE_SIGNATURE[1]);
    write_byte(FILE_FORMAT_VERSION);

    write_int64(time(0));
    
    const auto categories = base.fetch_all_categories();
    for (const auto& category : categories)
    {
        write_string(category.second);
        write_int16(category.first);
    }
    write_byte(0);

    auto passwords = base.fetch_passwords({}, {});
    for (auto& password : passwords)
    {
        write_password(password.second);
    }
    write_byte(0);
}

void EncryptedWriter::write_password(const Password& password)
{
    write_string(password.name);
    write_string(password.value);
    write_int16( password.category_id);
    write_string(password.service.value_or(""));
    write_string(password.login  .value_or(""));
}

void EncryptedWriter::write_string(const std::string& value)
{
    for (char ch : value)
    {
        write_byte(encrypt_value(ch, MIN_STRING_CHAR, MAX_STRING_CHAR));
    }

    write_byte('\0');
}

void EncryptedWriter::write_int(uint64_t value, size_t length_in_bytes)
{
    for (size_t i = 0; i < length_in_bytes; i++)
    {
        write_byte(value & 0xFF);
        value >>= 8;
    }
}

void EncryptedWriter::write_int64(uint64_t value)
{
    write_int(value, 8);
}

void EncryptedWriter::write_int16(uint16_t value)
{
    write_int(value, 2);
}

int EncryptedWriter::encrypt_value(int value, int min, int max)
{
    int range = max - min;
    return euclidean_modulo(value - min + context.next_encryption_value(), range) + min;
}

void EncryptedWriter::write_byte(uint8_t byte)
{
    stream.put(byte);
}
