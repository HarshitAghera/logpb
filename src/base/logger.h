// Copyright (C) 2023  Harshit Aghera
// See end of file for extended copyright information.

#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <cstdio>

#include <google/protobuf/io/zero_copy_stream_impl.h>

using google::protobuf::io::FileOutputStream;

class Logger {
public:
    virtual ~Logger() = default;

    int set_headers(const std::vector<std::string_view>& h);
    std::string& get_elem_ref(const size_t col);
    int add_elem(const size_t col, std::string&& data);
    int add_elem(const size_t col, const std::string& data);
    int write_entry();
    int create_entry();

private:
    virtual int log_entry(const std::vector<std::string>& field) = 0;
    virtual int write_headers(const std::vector<std::string_view>& h) = 0;

    size_t number_of_columns;

    std::vector<std::string> entry;
};

class Console_Logger : public Logger {
public:
    void set_seperator(const std::string_view sep);

private:
    int log_entry(const std::vector<std::string>& field) override;
    int write_headers(const std::vector<std::string_view>& h) override;

    std::string seperator{"\t\t"};
    std::vector<std::string_view> headers;
};

class CSV_Logger : public Logger {
public:
    CSV_Logger(const std::string_view file, const std::string_view del,
               const std::string_view lb);
    ~CSV_Logger();

private:
    int log_entry(const std::vector<std::string>& field) override;
    int write_headers(const std::vector<std::string_view>& h) override;

    template <typename T>
    void write_lines(const T& entry);

private:
    std::string delimeter;
    std::string line_break;
    int buffer_size;

    std::string file_path;
    std::FILE* file_ptr;
    int file_descriptor;

    FileOutputStream file;

    struct File_Buffer {
        char* buffer;
        int size;
    };

    File_Buffer fbuf;

    std::string headers;
    std::vector<std::string> entries;
};

struct CSV_Logger_Info {
    std::string msg_name;
    std::string file_path;
    std::string del;
    std::string lb;
};

// PROJECT PROJECT-OBJECTIVE.
// Copyright (C) 2023  Harshit Aghera
//
// This file is part of PROJECT.
//
// PROJECT is free software : you can redistribute it and / or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// PROJECT is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with PROJECT. If not, see <https://www.gnu.org/licenses/>.
