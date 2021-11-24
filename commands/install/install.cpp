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


#include "install.hpp"

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


namespace fs = std::filesystem;


namespace microdnf {


using namespace libdnf::cli;


InstallCommand::InstallCommand(Command & parent) : Command(parent, "install") {
    auto & ctx = static_cast<Context &>(get_session());
    auto & parser = ctx.get_argument_parser();

    auto & cmd = *get_argument_parser_command();
    cmd.set_short_description("Install software");

    patterns_to_install_options = parser.add_new_values();
    auto keys = parser.add_new_positional_arg(
        "keys_to_match",
        ArgumentParser::PositionalArg::UNLIMITED,
        parser.add_init_value(std::unique_ptr<libdnf::Option>(new libdnf::OptionString(nullptr))),
        patterns_to_install_options);
    keys->set_short_description("List of keys to match");
    keys->set_complete_hook_func([&ctx](const char * arg){return match_available_pkgs(ctx, arg);});
    cmd.register_positional_arg(keys);
}


void InstallCommand::run() {
    auto & ctx = static_cast<Context &>(get_session());
    auto & package_sack = *ctx.base.get_rpm_package_sack();

    // To search in the system repository (installed packages)
    // Creates system repository in the repo_sack and loads it into rpm::PackageSack.
    package_sack.create_system_repo(false);

	//iotbzh: install rednode
    ctx.rednode.install(package_sack);

    // To search in available repositories (available packages)
    libdnf::repo::RepoQuery enabled_repos(ctx.base);
    enabled_repos.filter_enabled(true);
    ctx.load_rpm_repos(enabled_repos);

    std::cout << std::endl;

    libdnf::Goal goal(ctx.base);
    for (auto & pattern : *patterns_to_install_options) {
        auto option = dynamic_cast<libdnf::OptionString *>(pattern.get());
        goal.add_rpm_install(option->get_value());
    }
    auto transaction = redlib::RedTransaction(goal.resolve(false));

    //iotbzh: check transaction pkgs
    ctx.rednode.checkTransactionPkgs(transaction);

    if (transaction.get_problems() != libdnf::GoalProblem::NO_PROBLEM) {
        std::cout << transaction.all_package_solver_problems_to_string() << std::endl;
        return;
    }

    if (!libdnf::cli::output::print_transaction_table(transaction)) {
        return;
    }

    if (!userconfirm(ctx.base.get_config())) {
        std::cout << "Operation aborted." << std::endl;
        return;
    }

    ctx.download_and_run(transaction);
}


}  // namespace microdnf
