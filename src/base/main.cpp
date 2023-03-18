// Copyright (C) 2023  Harshit Aghera
// See end of file for extended copyright information.

#include <iostream>
#include "msg_type_gen.h"
#include "stream_parser.h"
#include "stream_generator.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <stdio.h>

void read_stream_from_file(const std::string_view filename,
                           const logpb::Message_Def_Gen& msg_defs) {
    std::FILE* fd = fopen(filename.data(), "rb");

    logpb::Stream_Parser parser =
        logpb::Stream_Parser::create_from_file(fileno(fd), &msg_defs);

    parser.parse();

    fclose(fd);
}

void write_stream_to_file(const std::string_view filename) {
    std::FILE* fd = fopen(filename.data(), "wb");

    logpb::Stream_Generator generator{
        logpb::Stream_Generator::create_from_file(fileno(fd))};

    generator.gen_msgs();

    //    fflush(fd);
    //    fclose(fd);
    //    close(fileno(fd));
}

int main(int argc, char* argv[]) {
    // std::string output_file{argv[3]};
    // logpb::generate_rand_msgs(output_file);
    // fmt::print("Output is written to {0}\n", argv[1]);

    logpb::Message_Def_Gen cmplr({std::string{argv[1]}});

    // cmplr.print_data();
    // cmplr.print_summary();

    std::unique_ptr<Logger> force_console_logger{new Console_Logger{}};
    cmplr.register_logger("logpb.Force", force_console_logger.get());
    // std::unique_ptr<Logger> pres_console_logger{new Console_Logger{}};
    // cmplr.register_logger("logpb.Pressure", pres_console_logger.get());

    CSV_Logger force_csv_logger{"out/force.csv", ",", "\n"};
    cmplr.register_logger("logpb.Force", &force_csv_logger);

    std::string bin_file{argv[2]};

    // write_stream_to_file(bin_file);
    read_stream_from_file(bin_file, cmplr);

    // std::fstream in(std::string{argv[3]}, std::ios::in | std::ios::binary);
    // const auto msg = cmplr.parse_message(argv[2], in);

    // auto pressure = cmplr.get_numuric("pressure", "pressure", *msg);
    // auto force = cmplr.get_numuric("force", "force", *msg);

    // fmt::print("Pressure: {}\nForce: {}\n\n", fmt::join(pressure, ", "),
    // fmt::join(force, ", "));

    // delete msg;
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
