#pragma once

#include <vector>
#include <string>


class Logger {
    virtual void set_titles(const std::vector<std::string> titles) = 0;
    virtual void add_entry(const int index, const std::string& value) = 0;
    virtual void get_ready_for_entry();
    virtual void log_entry() = 0;
};