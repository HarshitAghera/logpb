// See end of file for extended copyright information.

#include "msg_type_gen.h"

#include "msg_defs/nanopb.pb.h"

#include <fstream>

#include <fmt/core.h>
// #define FMT_HEADER_ONLY
#include <fmt/format.h>

#include "logger.h"
#include "plotter.h"

namespace logpb {
// bool generate_rand_msgs(std::string file_name) {
//     std::fstream file(file_name, std::ios::out | std::ios::binary |
//     std::ios::trunc);

//     logpb::Data data;

//     for (int i{}; i < 100; ++i) {
//         logpb::Pressure* pres = data.add_pressure();
//         logpb::Force* force = data.add_force();

//         pres->set_pressure(i);
//         pres->set_temperature(i);

//         force->set_force(i);
//         force->add_temperature(i);
//         force->add_temperature(2*i);
//     }

//     data.SerializeToOstream(&file);

//     return true;
// }

Message_Def_Gen::Message_Def_Gen(const std::vector<std::string>& files)
    : Message_Def_Gen() {
    for (auto& filename : files) {
        import_def_file(filename);
    }

    refresh_registery();
}

Message_Def_Gen::Message_Def_Gen() : importer{&dst, &error_collector} {
    // mapping app directory to viratual directory
    dst.MapPath("", "");
    dst.MapPath("/", "/");
    dst.MapPath("C:/", "C:/");

    dst.MapPath("nanopb.proto", "build-dir/bin/resources/nanopb.proto");
    dst.MapPath("google/protobuf/descriptor.proto",
                "build-dir/bin/resources/descriptor.proto");

    dst.MapPath("nanopb.proto",
                "build-logpb-Desktop_Qt_6_3_2_MSVC2019_64bit-Debug/bin/"
                "resources/nanopb.proto");
    dst.MapPath("google/protobuf/descriptor.proto",
                "build-logpb-Desktop_Qt_6_3_2_MSVC2019_64bit-Debug/bin/"
                "resources/descriptor.proto");

    // dst.MapPath("nanopb.proto", "resources/nanopb.proto");
    // dst.MapPath("google/protobuf/descriptor.proto",
    //             "resources/descriptor.proto");
}

File_Error_Collector Message_Def_Gen::import_def_file(
    const std::string& filename) {
    File_Error_Collector errors;

    error_collector.set_file_collector(&errors);
    const FileDescriptor* proto = importer.Import(filename);

    if (proto) {
        const std::string file_pkg_name = proto->package();

        auto compare_pkgs = [&](const Package& p) {
            return p.name == file_pkg_name;
        };

        auto file_pkg = find_if(pkgs.begin(), pkgs.end(), compare_pkgs);

        if (file_pkg == pkgs.end()) {
            pkgs.emplace_back(
                Package{.name = file_pkg_name, .msgs = {}, .enums = {}});

            file_pkg = std::prev(pkgs.end());

            // packages.insert({file_pkg_name, file_pkg.base()});
            packages.insert({file_pkg_name, &(*file_pkg)});
        }

        for (int i{}; i < proto->message_type_count(); ++i) {
            add_msg(file_pkg->msgs, *(proto->message_type(i)));
        }

        for (int i{}; i < proto->enum_type_count(); ++i) {
            add_enum(file_pkg->enums, *(proto->enum_type(i)));
        }
    } else {
        // TODO : use ASSERTS or exception
        // fmt::print("Content of file : {0}\n", dst.GetLastErrorMessage());
    }

    return errors;
}

int Message_Def_Gen::refresh_registery() {
    // TODO-Harshit : make this function work with multiple message defs loads.

    // packages = {};
    // msgs = {};
    // enums = {};
    // fields = {};
    // msg_index = {};

    for (auto& pkg : pkgs) {
        create_enum_reg(pkg.enums);
        create_msg_reg(pkg.msgs);
    }

    return 0;
}

int Message_Def_Gen::create_enum_reg(const std::vector<Enum>& vec_enums) {
    for (auto& e : vec_enums) {
        enums.insert({e.full_name, &e});
    }

    return 0;
}

int Message_Def_Gen::create_field_reg(std::vector<Field>& vec_fields) {
    for (auto& field : vec_fields) {
        switch (field.type) {
            case FieldDescriptor::CppType::CPPTYPE_MESSAGE: {
                auto const msg_ident = msgs.find(field.type_name);

                if (msg_ident) {
                    field.msg_def = *msg_ident;
                } else {
                    // TODO : use ASSERTS
                    fmt::print("{} not found {}\n", field.full_name,
                               field.type_name);
                }
                break;
            }

            case FieldDescriptor::CppType::CPPTYPE_ENUM: {
                auto const enum_ident = enums.find(field.type_name);

                if (enum_ident) {
                    field.enum_def = *enum_ident;
                } else {
                    // TODO : use ASSERTS
                    fmt::print("{} not found {}\n", field.full_name,
                               field.type_name);
                }
            }

            default: {
            }
        }

        fields.insert({field.full_name, &field});
    }

    return 0;
}

int Message_Def_Gen::create_msg_reg(std::vector<Msg>& vec_msgs) {
    for (auto& msg : vec_msgs) {
        msgs.insert({msg.full_name, &msg});
        msg_index.insert({msg.id, &msg});

        create_msg_reg(msg.msgs);
        create_enum_reg(msg.enums);
        create_field_reg(msg.fields);
    }

    return 0;
}

int Message_Def_Gen::add_msg(std::vector<Msg>& data,
                             const Descriptor& descriptor) {
    data.emplace_back(
        Msg{.id = descriptor.options().GetExtension(nanopb_msgopt).msgid(),
            .name = descriptor.name(),
            .full_name = descriptor.full_name(),
            .descriptor = &descriptor});

    Msg& msg = data.back();

    for (int i{}; i < descriptor.enum_type_count(); ++i) {
        add_enum(msg.enums, *(descriptor.enum_type(i)));
    }

    for (int i{}; i < descriptor.nested_type_count(); ++i) {
        add_msg(msg.msgs, *(descriptor.nested_type(i)));
    }

    for (int i{}; i < descriptor.field_count(); ++i) {
        add_field(msg.fields, *(descriptor.field(i)));
    }

    return 0;
}

int Message_Def_Gen::add_enum(std::vector<Enum>& data,
                              const EnumDescriptor& descriptor) {
    data.emplace_back(Enum{.name = descriptor.name(),
                           .full_name = descriptor.full_name(),
                           .values{},
                           .descriptor = &descriptor});

    for (int i{}; i < descriptor.value_count(); ++i) {
        data.back().values.emplace_back(
            Enum_Value{.name = descriptor.value(i)->name(),
                       .value = descriptor.value(i)->number()});
    }

    return 0;
}

int Message_Def_Gen::add_field(std::vector<Field>& vec_fields,
                               const FieldDescriptor& descriptor) {
    std::string type_ident;

    switch (descriptor.cpp_type()) {
        case FieldDescriptor::CppType::CPPTYPE_MESSAGE: {
            type_ident = descriptor.message_type()->full_name();
            break;
        }

        case FieldDescriptor::CppType::CPPTYPE_ENUM: {
            type_ident = descriptor.enum_type()->full_name();
            break;
        }

        default: {
        }
    }

    vec_fields.emplace_back(Field{.name = descriptor.name(),
                                  .full_name = descriptor.full_name(),
                                  .type = descriptor.cpp_type(),
                                  .label = descriptor.label(),
                                  .descriptor = &descriptor,
                                  .type_name = type_ident,
                                  .msg_def = {},
                                  .enum_def = {}});

    return 0;
}

void Message_Def_Gen::print_msg(const Msg& msg, int level) {
    std::string indent;
    for (int i{}; i < level; ++i) {
        indent.push_back('\t');
    }

    auto i_print = [&](std::string_view fstr, auto&&... args) {
        fmt::print("{}{}", indent, fmt::format(fmt::runtime(fstr), args...));
    };

    i_print("{0} :\n", msg.name);

    i_print("\t{0} :\n", "Fields");
    for (auto& f : msg.fields) {
        i_print("\t\t{0:} : {1:}, {2:}\n", f.name, static_cast<int>(f.type),
                static_cast<int>(f.label));
    }
    fmt::print("\n");

    i_print("\t{0} :\n", "Enums");
    for (auto& e : msg.enums) {
        print_enum(e, level + 2);
    }
    fmt::print("\n");

    i_print("\t{0} :\n", "Nested Messages");
    for (auto& m : msg.msgs) {
        print_msg(m, level + 2);
    }
}

void Message_Def_Gen::print_enum(const Enum& elem, int level) {
    std::string indent;
    for (int i{}; i < level; ++i) {
        indent.push_back('\t');
    }

    auto i_print = [&](std::string_view fstr, auto&&... args) {
        fmt::print("{}{}", indent, fmt::format(fmt::runtime(fstr), args...));
    };

    i_print("{}\n", elem.name);

    for (auto& v : elem.values) {
        i_print("\t{0:} : {1}\n", v.name, v.value);
    }
}

void Message_Def_Gen::print_data() {
    fmt::print("Parsed Objects :\n");

    for (auto& pkg : pkgs) {
        fmt::print("Package : {}\n", pkg.name);

        for (auto& m : pkg.msgs) {
            print_msg(m, 0);
        }
        for (auto& e : pkg.enums) {
            print_enum(e, 0);
        }

        fmt::print("\n");
    }

    fmt::print("\n\n");
}

void Message_Def_Gen::print_summary() {
    fmt::print("Content summary of the parsed files.\n");

    auto print_s_cont = [](auto& s_cont) -> void {
        for (auto& e : s_cont) {
            fmt::print("{}, ", e.first);

            if (e.second) {
                fmt::print("{}\n", e.second->name);
            } else {
                // TODO : use ASSERT
                fmt::print("\n\nBIGGG ERROR\n");
            }
        }
    };

    fmt::print("\nPackages:\n");
    print_s_cont(packages);

    fmt::print("\nMessages:\n");
    print_s_cont(msgs);

    fmt::print("\nEnums:\n");
    print_s_cont(enums);

    fmt::print("\nFields:\n");
    print_s_cont(fields);

    fmt::print("\n\n");
}

std::vector<std::string> Message_Def_Gen::get_package_list() const {
    return packages.get_key_list();
}

std::vector<std::string> Message_Def_Gen::get_message_list(
    const std::vector<std::string>& package_names) const {
    std::vector<std::string> message_list;

    for (auto& pname : package_names) {
        auto& package = **(packages.find(pname));
        for (auto& m : package.msgs) {
            if (package_names.size() > 1) {
                message_list.push_back(m.full_name);
            } else {
                message_list.push_back(m.name);
            }
        }
    }

    return message_list;
}

std::vector<std::string> Message_Def_Gen::get_field_list(
    const std::string& package_name,
    const std::vector<std::string>& message_names) const {
    std::vector<std::string> field_list;

    for (auto& message_name : message_names) {
        const auto& msg = **(msgs.find(package_name + "." + message_name));

        for (const auto& field : msg.fields) {
            if (message_names.size() > 1) {
                field_list.push_back(msg.name + field.name);
            } else {
                field_list.push_back(field.name);
            }
        }
    }

    return field_list;
}

std::vector<double> Message_Def_Gen::get_numuric(const std::string& name,
                                                 const std::string& f_name,
                                                 const Message& message) {
    std::vector<double> data;

    const FieldDescriptor* fd = message.GetDescriptor()->FindFieldByName(name);
    const Reflection* reflection = message.GetReflection();

    const int size = reflection->FieldSize(message, fd);
    data.reserve(size);

    for (int i{}; i < size; ++i) {
        const auto& nm = reflection->GetRepeatedMessage(message, fd, i);
        const Reflection* nm_ref = nm.GetReflection();
        const FieldDescriptor* nm_fd =
            nm.GetDescriptor()->FindFieldByName(f_name);

        switch (nm_fd->cpp_type()) {
            case nm_fd->CppType::CPPTYPE_INT32: {
                data.push_back(nm_ref->GetInt32(nm, nm_fd));

                break;
            }

            default: {
            }
        }
    }

    return data;
}

int Message_Def_Gen::register_logger(const std::string& msg_name,
                                     Logger* logger) const {
    Msg* msg = *msgs.find(std::string{msg_name});

    if (!msg || !logger) {
        return -1;
    }

    msg->loggers.push_back(logger);

    std::vector<std::string_view> headers;
    headers.reserve(msg->fields.size());

    for (auto& field : msg->fields) {
        headers.push_back(field.name);
    }

    logger->set_headers(headers);

    return 0;
}

int Message_Def_Gen::register_plotter(const std::string& field_name,
                                      Plotter* plotter) const {
    const Field* field = *fields.find(field_name);

    if (!field || !plotter) {
        return -1;
    }

    field->plotters.push_back(plotter);

    return 0;
}

int Message_Def_Gen::parse_message_impl(const Message& msg,
                                        const Msg& msg_info) const {
    const Reflection* msg_reflection = msg.GetReflection();

    for (auto logger : msg_info.loggers) {
        logger->create_entry();
    }

    const size_t field_count = msg_info.fields.size();
    for (size_t i{}; i < field_count; ++i) {
        const Field& field = msg_info.fields[i];
        const FieldDescriptor* fd = field.descriptor;
        std::string value;

        switch (msg_info.fields[i].label) {
            case FieldDescriptor::Label::LABEL_REPEATED: {
                const size_t nos = msg_reflection->FieldSize(msg, fd);

                if (nos > 0) {
                    value.append("\"");
                }

                for (size_t j{}; j < nos; ++j) {
                    switch (msg_info.fields[i].type) {
                        case FieldDescriptor::CppType::CPPTYPE_MESSAGE: {
                            const Message& nested_msg =
                                msg_reflection->GetRepeatedMessage(msg, fd, j);
                            const Msg* msg = msg_info.fields[i].msg_def;

                            if (msg) {
                                parse_message_impl(nested_msg, *msg);
                            }

                            break;
                        }

                        case FieldDescriptor::CppType::CPPTYPE_INT32: {
                            int32_t val =
                                msg_reflection->GetRepeatedInt32(msg, fd, j);

                            for (auto plotter : field.plotters) {
                                plotter->append(static_cast<float>(val));
                            }

                            value += fmt::format("{},", val);

                            break;
                        }

                        default: {
                        }
                    }
                }

                if (nos > 0) {
                    value.pop_back();
                    value.append("\"");
                }

                break;
            }

            case FieldDescriptor::Label::LABEL_OPTIONAL:
            case FieldDescriptor::Label::LABEL_REQUIRED: {
                if (msg_reflection->HasField(msg, fd) ||
                    fd->has_default_value()) {
                    switch (msg_info.fields[i].type) {
                        case FieldDescriptor::CppType::CPPTYPE_INT32: {
                            int32_t val = msg_reflection->GetInt32(msg, fd);

                            for (auto plotter : field.plotters) {
                                plotter->append(static_cast<float>(val));
                            }

                            value = fmt::format("{}", val);

                            break;
                        }

                        case FieldDescriptor::CppType::CPPTYPE_ENUM: {
                            value = msg_reflection->GetEnum(msg, fd)->name();

                            break;
                        }

                        default: {
                        }
                    }
                } else {
                    // fmt::print("Field {} not set.\n",
                    // msg_info.fields[i].name);
                }

                break;
            }
        }

        for (auto logger : msg_info.loggers) {
            logger->add_elem(i, value);
        }
    }

    for (auto logger : msg_info.loggers) {
        logger->write_entry();
    }

    return 0;
}

Message* Message_Def_Gen::parse_message(const std::string& name,
                                        std::istream& in) {
    const Msg& msg_info = **msgs.find(name);
    const Descriptor* msg_descriptor = msg_info.descriptor;

    Message* msg{msg_factory.GetPrototype(msg_descriptor)->New()};

    if (msg->ParsePartialFromIstream(&in)) {
        parse_message_impl(*msg, msg_info);
        return msg;
    } else {
        delete msg;
        return nullptr;
    }
}

Message* Message_Def_Gen::parse_message(const uint32_t msgid,
                                        io::CodedInputStream& cis) const {
    const Msg& msg_info = **msg_index.find(msgid);
    const Descriptor* msg_descriptor = msg_info.descriptor;

    Message* msg{msg_factory.GetPrototype(msg_descriptor)->New()};

    if (msg->ParseFromCodedStream(&cis)) {
        parse_message_impl(*msg, msg_info);
        return msg;
    } else {
        delete msg;
        return nullptr;
    }
}
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
