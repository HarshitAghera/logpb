// Copyright (C) 2023  Harshit Aghera
// See end of file for extended copyright information.

#pragma once

#include <string>
#include <string_view>
#include <vector>

class Logger {
public:
    virtual ~Logger() = default;

    int set_headers(const std::vector<std::string_view>& h);
    int add_elem(const size_t col, const std::string_view data);
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
    CSV_Logger(const std::string_view file, const std::string_view del, const std::string_view lb);
    ~CSV_Logger();

private:
    int log_entry(const std::vector<std::string>& field) override;
    int write_headers(const std::vector<std::string_view>& h) override;

private:
    std::string delimeter;
    std::string line_break;

    std::string headers;
    std::vector<std::string> entries;
    std::string file_path;
    int file_descriptor;

    const int buffer_size;
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
