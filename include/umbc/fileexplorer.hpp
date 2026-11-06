/**
 * @file umbc/fileexplorer.hpp
 *
 * Contains the prototype for the File Explorer class. The File Explorer class is used for traversing through
 * a file system and providing files at the current directory level.
 */

#ifndef _FILE_EXPLORER_HPP_
#define _FILE_EXPLORER_HPP_

// standard libraries
#include <cstdint>
#include <string>
#include <map>
#include <list>

// local header files
#include "api.h"
#include "robot.hpp"

// namespaces used
using namespace pros;
using namespace std;


namespace umbc {

    /**
     * Provides a utility for listing files and navigating a file system starting from a specified root
     * directory. Child directories must be navigated by name because the PROS filesystem API cannot list them.
     */
    class FileExplorer {

        private:

            // initial size in bytes of the buffer used to list files
            static constexpr std::size_t initial_buffer_size = 512;

            // maximum size in bytes of the buffer used to list files
            static constexpr std::size_t maximum_buffer_size = 32768;

            // character the buffer used to list files is filled with to detect unwritten bytes
            static constexpr char buffer_fill_character = '\x7f';

            std::string root_dir = std::string();       // root directory of the FileExplorer's file system
            std::string current_dir = std::string();    // current directory the FileExplorer is in

            // map of the current directory's files and their absolute paths
            std::map<std::string, std::string> files = std::map<std::string, std::string>();

            bool current_dir_valid = false;             // whether files were loaded from the current directory

            /**
             * @brief Converts backslashes to forward slashes and removes trailing slashes from a path
             *
             * @param path: Path to normalize
             *
             * @return Normalized path
             */
            static std::string normalize_path(const std::string& path);

            /**
             * @brief Joins a directory path and a name into a single path
             *
             * @param directory_path: Path of the directory
             * @param name: Name of the file or directory within the directory
             *
             * @return Joined path
             */
            static std::string join_path(const std::string& directory_path, const std::string& name);

            /**
             * @brief Determines if a name is a valid name for a child directory
             *
             * @param name: Name being evaluated
             *
             * @return True if the name is not empty, is not . or .. and contains no path
             *      separators or null characters. Otherwise false
             */
            static bool is_directory_name(const std::string& name);

            /**
             * @brief Loads the files at a directory into a map.
             *
             * @param directory_path: Path of the directory to load
             * @param directory_files: Map to populate with file names and paths
             *
             * @return 0 on success. Otherwise the filesystem error number
             */
            std::int32_t load_files(
                const std::string& directory_path,
                std::map<std::string, std::string>& directory_files
            );


        public:

            /**
             * Error codes for the FileExplorer Class
             */
            enum class Result : std::int32_t {
                OK = 0,                 // success
                ERROR = 1,              // generic error
                ERROR_INVALID_DIR = 2,  // directory does not exist
                ERROR_INVALID_FILE = 3  // file does not exist
            };

            /**
             * @brief Creates a FileExplorer object
             * 
             * @param root_dir: Root directory of the file system being explored.
             *      FileExplorer will never go a directory level above the root
             *      directory.
             */
            FileExplorer(const std::string& root_dir);

            /**
             * @brief Gets the full path to the root directory.
             * 
             * @return Path of root directory
             */
            std::string get_root_directory_path();

            /**
             * @brief Gets the name of the root directory
             * 
             * @return Name of root directory
             */
            std::string get_root_directory();

            /**
             * @brief Gets the current directory's full path
             * 
             * @return Path of current directory
             */
            std::string get_current_directory_path();

            /**
             * @brief Gets the name of the current directory
             * 
             * @return Name of current directory
             */
            std::string get_current_directory();

            /**
             * @brief Determines if a file is at the current directory level and is a file
             * 
             * @param file: Name of file being evaluated
             * 
             * @return True if the file exists at the current directory level and is a file.
             *      Otherwise false
             */
            bool is_file(const std::string& file);

            /**
             * @brief Determines if a named child of the current directory is a directory
             * 
             * @param directory: Name of directory being evaluated
             * 
             * @return True if the directory is a child of the current directory and is a directory.
             *      Otherwise false
             */
            bool is_directory(const std::string& directory);

            /**
             * @brief Returns all the files at the current directory level
             * 
             * @return All files at the current directory level. If there are no files at the current
             *      directory level an empty list will be returned.
             */
            std::list<string> get_files();

            /**
             * @brief Sets the current directory to a named child directory
             * 
             * @param directory: Name of directory to set as the current directory
             * 
             * @return OK on success, ERROR_INVALID_DIR if the directory does not exist, or ERROR
             *      if the filesystem could not be read
             */
            umbc::FileExplorer::Result go_to_directory(const std::string& directory);

            /**
             * @brief Sets the current directory to the root directory
             * 
             * @return OK on success, ERROR_INVALID_DIR if the root directory does not exist, or ERROR
             *      if the filesystem could not be read
             */
            umbc::FileExplorer::Result go_to_root();

            /**
             * @brief Sets the current directory to the parent directory.
             * 
             * Sets the current directory to the parent directory in the context
             * of the current directory. If the current directory is the root
             * directory, the current directory will remain the root directory.
             * 
             * @return OK on success, ERROR_INVALID_DIR if the parent directory does not exist, or ERROR
             *      if the filesystem could not be read
             */
            umbc::FileExplorer::Result go_to_parent();
    };
}

#endif // _FILE_EXPLORER_HPP_
