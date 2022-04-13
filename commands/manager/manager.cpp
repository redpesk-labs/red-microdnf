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
        auto &ctx = static_cast<Context &>(get_session());
        auto &parser = ctx.get_argument_parser();

        auto &cmd = *get_argument_parser_command();
        cmd.set_short_description("Manager for handle nodes");

        alias = dynamic_cast<libdnf::OptionString *>(
            parser.add_init_value(std::unique_ptr<libdnf::OptionString>(new libdnf::OptionString(""))));

        no_system_node = dynamic_cast<libdnf::OptionBool *>(
            parser.add_init_value(std::unique_ptr<libdnf::OptionBool>(new libdnf::OptionBool(false))));

        update = dynamic_cast<libdnf::OptionBool *>(
            parser.add_init_value(std::unique_ptr<libdnf::OptionBool>(new libdnf::OptionBool(false))));

        tmplate = dynamic_cast<libdnf::OptionString *>(
            parser.add_init_value(std::unique_ptr<libdnf::OptionString>(new libdnf::OptionString(""))));

        tmplateAdmin = dynamic_cast<libdnf::OptionString *>(
            parser.add_init_value(std::unique_ptr<libdnf::OptionString>(new libdnf::OptionString(""))));

        auto arg_alias = parser.add_new_named_arg("alias");
        arg_alias->set_has_value(true);
        arg_alias->set_long_name("alias");
        arg_alias->set_short_description("rednode alias name default is node dirname");
        arg_alias->link_value(alias);
        arg_alias->set_const_value("true");

        auto arg_update = parser.add_new_named_arg("update");
        arg_update->set_has_value(false);
        arg_update->set_long_name("update");
        arg_update->set_short_description("force creation even when node exist");
        arg_update->set_const_value("true");
        arg_update->link_value(update);

        auto arg_no_system_node = parser.add_new_named_arg("no-system-node");
        arg_no_system_node->set_has_value(false);
        arg_no_system_node->set_long_name("no-system-node");
        arg_no_system_node->set_short_description("Do not create system node: system configuration will be in first parent");
        arg_no_system_node->set_const_value("true");
        arg_no_system_node->link_value(no_system_node);

        auto arg_tmplate = parser.add_new_named_arg("template");
        arg_tmplate->set_has_value(true);
        arg_tmplate->set_long_name("template");
        arg_tmplate->set_short_description("Create node config from template [default= /etc/redpak/template.d/default.yaml]");
        arg_tmplate->link_value(tmplate);
        arg_tmplate->set_const_value("true");

        auto arg_tmplate_admin = parser.add_new_named_arg("template-admin");
        arg_tmplate_admin->set_has_value(true);
        arg_tmplate_admin->set_long_name("template-admin");
        arg_tmplate_admin->set_short_description("Create node admin config from template [default= /etc/redpak/template.d/admin.yaml]");
        arg_tmplate_admin->link_value(tmplateAdmin);
        arg_tmplate_admin->set_const_value("true");

        cmd.register_named_arg(arg_alias);
        cmd.register_named_arg(arg_update);
        cmd.register_named_arg(arg_no_system_node);
        cmd.register_named_arg(arg_tmplate);
        cmd.register_named_arg(arg_tmplate_admin);
    }

    void ManagerCommand::run()
    {
        auto &ctx = static_cast<Context &>(get_session());

        // check we redpath is defined and exist
        if (ctx.rednode.isRedpath(false))
            throw std::runtime_error("Syntax Error: redpak --redpath=/xx/../xyz subcommand (missing --redpath)");

        ctx.rednode.createRedNode(alias->get_value(), update->get_value(), tmplate->get_value(), tmplateAdmin->get_value(), no_system_node->get_value());

        // create system repo
        auto &package_sack = *ctx.base.get_rpm_package_sack();
        package_sack.create_system_repo(false);

        std::cout << "SUCCESS: rednode " << ctx.rednode.getRedpath() << " ready" << std::endl;
    }

} // namespace microdnf
