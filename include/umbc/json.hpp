/**
 * @file umbc/json.hpp
 *
 * Contains the prototype for the Json class. The Json class is used for
 * parsing, modifying and serializing JSON documents while preserving
 * unrelated values.
 */

#ifndef _UMBC_JSON_HPP_
#define _UMBC_JSON_HPP_

// standard libraries
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

// namespaces used
using namespace std;


namespace umbc {

    /**
     * Represents a single JSON value. A value may be a null, boolean, number,
     * string, array or object. Object members retain their insertion order so
     * unrelated values are preserved when a document is parsed, modified and
     * serialized.
     */
    class Json {

        private:

            /**
             * JSON value types
             */
            enum class Type : std::int32_t {
                NIL = 0,        // JSON null value
                BOOLEAN = 1,    // JSON true or false value
                NUMBER = 2,     // JSON number value
                STRING = 3,     // JSON string value
                ARRAY = 4,      // JSON array value
                OBJECT = 5      // JSON object value
            };

            // number of spaces per indentation level when serializing
            static constexpr std::uint32_t serialize_indent_spaces = 4;

            Type type = Type::NIL;              // type of this JSON value
            bool boolean_value = false;         // value when type is BOOLEAN
            double number_value = 0.0;          // value when type is NUMBER
            std::string string_value;           // value when type is STRING
            std::vector<Json> array_values;     // values when type is ARRAY

            // members and their names in insertion order when type is OBJECT
            std::vector<std::pair<std::string, Json>> object_members;

            /**
             * @brief Advances a position past any JSON whitespace
             * 
             * @param text: Text being parsed
             * @param position: Position to advance
             */
            static void skip_whitespace(const std::string& text, std::size_t& position);

            /**
             * @brief Parses a JSON value starting at a position
             * 
             * @param text: Text being parsed
             * @param position: Position of the value. Advanced past the value on success
             * @param value: Json object to populate with the parsed value
             * 
             * @return True if the value was parsed successfully. Otherwise false
             */
            static bool parse_value(const std::string& text, std::size_t& position, Json& value);

            /**
             * @brief Parses a JSON string starting at a position
             * 
             * @param text: Text being parsed
             * @param position: Position of the opening quote. Advanced past the
             *      closing quote on success
             * @param parsed_string: String to populate with the decoded characters
             * 
             * @return True if the string was parsed successfully. Otherwise false
             */
            static bool parse_string(const std::string& text, std::size_t& position, std::string& parsed_string);

            /**
             * @brief Appends a unicode code point to a string as UTF-8
             * 
             * @param code_point: Unicode code point to append
             * @param utf8_string: String to append the encoded code point to
             */
            static void append_utf8(std::uint32_t code_point, std::string& utf8_string);

            /**
             * @brief Appends a string to the output as a quoted JSON string
             * 
             * @param unescaped_string: String to escape and quote
             * @param output: Output to append the quoted JSON string to
             */
            static void serialize_string(const std::string& unescaped_string, std::string& output);

            /**
             * @brief Appends this value to the output as JSON text
             * 
             * @param output: Output to append the JSON text to
             * @param indent_level: Current indentation level
             */
            void serialize_value(std::string& output, std::uint32_t indent_level) const;


        public:

            /**
             * @brief Creates a Json object representing a JSON null value
             */
            Json();

            /**
             * @brief Parses JSON text into a Json object
             * 
             * @param text: JSON text to parse
             * @param value: Json object to populate with the parsed document
             * 
             * @return True if the text was parsed successfully. Otherwise false
             *      and the value is reset to a JSON null value
             */
            static bool parse(const std::string& text, Json& value);

            /**
             * @brief Serializes this value to pretty-printed JSON text
             * 
             * @return JSON text representing this value
             */
            std::string serialize() const;

            /**
             * @brief Checks if this value is a JSON object
             * 
             * @return True if this value is a JSON object. Otherwise false
             */
            bool is_object() const;

            /**
             * @brief Checks if this value is a JSON string
             * 
             * @return True if this value is a JSON string. Otherwise false
             */
            bool is_string() const;

            /**
             * @brief Checks if an object member exists
             * 
             * @param name: Name of the member to check for
             * 
             * @return True if this value is an object and the member exists.
             *      Otherwise false
             */
            bool contains(const std::string& name) const;

            /**
             * @brief Retrieves an object member
             * 
             * @param name: Name of the member to retrieve
             * 
             * @return Pointer to the member if this value is an object and the
             *      member exists. Otherwise nullptr
             */
            const Json* get(const std::string& name) const;

            /**
             * @brief Retrieves an object member, creating it if it does not exist
             * 
             * If this value is not an object it becomes an empty object and any
             * previous value is discarded.
             * 
             * @param name: Name of the member to retrieve or create
             * 
             * @return Reference to the member
             */
            Json& insert(const std::string& name);

            /**
             * @brief Removes an object member if it exists
             * 
             * @param name: Name of the member to remove
             */
            void remove(const std::string& name);

            /**
             * @brief Retrieves this value as a string
             * 
             * @param default_value: Value to return if this value is not a string
             * 
             * @return String value if this value is a string. Otherwise the default value
             */
            std::string get_string(const std::string& default_value) const;

            /**
             * @brief Sets this value to a JSON string
             * 
             * @param value: String value to set
             */
            void set_string(const std::string& value);
    };
}

#endif // _UMBC_JSON_HPP_
