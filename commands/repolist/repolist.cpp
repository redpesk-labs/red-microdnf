/*
Copyright (C) 2019-2020 Red Hat, Inc.

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

#include "repolist.hpp"

#include "../../context.hpp"

#include <libdnf/conf/option_string.hpp>
#include "libdnf-cli/output/repolist.hpp"

namespace microdnf {

void CmdRepolist::set_argument_parser(Context & ctx) {
    enable_disable_option = dynamic_cast<libdnf::OptionEnum<std::string> *>(
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::OptionEnum<std::string>>(
            new libdnf::OptionEnum<std::string>("enabled", {"all", "enabled", "disabled"}))));

    auto all = ctx.arg_parser.add_new_named_arg("all");
    all->set_long_name("all");
    all->set_short_description("show all repos");
    all->set_const_value("all");
    all->link_value(enable_disable_option);

    auto enabled = ctx.arg_parser.add_new_named_arg("enabled");
    enabled->set_long_name("enabled");
    enabled->set_short_description("show enabled repos (default)");
    enabled->set_const_value("enabled");
    enabled->link_value(enable_disable_option);

    auto disabled = ctx.arg_parser.add_new_named_arg("disabled");
    disabled->set_long_name("disabled");
    disabled->set_short_description("show disabled repos");
    disabled->set_const_value("disabled");
    disabled->link_value(enable_disable_option);

    patterns_to_show_options = ctx.arg_parser.add_new_values();
    auto repos = ctx.arg_parser.add_new_positional_arg(
        "repos_to_show",
        ArgumentParser::PositionalArg::UNLIMITED,
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::Option>(new libdnf::OptionString(nullptr))),
        patterns_to_show_options);
    repos->set_short_description("List of repos to show");

    auto conflict_args =
        ctx.arg_parser.add_conflict_args_group(std::unique_ptr<std::vector<ArgumentParser::Argument *>>(
            new std::vector<ArgumentParser::Argument *>{all, enabled, disabled}));

    all->set_conflict_arguments(conflict_args);
    enabled->set_conflict_arguments(conflict_args);
    disabled->set_conflict_arguments(conflict_args);

    auto repolist = ctx.arg_parser.add_new_command("repolist");
    repolist->set_short_description("display the configured software repositories");
    repolist->set_description("");
    repolist->named_args_help_header = "Optional arguments:";
    repolist->positional_args_help_header = "Positional arguments:";
    repolist->parse_hook = [this, &ctx](
                               [[maybe_unused]] ArgumentParser::Argument * arg,
                               [[maybe_unused]] const char * option,
                               [[maybe_unused]] int argc,
                               [[maybe_unused]] const char * const argv[]) {
        ctx.select_command(this);
        return true;
    };

    repolist->add_named_arg(all);
    repolist->add_named_arg(enabled);
    repolist->add_named_arg(disabled);
    repolist->add_positional_arg(repos);

    ctx.arg_parser.get_root_command()->add_command(repolist);

    auto repoinfo = ctx.arg_parser.add_new_command("repoinfo");
    repoinfo->set_short_description("display the configured software repositories");
    repoinfo->set_description("");
    repoinfo->named_args_help_header = "Optional arguments:";
    repoinfo->positional_args_help_header = "Positional arguments:";
    repoinfo->parse_hook = [this, &ctx](
                               [[maybe_unused]] ArgumentParser::Argument * arg,
                               [[maybe_unused]] const char * option,
                               [[maybe_unused]] int argc,
                               [[maybe_unused]] const char * const argv[]) {
        // TODO(jrohel): implement repoinfo
        throw std::logic_error("Not implemented");
        ctx.select_command(this);
        return true;
    };

    repoinfo->add_named_arg(all);
    repoinfo->add_named_arg(enabled);
    repoinfo->add_named_arg(disabled);
    repoinfo->add_positional_arg(repos);

    ctx.arg_parser.get_root_command()->add_command(repoinfo);
}

void CmdRepolist::run(Context & ctx) {
    std::vector<std::string> patterns_to_show;
    if (patterns_to_show_options->size() > 0) {
        patterns_to_show.reserve(patterns_to_show_options->size());
        for (auto & pattern : *patterns_to_show_options) {
            patterns_to_show.emplace_back(dynamic_cast<libdnf::OptionString *>(pattern.get())->get_value());
        }
    }

    auto query = ctx.base.get_rpm_repo_sack().new_query();
    if (enable_disable_option->get_value() == "enabled") {
        query.ifilter_enabled(true);
    } else if (enable_disable_option->get_value() == "disabled") {
        query.ifilter_enabled(false);
    }

    if (patterns_to_show.size() > 0) {
        auto query_names = query;
        query.ifilter_id(libdnf::sack::QueryCmp::IGLOB, patterns_to_show);
        query |= query_names.ifilter_name(libdnf::sack::QueryCmp::IGLOB, patterns_to_show);
    }

    bool with_status = enable_disable_option->get_value() == "all";

    libdnf::cli::output::print_repolist_table(
            query,
            with_status,
            libdnf::cli::output::COL_REPO_ID);
}

}  // namespace microdnf
