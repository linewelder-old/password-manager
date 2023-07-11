#pragma once

#include <iostream>
#include <cstdint>
#include "data/MemoryPasswordBase.h"
#include "EncryptionContext.h"

class EncryptedWriter
{
    std::ostream& stream;
    EncryptionContext context;

public:
    EncryptedWriter(std::ostream& stream, std::string password);
    void write_base(const PasswordBase& base);

private:
    void write_password(const Password& password);
    void write_string(const std::string& value);
    void write_int(uint64_t value, size_t length_in_bytes);
    void write_int64(uint64_t value);
    void write_int16(uint16_t value);
    void write_byte(uint8_t value);

    /**
    Szyfruje podana wartosc, przyjmujac, ze nalezy ona do przedzialu [min, max)
    w taki sposob, ze rezultat tez nalezy do tego przedzialu.
    */
    int encrypt_value(int value, int min, int max);
};
