from conans import ConanFile, Meson, tools


class CppTemplateConan(ConanFile):
    name = "bricks"
    version = "0.1.0"
    license = "<Put the package license here>"
    author = "Jakob Widauer jakob.widauer@gmail.com"
    url = "https://github.com/jwidauer/bricks"
    description = "Small, useful blocks of code, to build bigger things"
    topics = "library", "template"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_coverage": [True, False],
    }
    default_options = {"shared": False, "fPIC": True, "build_coverage": False}
    generators = "pkg_config"
    exports_sources = "src/*"
    tool_requires = "doctest/2.4.9"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):
        meson = Meson(self)
        meson.configure(defs={"b_coverage": self.options.build_coverage})
        meson.build()

    def package(self):
        meson = Meson(self)
        meson.install()

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
