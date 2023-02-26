// Copyright (C) 2023  Harshit Aghera
// See end of file for extended copyright information.

#include "logger.h"

#include <fmt/core.h>
#include <fmt/ranges.h>

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