#############################################################################
#  HandheldFanControl Copyright (C) 2025  Ziperix                           #
#                                                                           #
#  This program is free software: you can redistribute it and/or modify     #
#  it under the terms of the GNU General Public License as published by     #
#  the Free Software Foundation, either version 3 of the License, or        #
#  (at your option) any later version.                                      #
#                                                                           #
#  This program is distributed in the hope that it will be useful,          #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of           #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
#  GNU General Public License for more details.                             #
#                                                                           #
#  You should have received a copy of the GNU General Public License        #
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.   #
#############################################################################

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout

required_conan_version = ">=2.0"


class HandheldFanControl(ConanFile):
    name = "handheld-fan-control"
    version = "indev"
    description = "Fan control decky plugin for handheld PCs"
    url = "https://github.com/Zipeerix/handheld-fan-control"
    homepage = "https://github.com/Zipeerix/handheld-fan-control"
    author = "Zipeerix (ziperix@icloud.com)"
    license = "GPL-3.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"
    exports_sources = "CMakeLists.txt", "src/*", "test/*", "proto/*"
    package_type = "application"
    build_policy = "missing"
    languages = "C++"
    options = {
        "skip_static_analysis": [True, False],
        "skip_compiler_flags": [True, False],
        "skip_tests": [True, False],
    }
    default_options = {
        "skip_static_analysis": False,
        "skip_compiler_flags": False,
        "skip_tests": False,
    }
    options_description = {
        "skip_static_analysis": "Skip static analysis checks during the build process.",
        "skip_compiler_flags": "Skip applying custom compiler flags.",
        "skip_tests": "Skip building and running tests.",
    }

    def layout(self):
        cmake_layout(self)

    def build_requirements(self):
        self.tool_requires("protobuf/<host_version>")

    def configure(self):
        self.options["tomlplusplus"].exceptions = False
        self.options["spdlog"].use_std_fmt = True

    def requirements(self):
        self.requires("gtest/1.16.0")
        self.requires("protobuf/5.27.0")
        self.requires("boost/1.86.0")
        self.requires("spdlog/1.15.1")
        self.requires("tomlplusplus/3.4.0")
        self.requires("libenvpp/1.5.0")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.cache_variables["SKIP_STATIC_ANALYSIS"] = self.options.skip_static_analysis
        tc.cache_variables["SKIP_COMPILER_FLAGS"] = self.options.skip_compiler_flags
        tc.cache_variables["SKIP_TESTS"] = self.options.skip_tests
        tc.cache_variables["HANDHELD_FAN_CONTROL_VERSION"] = self.version
        tc.generate()

    def build(self):
        print("******* HandheldFanControl build *******")
        print(f"- Version: {self.version}")
        print(
            f"- Skipping static analysis: {self.options.skip_static_analysis}")
        print(f"- Skipping compiler flags: {self.options.skip_compiler_flags}")
        print(f"- Skipping tests: {self.options.skip_tests}")
        print("*****************************************")

        cmake = CMake(self)
        cmake.configure()
        cmake.build()

        if not self.options.skip_tests:
            self.run("./HandheldFanControl_Tests")
