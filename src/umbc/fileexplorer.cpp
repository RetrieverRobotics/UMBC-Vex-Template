/**
 * @file umbc/fileexplorer.cpp
 *
 * Contains the implementation of the FileExplorer class.
 */

// standard libraries
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <map>
#include <string>
#include <vector>

// local header files
#include "api.h"
#include "umbc.h"

// namespaces used
using namespace pros;
using namespace umbc;
using namespace std;


std::string umbc::FileExplorer::normalize_path(const std::string& path) {

    std::string normalized_path = path;

    for (char& character : normalized_path) {
        if ('\\' == character) {
            character = '/';
        }
    }

    while (1 < normalized_path.size() && '/' == normalized_path.back()) {
        normalized_path.pop_back();
    }

    return normalized_path;
}


std::string umbc::FileExplorer::join_path(const std::string& directory_path, const std::string& name) {

    if (directory_path.empty() || "/" == directory_path) {
        return directory_path + name;
    }

    return directory_path + "/" + name;
}


bool umbc::FileExplorer::is_directory_name(const std::string& name) {

    return !name.empty()
        && "." != name
        && ".." != name
        && std::string::npos == name.find('/')
        && std::string::npos == name.find('\\')
        && std::string::npos == name.find('\0');
}


umbc::FileExplorer::FileExplorer(const std::string& root_dir) {

    this->root_dir = normalize_path(root_dir);
    this->current_dir = this->root_dir;

    std::int32_t error = this->load_files(this->current_dir, this->files);

    if (0 != error) {
        ERROR("failed to initialize FileExplorer at " + this->root_dir + " (errno " + std::to_string(error) + ")");
        return;
    }

    this->current_dir_valid = true;
}


std::string umbc::FileExplorer::get_root_directory_path() {

    return this->root_dir;
}


std::string umbc::FileExplorer::get_root_directory() {

    std::size_t separator = this->root_dir.find_last_of('/');

    if (std::string::npos == separator) {
        return this->root_dir;
    }

    if (separator + 1 == this->root_dir.size()) {
        return this->root_dir;
    }

    return this->root_dir.substr(separator + 1);
}


std::string umbc::FileExplorer::get_current_directory_path() {

    return this->current_dir;
}


std::string umbc::FileExplorer::get_current_directory() {

    std::size_t separator = this->current_dir.find_last_of('/');

    if (std::string::npos == separator) {
        return this->current_dir;
    }

    if (separator + 1 == this->current_dir.size()) {
        return this->current_dir;
    }

    return this->current_dir.substr(separator + 1);
}


bool umbc::FileExplorer::is_file(const std::string& file) {

    return this->current_dir_valid && this->files.end() != this->files.find(file);
}


bool umbc::FileExplorer::is_directory(const std::string& directory) {

    if (false == this->current_dir_valid || false == is_directory_name(directory)) {
        return false;
    }

    std::map<std::string, std::string> directory_files;
    return 0 == this->load_files(join_path(this->current_dir, directory), directory_files);
}


std::list<string> umbc::FileExplorer::get_files() {

    std::list<std::string> directory_files;

    if (false == this->current_dir_valid) {
        return directory_files;
    }

    for (const auto& file : this->files) {
        directory_files.push_back(file.first);
    }

    return directory_files;
}


umbc::FileExplorer::Result umbc::FileExplorer::go_to_directory(const std::string& directory) {

    if (false == this->current_dir_valid || false == is_directory_name(directory)) {
        return umbc::FileExplorer::Result::ERROR_INVALID_DIR;
    }

    std::string next_directory = join_path(this->current_dir, directory);
    std::map<std::string, std::string> next_files;

    std::int32_t error = this->load_files(next_directory, next_files);

    if (0 != error) {
        return (ENOENT == error || EINVAL == error)
            ? umbc::FileExplorer::Result::ERROR_INVALID_DIR
            : umbc::FileExplorer::Result::ERROR;
    }

    this->current_dir = next_directory;
    this->files.swap(next_files);
    this->current_dir_valid = true;

    return umbc::FileExplorer::Result::OK;
}


