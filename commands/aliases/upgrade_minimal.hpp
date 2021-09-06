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


#ifndef MICRODNF_COMMANDS_ALIASES_UPGRADE_MINIMAL_HPP
#define MICRODNF_COMMANDS_ALIASES_UPGRADE_MINIMAL_HPP


#include "../upgrade/upgrade.hpp"


namespace microdnf {


class UpgradeMinimalAlias : public UpgradeCommand {
public:
    explicit UpgradeMinimalAlias(Command & parent) : UpgradeCommand(parent, "upgrade-minimal") {
        auto & cmd = *get_argument_parser_command();
        cmd.set_short_description("Alias to `upgrade --minimal`");

        // set the default value of the --minimal option to `true`
        auto minimal = dynamic_cast<libdnf::OptionBool *>(this->minimal);
        minimal->set(libdnf::Option::Priority::DEFAULT, true);
    }
};


}  // namespace microdnf


#endif  // MICRODNF_COMMANDS_ALIASES_UPGRADE_MINIMAL_HPP
