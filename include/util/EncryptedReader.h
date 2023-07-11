#pragma once

#include <iostream>
#include <cstdint>
#include <ctime>
#include "data/MemoryPasswordBase.h"
#include "EncryptionContext.h"

class EncryptedReader
{
    std::istream& stream;
    EncryptionContext context;
    time_t read_last_opening_time;

public:
    EncryptedReader(std::istream& stream, std::string password);

    /**
    * Czyta plik w podana baze.
    * Odczytany czas ostatniego otwarcia mozna zdobyc
    * przez metode get_read_last_opening_time().
    * \exception InvalidDataFormatException
    * \exception InvalidFileFormatException
     */
    void read_to_base(PasswordBase& base);

    time_t get_read_last_opening_time();

private:
    /**
    * Pyta sie odczytac haslo,
    * jesli zamiast hasla jest zero, nic nie zwraca.
    * Nie sprawdza poprawnosci odczytanych danych.
    * \exception InvalidFileFormatException
    */
    std::optional<Password> try_read_password();

    /**
    * \exception InvalidFileFormatException Nie ma zera na koncu
    */
    std::string read_string();

    /**
    * \exception InvalidFileFormatException Zostalo sie mniej niz trzeba bajtow w pliku
    */
    uint64_t read_int(size_t length_in_bytes);

    /**
    * \exception InvalidFileFormatException Zostalo sie mniej niz 4 bajty w pliku
    */
    uint64_t read_int64();

    /**
    * \exception InvalidFileFormatException Zostalo sie mniej niz 2 bajty w pliku
    */
    uint16_t read_int16();

    /**
    * \throws InvalidFileFormatException gdy plik sie skonczyl
    */
    uint8_t read_byte();

    /**
    * Odszyfrowuje wartosc, przyjmujac, ze nalezy ona do przedzialu [min, max).
    * Zaszyfrowana wartosc tez musi nalezyc do tego przedzialu.
    * 
    * \exception InvalidFileFormatException Wartosc jest poza przedzialem
    */
    int decrypt_value(int value, int min, int max);
};
