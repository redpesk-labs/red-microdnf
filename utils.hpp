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

#ifndef MICRODNF_UTILS_HPP
#define MICRODNF_UTILS_HPP

#include <filesystem>
#include <string>

namespace microdnf {

/// Returns "true" if program runs with effective user ID = 0
bool am_i_root() noexcept;

namespace xdg {

/// Returns user home directory
std::filesystem::path get_user_home_dir();

/// Returns user cache directory
std::filesystem::path get_user_cache_dir();

/// Returns user configuration directory
std::filesystem::path get_user_config_dir();

/// Returns user data directory
std::filesystem::path get_user_data_dir();

/// Returns user runtime directory
std::filesystem::path get_user_runtime_dir();

}  // namespace xdg

}  // namespace microdnf

#endif
