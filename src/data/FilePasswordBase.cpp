#include "data/FilePasswordBase.h"

#include <fstream>
#include <filesystem>
#include "data/Password.h"
#include "util/utils.h"
#include "util/EncryptedReader.h"
#include "util/EncryptedWriter.h"

#include <ctime>

FilePasswordBase::FilePasswordBase(
    std::filesystem::path file, std::string password, PasswordBase& underlying_base)
    : file(file),
      password(password),
      underlying_base(underlying_base),
      last_opening_time(time(0)) {}

FilePasswordBase FilePasswordBase::create_new(
    std::filesystem::path file, std::string password, PasswordBase& underlying_base)
{
    FilePasswordBase base(file, password, underlying_base);
    base.save();

    return base;
}

FilePasswordBase FilePasswordBase::load_from_file(
    std::filesystem::path file, std::string password, PasswordBase& underlying_base)
{
    FilePasswordBase base(file, password, underlying_base);

    std::ifstream stream(file);
    EncryptedReader reader(stream, password);
    reader.read_to_base(base.underlying_base);
    base.last_opening_time = reader.get_read_last_opening_time();

    stream.close();

    // Zapisujemy, zeby zaznaczyc ze baza zostala otwarta
    base.save();

    return base;
}

time_t FilePasswordBase::get_last_opening_time() const
{
    return last_opening_time;
}

void FilePasswordBase::save() const
{
    std::ofstream stream(file);
    EncryptedWriter writer(stream, password);
    writer.write_base(underlying_base);
}

PasswordId FilePasswordBase::add_password(Password password)
{
    PasswordId id = underlying_base.add_password(password);
    save();
    return id;
}

CategoryId FilePasswordBase::add_category(std::string name)
{
    CategoryId id = underlying_base.add_category(name);
    save();
    return id;
}

void FilePasswordBase::insert_category(CategoryId id, std::string name)
{
    underlying_base.insert_category(id, name);
    save();
}

void FilePasswordBase::remove_password(PasswordId id)
{
    underlying_base.remove_password(id);
    save();
}

void FilePasswordBase::remove_category(CategoryId id)
{
    underlying_base.remove_category(id);
    save();
}

Password FilePasswordBase::get_password(PasswordId id) const
{
    return underlying_base.get_password(id);
}

std::string FilePasswordBase::get_category(CategoryId id) const
{
    return underlying_base.get_category(id);
}

CategoryId FilePasswordBase::get_category_id(std::string name) const
{
    return underlying_base.get_category_id(name);
}

std::vector<PasswordRecord> FilePasswordBase::fetch_passwords(
    const std::vector<SearchParameter>& search_parameters,
    const std::vector<PasswordProperty>& sorting_parameters) const
{
    return underlying_base.fetch_passwords(
        search_parameters, sorting_parameters);
}

std::vector<CategoryRecord> FilePasswordBase::fetch_all_categories() const
{
    return underlying_base.fetch_all_categories();
}
