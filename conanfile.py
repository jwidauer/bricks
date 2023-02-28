from conan import ConanFile, __version__ as conan_version
from conan.tools.cmake import cmake_layout
from conan.tools.meson import MesonToolchain, Meson
from conan.tools.files import load
from conan.tools.build import check_min_cppstd
from conan.tools.scm import Version


class BricksConan(ConanFile):
    name = "bricks"
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
    exports_sources = "include/*", "tests/*"
    no_copy_source = True
    generators = "PkgConfigDeps", "VirtualBuildEnv"
    requires = "doctest/2.4.9"

    def set_version(self):
        if conan_version >= Version("2.0.0"):
            self.version = load(self, "version.txt")

    def validate(self):
        check_min_cppstd(self, 17)

    # def layout(self):
    #     if conan_version >= Version("2.0.0"):
    #         cmake_layout(self)

    def generate(self):
        tc = MesonToolchain(self)
        tc.project_options["b_coverage"] = bool(self.options.build_coverage)
        tc.generate()

    def build(self):
        if not self.conf.get("tools.build:skip_test", default=False):
            meson = Meson(self)
            meson.configure()
            meson.build()

    def package(self):
        meson = Meson(self)
        meson.install()

    def package_id(self):
        self.info.clear()
