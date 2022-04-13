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

#ifndef MICRODNF_COMMANDS_MANAGER_MANAGER_HPP
#define MICRODNF_COMMANDS_MANAGER_MANAGER_HPP

#include <libdnf-cli/session.hpp>

#include <libdnf/conf/option_bool.hpp>

#include <memory>
#include <vector>

namespace microdnf
{
    class ManagerCommand : public libdnf::cli::session::Command
    {
    public:
        explicit ManagerCommand(Command &parent);
        void run() override;

    private:
        libdnf::OptionString *alias{nullptr};
        libdnf::OptionBool *update{nullptr};
        libdnf::OptionString *tmplate{nullptr};
        libdnf::OptionString *tmplateAdmin{nullptr};
        libdnf::OptionBool *no_system_node{nullptr};
    };

} // namespace microdnf

#endif
