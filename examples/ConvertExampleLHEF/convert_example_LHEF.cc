// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2026 The HepMC collaboration (see AUTHORS for details)
//
/// @example convert_example_LHEF.cc
/// @brief Utility to convert between different types of LHE event records. So far only LHEF is supported.
///
#include "HepMC3/LHEF.h"

#include <cstdint>
#include <memory>
#include <string>
#include "cmdline.h"
using namespace LHEF;
enum class formats : std::uint8_t { lhef, none };

template <class T>
std::shared_ptr<T> get_input_file(const char* name, const bool input_is_stdin) {
    const std::string n(name);
    return (input_is_stdin?std::make_shared<T>(std::cin):std::make_shared<T>(n));
}
template <class T>
std::shared_ptr<T> get_output_file(const char* name) {
    const std::string n(name);
    return std::make_shared<T>(n);
}

int main(int argc, char** argv)
{
    gengetopt_args_info ai{};
    if (cmdline_parser (argc, argv, &ai) != 0) {
        exit(1);
    }
    auto validarguments = ( ai.inputs_num == 2 &&  std::string(ai.output_format_arg) !=  "none") ||
                ( ai.inputs_num == 1 && ( std::string(ai.output_format_arg) ==  "none"  ));

    if ( !validarguments )
    {
        printf("Exactly two arguments are requred: the name of input and output files if the output format in not \"none\" \n");
        printf("In case the output format is \"none\" exactly one argument should be given: the name of input file.\n");
        exit(1);
    }
    const std::map<std::string, formats> format_map = {
        { "lhef", formats::lhef },
        { "none", formats::none }
    };

    long int  events_parsed = 0;
    const long int  events_limit = ai.events_limit_arg;
    const long int  first_event_number = ai.first_event_number_arg;
    const long int  last_event_number = ai.last_event_number_arg;
    const long int  print_each_events_parsed = ai.print_every_events_parsed_arg;

    std::shared_ptr<Reader>      input_file;
    const bool input_is_stdin = (std::string(ai.inputs[0]) == std::string("-"));
    if (input_is_stdin) std::ios_base::sync_with_stdio(false);
#ifdef _LIBCPP_VERSION
    if (input_is_stdin) {
        printf("The program cannot process inputs from standard input as std::ios_base::sync_with_stdio(bool) is not implemented in libc++, please use another C++ standard library.\n");
        exit(4);
    }
#endif

    bool ignore_writer = false;
    switch (format_map.at(std::string(ai.input_format_arg)))
    {
    case formats::lhef:
        input_file = get_input_file<Reader>(ai.inputs[0], input_is_stdin);
        break;
    default:
        printf("Input format %s  is not known\n", ai.input_format_arg);
        exit(2);
        break;
    }
    std::shared_ptr<Writer> output_file;
    switch (format_map.at(std::string(ai.output_format_arg)))
    {
    case formats::lhef:
        output_file = get_output_file<Writer>(ai.inputs[1]);
        break;
    case formats::none:
        output_file = nullptr;
        ignore_writer = true;
        break;
    default:
        printf("Output format %s  is not known\n", ai.output_format_arg);
        exit(2);
        break;
    }
    if (output_file)
    {
        output_file->heprup = input_file->heprup;
        output_file->headerBlock(input_file->headerBlock);
        output_file->initComments(input_file->initComments);
        output_file->init();
    }

    while (true)
    {
        const bool res_read = input_file->readEvent();

        if (!res_read) {
            printf("End of file reached. Exit.\n");
            break;
        }
        if (input_file->currevent < first_event_number) continue;
        if (input_file->currevent > last_event_number) continue;

        if (!ignore_writer)
        {
            output_file->hepeup = input_file->hepeup;
            output_file->hepeup.heprup = &output_file->heprup;
            output_file->eventComments(input_file->eventComments);
            output_file->writeEvent();
        }
        ++events_parsed;
        if( events_parsed%print_each_events_parsed == 0 ) printf("Events parsed: %li\n", events_parsed);
        if( events_parsed >= events_limit ) {
            printf("Event limit reached:->events_parsed(%li) >= events_limit(%li)<-. Exit.\n", events_parsed, events_limit);
            break;
        }
    }

    cmdline_parser_free(&ai);
    return EXIT_SUCCESS;
}
