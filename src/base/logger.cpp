// Copyright (C) 2023  Harshit Aghera
// See end of file for extended copyright information.

#include "logger.h"

// #include <fmt/core.h>
// #include <fmt/ranges.h>

#include <cstring>

#include <fcntl.h>
#ifdef _MSC_VER
#include <google/protobuf/io/io_win32.h>
#else
#define O_BINARY 0
#endif

#ifdef _WIN32
using google::protobuf::io::win32::open;
#endif

std::string& Logger::get_elem_ref(const size_t col) { return entry[col]; }

int Logger::add_elem(const size_t col, const std::string& data) {
    entry[col] = data;

    return 0;
}

int Logger::add_elem(const size_t col, std::string&& data) {
    entry[col] = std::move(data);

    return 0;
}

int Logger::write_entry() {
    log_entry(entry);

    return 0;
}

int Logger::create_entry() {
    for (auto& e : entry) {
        e.clear();
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

    // fmt::print("{}\n", fmt::join(headers, seperator));

    return 0;
}

void Console_Logger::set_seperator(const std::string_view sep) {
    seperator = sep;
}

int Console_Logger::log_entry(const std::vector<std::string>& entry) {
    // fmt::print("{}\n", fmt::join(entry, seperator));

    return 0;
}

CSV_Logger::CSV_Logger(const std::string_view fp, const std::string_view del,
                       const std::string_view lb)
    : delimeter(del),
      line_break(lb),
      buffer_size(4098),
      file_path(fp),
      file_ptr{},
      file_descriptor(
          open(file_path.c_str(), O_CREAT | O_WRONLY | O_BINARY, 0666)),
      file(file_descriptor, buffer_size) {
    file.SetCloseOnDelete(true);
    file.Next(reinterpret_cast<void**>(&fbuf.buffer), &fbuf.size);

    line_break.append("\n");
}

CSV_Logger::~CSV_Logger() { file.BackUp(fbuf.size); }

int CSV_Logger::write_headers(const std::vector<std::string_view>& h) {
    write_lines(h);

    return 0;
}

int CSV_Logger::log_entry(const std::vector<std::string>& entry) {
    write_lines(entry);

    return 0;
}

template <typename T>
void CSV_Logger::write_lines(const T& entry) {
    auto write_string = [&](const std::string_view elem) {
        const int elem_size = elem.size();

        while (elem_size > fbuf.size) {
            file.BackUp(fbuf.size);
            file.Next(reinterpret_cast<void**>(&fbuf.buffer), &fbuf.size);
        }

        fbuf.size -= elem_size;

        std::memcpy(fbuf.buffer, elem.data(), elem_size);
        fbuf.buffer += elem_size;
    };

    for (size_t i{}; i < entry.size(); ++i) {
        write_string(entry[i]);

        if (i != entry.size() - 1) {
            write_string(delimeter);
        } else {
            write_string(line_break);
        }
    }
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
