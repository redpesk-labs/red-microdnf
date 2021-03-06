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


#ifndef MICRODNF_COMMANDS_REPO_REPO_INFO_HPP
#define MICRODNF_COMMANDS_REPO_REPO_INFO_HPP


#include "arguments.hpp"
#include "repo_list.hpp"

#include <libdnf-cli/session.hpp>

#include <libdnf/conf/option_enum.hpp>

#include <memory>
#include <vector>


namespace microdnf {


class RepoInfoCommand : public RepoListCommand {
public:
    explicit RepoInfoCommand(Command & parent);
    void print(const libdnf::repo::RepoQuery & query, [[maybe_unused]] bool with_status) override;

protected:
    // to be used by an alias command only
    explicit RepoInfoCommand(Command & parent, const std::string & name);
};


}  // namespace microdnf


#endif  // MICRODNF_COMMANDS_REPO_REPO_INFO_HPP
