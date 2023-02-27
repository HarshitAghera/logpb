// Copyright (C) 2023  Harshit Aghera
// See end of file for extended copyright information.

#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <fstream>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>

using namespace google::protobuf;
using namespace google::protobuf::compiler;

// #include "msg_defs/msg_defs.pb.h"
#include "logger.h"
#include "hash_map.h"

namespace logpb {
bool generate_rand_msgs(std::string file);

class Parser_Error_Collector final : public MultiFileErrorCollector {
    void AddError(const std::string& filename, int line, int column,
                  const std::string& message) override;
    void AddWarning(const std::string& filename, int line, int column,
                    const std::string& message) override;

    const std::vector<std::string>& get_errors() { return errors; }
    const std::vector<std::string>& get_warnings() { return warnings; }

private:
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};

class Message_Def_Gen {
public:
    Message_Def_Gen(const std::vector<std::string>& files);
    Message_Def_Gen();

    int import_def_file(const std::string& filename);

    // Parse message fron istream and return and pointer to it. Ownership is
    // passed to caller. If logger is registed for the message type using
    // register_logger method, message will get logged.
    Message* parse_message(const std::string& name, std::istream& in);
    Message* parse_message(const uint32_t msgid,
                           io::CodedInputStream& cos) const;

    // Register a logger for a message type.
    int register_logger(const std::string_view msg_name, Logger* logger);

    void print_data();
    void print_summary();

    std::vector<double> get_numuric(const std::string& name,
                                    const std::string& f_name,
                                    const Message& message);

private:
    struct Enum;
    struct Msg;

    struct Field {
        std::string name;
        std::string full_name;
        FieldDescriptor::CppType type;
        FieldDescriptor::Label label;

        const FieldDescriptor* descriptor;

        std::string type_name;
        const Msg* msg_def;
        const Enum* enum_def;
    };

    struct Enum_Value {
        std::string name;
        int value;
    };

    struct Enum {
        std::string name;
        std::string full_name;
        std::vector<Enum_Value> values;

        const EnumDescriptor* descriptor;
    };

    struct Msg {
        uint32_t id;
        std::string name;
        std::string full_name;
        std::vector<Field> fields;
        std::vector<Enum> enums;
        std::vector<Msg> msgs;

        const Descriptor* descriptor;

        std::vector<Logger*> loggers;
    };

    struct Package {
        std::string name;
        std::vector<Msg> msgs;
        std::vector<Enum> enums;
    };

    static int add_enum(std::vector<Enum>& enums,
                        const EnumDescriptor& descriptor);
    static int add_field(std::vector<Field>& fields, const FieldDescriptor& fd);
    static int add_msg(std::vector<Msg>& msgs, const Descriptor& descriptor);

    int create_msg_reg(std::vector<Msg>& vec_msgs);
    int create_enum_reg(const std::vector<Enum>& vec_enums);
    int create_field_reg(std::vector<Field>& vec_fields);

    void print_msg(const Msg& msg, int level);
    void print_enum(const Enum& elem, int level);

    int parse_message_impl(const Message& msg, const Msg& msg_info) const;

    const DescriptorPool& pool() { return *importer.pool(); };

    std::vector<Package> pkgs;

    S_Container<std::string, const Package* const> packages;
    S_Container<std::string, Msg* const> msgs;
    S_Container<uint32_t, const Msg* const> msg_index;
    S_Container<std::string, const Field* const> fields;
    S_Container<std::string, const Enum* const> enums;

    DiskSourceTree dst;
    Parser_Error_Collector error_collector;
    Importer importer;
    mutable DynamicMessageFactory msg_factory;
};

}  // namespace logpb

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
