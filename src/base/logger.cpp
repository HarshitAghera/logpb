// Copyright (C) 2023  Harshit Aghera
// See end of file for extended copyright information.

#include "logger.h"

//#include <fmt/core.h>
#include <fmt/ranges.h>

#include "uio.h"

int Logger::add_elem(const size_t col, const std::string_view data) {
    entry[col] = data;

    return 0;
}

int Logger::write_entry() {
    log_entry(entry);

    return 0;
}

int Logger::create_entry() {
    for (auto& e : entry) {
        e = {};
    }

    return 0;
}

int Logger::set_headers(const std::vector<std::string_view>& h) {
    number_of_columns = h.size();

    entry.resize(number_of_columns);

    return write_headers(h);
}

int Console_Logger::write_headers(const std::vector<std::string_view>& h) {
    headers = h;

    fmt::print("{}\n", fmt::join(headers, seperator));

    return 0;
}

void Console_Logger::set_seperator(const std::string_view sep) {
    seperator = sep;
}

int Console_Logger::log_entry(const std::vector<std::string>& entry) {
    fmt::print("{}\n", fmt::join(entry, seperator));

    return 0;
}

CSV_Logger::CSV_Logger(const std::string_view file, const std::string_view del, const std::string_view lb)
    : file_path(file)
    , delimeter(del)
    , line_break(lb)
    , buffer_size(25)
    , file_descriptor(win_io::open(file_path.c_str(), O_WRONLY | O_TEXT))
{


}

CSV_Logger::~CSV_Logger() {
    win_io::close(file_descriptor);
}

int CSV_Logger::write_headers(const std::vector<std::string_view>& h) {
    std::string headers = fmt::format("{}{}", fmt::join(headers, delimeter), line_break);
    int error = write(file_descriptor, headers.c_str(), headers.size());
    return error;
}

int CSV_Logger::log_entry(const std::vector<std::string>& entry) {
    std::string line = fmt::format("{}{}", fmt::join(entry, delimeter), line_break);
    int error = write(file_descriptor, line.c_str(), line.size());
    return error;
}

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