umbc::FileExplorer::Result umbc::FileExplorer::go_to_root() {

    std::map<std::string, std::string> root_files;

    std::int32_t error = this->load_files(this->root_dir, root_files);

    if (0 != error) {
        return (ENOENT == error || EINVAL == error)
            ? umbc::FileExplorer::Result::ERROR_INVALID_DIR
            : umbc::FileExplorer::Result::ERROR;
    }

    this->current_dir = this->root_dir;
    this->files.swap(root_files);
    this->current_dir_valid = true;

    return umbc::FileExplorer::Result::OK;
}


umbc::FileExplorer::Result umbc::FileExplorer::go_to_parent() {

    if (this->current_dir == this->root_dir) {
        return this->go_to_root();
    }

    std::size_t separator = this->current_dir.find_last_of('/');
    std::string parent_directory = (std::string::npos == separator)
        ? this->root_dir
        : (0 == separator ? "/" : this->current_dir.substr(0, separator));

    if (parent_directory.size() < this->root_dir.size()
        || this->root_dir != parent_directory.substr(0, this->root_dir.size())
        || (this->root_dir != "/" && parent_directory.size() > this->root_dir.size()
            && '/' != parent_directory[this->root_dir.size()])) {
        parent_directory = this->root_dir;
    }

    std::map<std::string, std::string> parent_files;

    std::int32_t error = this->load_files(parent_directory, parent_files);

    if (0 != error) {
        return (ENOENT == error || EINVAL == error)
            ? umbc::FileExplorer::Result::ERROR_INVALID_DIR
            : umbc::FileExplorer::Result::ERROR;
    }

    this->current_dir = parent_directory;
    this->files.swap(parent_files);
    this->current_dir_valid = true;

    return umbc::FileExplorer::Result::OK;
}


std::int32_t umbc::FileExplorer::load_files(
    const std::string& directory_path,
    std::map<std::string, std::string>& directory_files
) {

    if (directory_path.empty() || std::string::npos != directory_path.find('\0')) {
        errno = EINVAL;
        ERROR("invalid directory path");
        return EINVAL;
    }

    std::size_t buffer_size = this->initial_buffer_size;
    std::vector<char> buffer;

    while (this->maximum_buffer_size >= buffer_size) {

        buffer.assign(buffer_size, '\x7f');
        errno = 0;

        if (1 != pros::usd::list_files(directory_path.c_str(), buffer.data(), static_cast<std::int32_t>(buffer.size()))) {
            if (ENOBUFS == errno && buffer_size < this->maximum_buffer_size) {
                buffer_size *= 2;
                continue;
            }

            std::int32_t error = (0 == errno) ? EIO : errno;
            errno = error;
            ERROR("failed to list files in " + directory_path + " (errno " + std::to_string(error) + ")");
            return error;
        }

        char* terminator = static_cast<char*>(std::memchr(buffer.data(), '\0', buffer.size()));

        if (nullptr == terminator) {
            if (buffer_size == this->maximum_buffer_size) {
                errno = ENOBUFS;
                ERROR("file list exceeded the buffer limit for " + directory_path);
                return ENOBUFS;
            }

            buffer_size *= 2;
            continue;
        }

        std::map<std::string, std::string> loaded_files;
        const char* entry = buffer.data();
        const char* end = terminator;

        while (entry < end) {
            const char* newline = static_cast<const char*>(std::memchr(entry, '\n', end - entry));
            const char* entry_end = (nullptr == newline) ? end : newline;

            if (entry < entry_end && '\r' == entry_end[-1]) {
                --entry_end;
            }

            if (entry < entry_end) {
                std::string file_name(entry, entry_end);
                loaded_files[file_name] = join_path(directory_path, file_name);
            }

            entry = (nullptr == newline) ? end : newline + 1;
        }

        directory_files.swap(loaded_files);
        return 0;
    }

    errno = ENOBUFS;
    ERROR("file list exceeded the buffer limit for " + directory_path);
    return ENOBUFS;
}
