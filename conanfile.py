from conans import ConanFile, CMake, tools


class ConanRecipe(ConanFile):
    name = "doctest"
    version = "2.3.8"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_paths"
    scm = {
        "type": "git",
        "url": "https://github.com/onqtam/doctest.git",
        "revision": "auto"
    }

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()
