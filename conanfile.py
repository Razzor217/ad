from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain


class AD(ConanFile):
    name = "ad"
    version = "0.0.1"
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("benchmark/1.9.4")
        self.requires("eigen/5.0.0")

    def build_requirements(self):
        self.tool_requires("cmake/3.31.8")
        self.test_requires("catch2/3.10.0")

    def generate(self):
        CMakeDeps(self).generate()
        CMakeToolchain(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()
