/*
Copyright Contributors to the libdnf project.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/


#include "search.hpp"

#include "../../context.hpp"

#include <libdnf/conf/option_string.hpp>


namespace fs = std::filesystem;


namespace microdnf {


using namespace libdnf::cli;


SearchCommand::SearchCommand(Command & parent) : Command(parent, "search") {
    auto & ctx = static_cast<Context &>(get_session());
    auto & parser = ctx.get_argument_parser();
    auto & cmd = *get_argument_parser_command();

    cmd.set_short_description("Search for software matching all specified strings");
    cmd.set_parse_hook_func([this, &ctx](
                                     [[maybe_unused]] ArgumentParser::Argument * arg,
                                     [[maybe_unused]] const char * option,
                                     [[maybe_unused]] int argc,
                                     [[maybe_unused]] const char * const argv[]) {
        ctx.set_selected_command(this);
        return true;
    });

    patterns = parser.add_new_values();
    auto patterns_arg = parser.add_new_positional_arg(
        "patterns",
        ArgumentParser::PositionalArg::UNLIMITED,
        parser.add_init_value(std::unique_ptr<libdnf::Option>(new libdnf::OptionString(nullptr))),
        patterns);
    patterns_arg->set_short_description("Patterns");
    cmd.register_positional_arg(patterns_arg);
}


void SearchCommand::run() {
    // auto & ctx = static_cast<Context &>(get_session());
    // auto & package_sack = *ctx.base.get_rpm_package_sack();

    // TODO(dmach): implement
}


}  // namespace microdnf
