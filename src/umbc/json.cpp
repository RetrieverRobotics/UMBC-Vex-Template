/**
 * @file umbc/json.cpp
 *
 * Contains the implementation of the Json class.
 */

// standard libraries
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

// local header files
#include "umbc.h"

// namespaces used
using namespace umbc;
using namespace std;


void umbc::Json::skip_whitespace(const std::string& text, std::size_t& position) {

    while (position < text.size()
        && (' ' == text[position] || '\t' == text[position]
            || '\n' == text[position] || '\r' == text[position])) {
        position++;
    }
}


void umbc::Json::append_utf8(std::uint32_t code_point, std::string& utf8_string) {

    // encode as a single byte
    if (0x80 > code_point) {
        utf8_string += (char)(code_point);
    }

    // encode as two bytes
    else if (0x800 > code_point) {
        utf8_string += (char)(0xC0 | (code_point >> 6));
        utf8_string += (char)(0x80 | (code_point & 0x3F));
    }

    // encode as three bytes
    else if (0x10000 > code_point) {
        utf8_string += (char)(0xE0 | (code_point >> 12));
        utf8_string += (char)(0x80 | ((code_point >> 6) & 0x3F));
        utf8_string += (char)(0x80 | (code_point & 0x3F));
    }

    // encode as four bytes
    else {
        utf8_string += (char)(0xF0 | (code_point >> 18));
        utf8_string += (char)(0x80 | ((code_point >> 12) & 0x3F));
        utf8_string += (char)(0x80 | ((code_point >> 6) & 0x3F));
        utf8_string += (char)(0x80 | (code_point & 0x3F));
    }
}


bool umbc::Json::parse_string(const std::string& text, std::size_t& position, std::string& parsed_string) {

    parsed_string.clear();

    // check for the opening quote
    if (position >= text.size() || '"' != text[position]) {
        return false;
    }
    position++;

    // decode each character until the closing quote
    while (position < text.size() && '"' != text[position]) {

        char character = text[position];

        // decode an escape sequence
        if ('\\' == character) {

            position++;
            if (position >= text.size()) {
                return false;
            }

            char escape = text[position];
            position++;

            switch (escape) {
                case '"': parsed_string += '"'; break;
                case '\\': parsed_string += '\\'; break;
                case '/': parsed_string += '/'; break;
                case 'b': parsed_string += '\b'; break;
                case 'f': parsed_string += '\f'; break;
                case 'n': parsed_string += '\n'; break;
                case 'r': parsed_string += '\r'; break;
                case 't': parsed_string += '\t'; break;

                // decode a unicode escape sequence
                case 'u': {

                    // check that all four hex digits are present
                    if (position + 4 > text.size()) {
                        return false;
                    }

                    // convert the hex digits into a code point
                    std::uint32_t code_point = (std::uint32_t)std::strtoul(
                        text.substr(position, 4).c_str(), nullptr, 16);
                    position += 4;

                    // decode a surrogate pair if present
                    if (0xD800 <= code_point && 0xDBFF >= code_point
                        && position + 6 <= text.size()
                        && '\\' == text[position] && 'u' == text[position + 1]) {

                        std::uint32_t low_surrogate = (std::uint32_t)std::strtoul(
                            text.substr(position + 2, 4).c_str(), nullptr, 16);

                        if (0xDC00 <= low_surrogate && 0xDFFF >= low_surrogate) {
                            code_point = 0x10000
                                + ((code_point - 0xD800) << 10)
                                + (low_surrogate - 0xDC00);
                            position += 6;
                        }
                    }

                    append_utf8(code_point, parsed_string);
                    break;
                }

                // invalid escape sequence
                default: {
                    return false;
                }
            }
        }

        // copy a regular character
        else {
            parsed_string += character;
            position++;
        }
    }

    // check for the closing quote
    if (position >= text.size()) {
        return false;
    }
    position++;

    return true;
}


