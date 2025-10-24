import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy


class AD(ConanFile):
    name = "ad"
    version = "0.0.1"
    settings = "os", "compiler", "build_type", "arch"

    exports_sources = "CMakeLists.txt", "include/*", "src/*", "test/*"

    def requirements(self):
        self.requires("eigen/5.0.0", transitive_headers=True)

    def build_requirements(self):
        self.tool_requires("cmake/3.31.8")
        self.test_requires("catch2/3.10.0")
        self.test_requires("benchmark/1.9.4")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        CMakeDeps(self).generate()
        CMakeToolchain(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        copy(self,
            pattern="*.h",
            src=os.path.join(self.source_folder, "include"),
            dst=os.path.join(self.package_folder, "include"),
            keep_path=True)

    def package_info(self):
        self.cpp_info.libs = []
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.set_property("cmake_target_name", "ad::ad")
        self.cpp_info.set_property("cmake_file_name", "ad")
        self.cpp_info.set_property("header_only", True)
