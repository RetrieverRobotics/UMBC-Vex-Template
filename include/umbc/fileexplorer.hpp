/**
 * @file umbc/fileexplorer.hpp
 *
 * Contains the prototype for the File Explorer class. The File Explorer Class is used for traversing through
 * a file system and providing files and directories at the current directory level.
 */

#ifndef _FILE_EXPLORER_HPP_
#define _FILE_EXPLORER_HPP_

#include "api.h"
#include "robot.hpp"

#include <cstdint>
#include <string>
#include <map>
#include <list>

using namespace pros;
using namespace std;

namespace umbc {

    /**
     * Provides a utility for navigating and managing a file system starting from a specified root directory.
     * It maintains the current directory state and offers methods to inspect files and directories, as well
     * as navigate through the file structure.
     */
    class FileExplorer {

        private:

            std::string root_dir = std::string();       // root directory of the FileExplorer's file system
            std::string current_dir = std::string();    // current directory the FileExplorer is in

            // map of the current directory's children directories and their absolute paths
            std::map<std::string, std::string> directories = std::map<std::string, std::string>();

            // map of the current directory's files and their absolute paths
            std::map<std::string, std::string> files = std::map<std::string, std::string>();


        public:

            /**
             * Error codes for the FileExplorer Class
             */
            enum class Result {
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
             * @brief Determines if a directory is a child of the current directory and is a directory
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
             * @brief Returns all the children directories of the current directory
             * 
             * @return All children directories of the current directory. If there are no child directories
             *      an empty list will be returned.
             */
            std::list<string> get_directories();

            /**
             * @brief Sets the current directory to the root directory
             * 
             * @param directory: Name of directory to set as the current directory
             * 
             * @return OK on success. ERROR_INVALID_DIR if root directory does not exist
             */
            umbc::FileExplorer::Result go_to_directory(const std::string& directory);

            /**
             * @brief Sets the current directory to the root directory
             * 
             * @return OK on success. ERROR_INVALID_DIR if root directory does not exist
             */
            umbc::FileExplorer::Result go_to_root();

            /**
             * @brief Sets the current directory to the parent directory.
             * 
             * Sets the current directory to the parent directory in the context
             * of the current directory. If the current directory is the root
             * directory, the current directory will remain the root directory.
             * 
             * @return OK on sucess. ERROR_INVALID_DIR if parent directory does not exist
             */
            umbc::FileExplorer::Result go_to_parent();
    };
}

#endif // _FILE_EXPLORER_HPP_