bool umbc::Json::parse_value(const std::string& text, std::size_t& position, Json& value) {

    skip_whitespace(text, position);

    // check that a value is present
    if (position >= text.size()) {
        return false;
    }

    char character = text[position];

    // parse an object
    if ('{' == character) {

        value = Json();
        value.type = Type::OBJECT;
        position++;
        skip_whitespace(text, position);

        // check for an empty object
        if (position < text.size() && '}' == text[position]) {
            position++;
            return true;
        }

        // parse each member of the object
        while (true) {

            // parse the member name
            skip_whitespace(text, position);
            std::string member_name;
            if (false == parse_string(text, position, member_name)) {
                return false;
            }

            // check for the name and value separator
            skip_whitespace(text, position);
            if (position >= text.size() || ':' != text[position]) {
                return false;
            }
            position++;

            // parse the member value
            Json member_value;
            if (false == parse_value(text, position, member_value)) {
                return false;
            }
            value.object_members.push_back(make_pair(member_name, member_value));

            // check for the end of the object or another member
            skip_whitespace(text, position);
            if (position >= text.size()) {
                return false;
            }
            else if ('}' == text[position]) {
                position++;
                return true;
            }
            else if (',' != text[position]) {
                return false;
            }
            position++;
        }
    }

    // parse an array
    else if ('[' == character) {

        value = Json();
        value.type = Type::ARRAY;
        position++;
        skip_whitespace(text, position);

        // check for an empty array
        if (position < text.size() && ']' == text[position]) {
            position++;
            return true;
        }

        // parse each element of the array
        while (true) {

            // parse the element value
            Json element_value;
            if (false == parse_value(text, position, element_value)) {
                return false;
            }
            value.array_values.push_back(element_value);

            // check for the end of the array or another element
            skip_whitespace(text, position);
            if (position >= text.size()) {
                return false;
            }
            else if (']' == text[position]) {
                position++;
                return true;
            }
            else if (',' != text[position]) {
                return false;
            }
            position++;
        }
    }

    // parse a string
    else if ('"' == character) {

        value = Json();
        value.type = Type::STRING;
        return parse_string(text, position, value.string_value);
    }

    // parse a true literal
    else if (0 == text.compare(position, 4, "true")) {

        value = Json();
        value.type = Type::BOOLEAN;
        value.boolean_value = true;
        position += 4;
        return true;
    }

    // parse a false literal
    else if (0 == text.compare(position, 5, "false")) {

        value = Json();
        value.type = Type::BOOLEAN;
        value.boolean_value = false;
        position += 5;
        return true;
    }

    // parse a null literal
    else if (0 == text.compare(position, 4, "null")) {

        value = Json();
        position += 4;
        return true;
    }

    // parse a number
    else if ('-' == character || ('0' <= character && '9' >= character)) {

        const char* number_start = text.c_str() + position;
        char* number_end = nullptr;
        double number = std::strtod(number_start, &number_end);

        // check that a number was parsed
        if (number_end == number_start) {
            return false;
        }

        value = Json();
        value.type = Type::NUMBER;
        value.number_value = number;
        position += (std::size_t)(number_end - number_start);
        return true;
    }

    // invalid value
    return false;
}


void umbc::Json::serialize_string(const std::string& unescaped_string, std::string& output) {

    output += '"';

    // escape each character as needed
    for (char character : unescaped_string) {

        switch (character) {
            case '"': output += "\\\""; break;
            case '\\': output += "\\\\"; break;
            case '\b': output += "\\b"; break;
            case '\f': output += "\\f"; break;
            case '\n': output += "\\n"; break;
            case '\r': output += "\\r"; break;
            case '\t': output += "\\t"; break;

            default: {

                // escape remaining control characters
                if (0x20 > (unsigned char)character) {
                    char escape_buffer[8];
                    std::snprintf(escape_buffer, sizeof(escape_buffer), "\\u%04X", character);
                    output += escape_buffer;
                }

                // copy a regular character
                else {
                    output += character;
                }
                break;
            }
        }
    }

    output += '"';
}


