/*
Copyright (C) 2020 Red Hat, Inc.

This file is part of microdnf: https://github.com/rpm-software-management/libdnf/

Microdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Microdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with microdnf.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "manager.hpp"

#include "../../context.hpp"

#include "libdnf-cli/output/transaction_table.hpp"

#include <libdnf/base/goal.hpp>
#include <libdnf/conf/option_string.hpp>
#include <libdnf/rpm/package.hpp>
#include <libdnf/rpm/package_query.hpp>
#include <libdnf/rpm/package_set.hpp>
#include <libdnf/rpm/transaction.hpp>

#include <filesystem>
#include <iostream>

#include "redconfig.hpp"

namespace microdnf
{

    using namespace libdnf::cli;

    ManagerCommand::ManagerCommand(Command &parent) : Command(parent, "manager")
    {
    auto & ctx = static_cast<Context &>(get_session());
    auto & parser = ctx.get_argument_parser();

    auto & cmd = *get_argument_parser_command();
    cmd.set_short_description("Manager for handle nodes");

    alias = dynamic_cast<libdnf::OptionString *>(
        parser.add_init_value(std::unique_ptr<libdnf::OptionString>(new libdnf::OptionString(""))));

    update = dynamic_cast<libdnf::OptionBool *>(
        parser.add_init_value(std::unique_ptr<libdnf::OptionBool>(new libdnf::OptionBool(false))));

    create = dynamic_cast<libdnf::OptionBool *>(
        parser.add_init_value(std::unique_ptr<libdnf::OptionBool>(new libdnf::OptionBool(false))));

    tmplate = dynamic_cast<libdnf::OptionString *>(
        parser.add_init_value(std::unique_ptr<libdnf::OptionString>(new libdnf::OptionString("default"))));

    auto arg_alias = parser.add_new_named_arg("alias");
    arg_alias->set_has_value(true);
    arg_alias->set_long_name("alias");
    arg_alias->set_short_description("rednode alias name [mandatory when creating");
    arg_alias->link_value(alias);

    auto arg_update = parser.add_new_named_arg("update");
    arg_update->set_has_value(false);
    arg_update->set_long_name("update");
    arg_update->set_short_description("force creation even when node exist");
    arg_update->set_const_value("true");
    arg_update->link_value(update);

    auto arg_create = parser.add_new_named_arg("create");
    arg_create->set_has_value(false);
    arg_create->set_long_name("create");
    arg_create->set_short_description("Create an empty node [require --alias]");
    arg_create->set_const_value("true");
    arg_create->link_value(create);

    auto arg_tmplate = parser.add_new_named_arg("template");
    arg_tmplate->set_has_value(true);
    arg_tmplate->set_long_name("template");
    arg_tmplate->set_short_description("Create node config from temmplate [default= /etc/redpak/template.d/default.yaml]");
    arg_tmplate->link_value(tmplate);

    }

    void ManagerCommand::run()
    {
        auto & ctx = static_cast<Context &>(get_session());

        bool nodeconfig = false;

        if (update->get_value())
        {
            create->set(libdnf::Option::Priority::RUNTIME, true);
        }

        if (create->get_value())
        {
            nodeconfig = true;
            if (alias->get_value().empty())
                throw std::runtime_error("--create require --alias=xxxx [no default]");

            if (tmplate->get_value().empty())
                create->set(libdnf::Option::Priority::RUNTIME, "default");
        }

        // check we redpath is defined and exist
        if (ctx.rednode.isRedpath(false))
            throw std::runtime_error("Syntax Error: redpak --redpath=/xx/../xyz subcommand (missing --redpath)");

        if (create->get_value())
        {
            ctx.rednode.createRedNode(alias->get_value(), create->get_value(), update->get_value(), tmplate->get_value(), "admin");
        }

        //create system repo
        auto &package_sack = *ctx.base.get_rpm_package_sack();
        package_sack.create_system_repo(false);

        std::cout << "SUCCESS: rednode ready" << std::endl;
    }

} // namespace microdnf