void umbc::Json::serialize_value(std::string& output, std::uint32_t indent_level) const {

    std::string indent = std::string((std::size_t)(indent_level * serialize_indent_spaces), ' ');
    std::string member_indent = std::string((std::size_t)((indent_level + 1) * serialize_indent_spaces), ' ');

    switch (this->type) {

        // serialize a null value
        case Type::NIL: {
            output += "null";
            break;
        }

        // serialize a boolean value
        case Type::BOOLEAN: {
            output += this->boolean_value ? "true" : "false";
            break;
        }

        // serialize a number value
        case Type::NUMBER: {

            char number_buffer[32];

            // serialize whole numbers without a decimal point
            if (this->number_value == std::floor(this->number_value)
                && 1e15 > std::fabs(this->number_value)) {
                std::snprintf(number_buffer, sizeof(number_buffer), "%lld",
                    (long long)this->number_value);
            }

            // serialize remaining numbers with full precision
            else {
                std::snprintf(number_buffer, sizeof(number_buffer), "%.17g", this->number_value);
            }

            output += number_buffer;
            break;
        }

        // serialize a string value
        case Type::STRING: {
            serialize_string(this->string_value, output);
            break;
        }

        // serialize an array value
        case Type::ARRAY: {

            // serialize an empty array on a single line
            if (this->array_values.empty()) {
                output += "[]";
                break;
            }

            // serialize each element on its own line
            output += "[\n";
            for (std::size_t i = 0; i < this->array_values.size(); i++) {
                output += member_indent;
                this->array_values[i].serialize_value(output, indent_level + 1);
                if (i + 1 < this->array_values.size()) {
                    output += ',';
                }
                output += '\n';
            }
            output += indent + "]";
            break;
        }

        // serialize an object value
        case Type::OBJECT: {

            // serialize an empty object on a single line
            if (this->object_members.empty()) {
                output += "{}";
                break;
            }

            // serialize each member on its own line
            output += "{\n";
            for (std::size_t i = 0; i < this->object_members.size(); i++) {
                output += member_indent;
                serialize_string(this->object_members[i].first, output);
                output += ": ";
                this->object_members[i].second.serialize_value(output, indent_level + 1);
                if (i + 1 < this->object_members.size()) {
                    output += ',';
                }
                output += '\n';
            }
            output += indent + "}";
            break;
        }

        // invalid value type
        default: {
            ERROR("cannot serialize value; invalid JSON type");
            output += "null";
            break;
        }
    }
}


umbc::Json::Json() {
    this->type = Type::NIL;
}


bool umbc::Json::parse(const std::string& text, Json& value) {

    std::size_t position = 0;

    // parse the document's root value
    if (false == parse_value(text, position, value)) {
        value = Json();
        return false;
    }

    // check that only whitespace remains after the root value
    skip_whitespace(text, position);
    if (position != text.size()) {
        value = Json();
        return false;
    }

    return true;
}


std::string umbc::Json::serialize() const {

    std::string output;
    this->serialize_value(output, 0);
    output += '\n';

    return output;
}


bool umbc::Json::is_object() const {
    return Type::OBJECT == this->type;
}


bool umbc::Json::is_string() const {
    return Type::STRING == this->type;
}


bool umbc::Json::is_number() const {
    return Type::NUMBER == this->type;
}


bool umbc::Json::contains(const std::string& name) const {
    return nullptr != this->get(name);
}


const umbc::Json* umbc::Json::get(const std::string& name) const {

    // only objects have members
    if (Type::OBJECT != this->type) {
        return nullptr;
    }

    // search the members for the given name
    for (const std::pair<std::string, Json>& member : this->object_members) {
        if (member.first == name) {
            return &member.second;
        }
    }

    return nullptr;
}


umbc::Json& umbc::Json::insert(const std::string& name) {

    // become an empty object if not already an object
    if (Type::OBJECT != this->type) {
        *this = Json();
        this->type = Type::OBJECT;
    }

    // search the members for the given name
    for (std::pair<std::string, Json>& member : this->object_members) {
        if (member.first == name) {
            return member.second;
        }
    }

    // create the member since it does not exist
    this->object_members.push_back(make_pair(name, Json()));
    return this->object_members.back().second;
}


void umbc::Json::remove(const std::string& name) {

    // only objects have members
    if (Type::OBJECT != this->type) {
        return;
    }

    // search the members for the given name and remove it
    for (std::size_t i = 0; i < this->object_members.size(); i++) {
        if (this->object_members[i].first == name) {
            this->object_members.erase(this->object_members.begin() + i);
            return;
        }
    }
}


std::string umbc::Json::get_string(const std::string& default_value) const {
    return (Type::STRING == this->type) ? this->string_value : default_value;
}


void umbc::Json::set_string(const std::string& value) {

    *this = Json();
    this->type = Type::STRING;
    this->string_value = value;
}


double umbc::Json::get_number(double default_value) const {
    return (Type::NUMBER == this->type) ? this->number_value : default_value;
}


void umbc::Json::set_number(double value) {

    *this = Json();
    this->type = Type::NUMBER;
    this->number_value = value;
}
