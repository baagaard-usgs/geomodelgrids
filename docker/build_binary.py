#!/usr/bin/env python3
"""Create binary package.

1. Install dependencies.
  * gcc (Linux only)
  * sqlite3
  * libtiff
  * openssl
  * libcurl
  * proj
  * hdf5
  * libffi
  * Python
  * Catch2
2. Install geomodelgrids
3. Create setup.sh
4. Update linking (macOS only)
5. Create tarball.
"""

import abc
import argparse
import multiprocessing
import os
import pathlib
import platform
import requests
import shutil
import subprocess
import tarfile


# --------------------------------------------------------------------------------------------------
class Package(abc.ABC):
    VERSION = None
    TARBALL = None
    URL = None
    SRC_DIR = None
    BUILD_DIR = None

    def __init__(self, install_dir: str, env: dict, show_progress: bool = True):
        self.install_dir = install_dir
        self.env = env
        self.nthreads = multiprocessing.cpu_count()
        self.show_progress = show_progress

    def install(self):
        self.download()
        self.extract_tarball()
        self.configure()
        self.build()

    def download(self, url: str = None, tarball: str = None):
        if not url:
            url = self.URL
        if not tarball:
            tarball = self.TARBALL
        if not os.path.isfile(tarball):
            self._status(f"Downloading {url} ...")
            response = requests.get(url, stream=True)
            if response.status_code == 200:
                with open(tarball, 'wb') as f:
                    f.write(response.raw.read())
                assert tarfile.is_tarfile(tarball)
            else:
                raise IOError(f"Could not download {url}. Response status={response.status_code}.")

    def extract_tarball(self, tarball=None, path="."):
        if not tarball:
            tarball = self.TARBALL
        self._status(f"Extracting {tarball} ...")
        tfile = tarfile.open(tarball, mode="r:*")
        tfile.extractall(path=path)

    @abc.abstractmethod
    def configure(self):
        return

    @abc.abstractmethod
    def _configure(self, args: list):
        return

    def build(self):
        cwd = os.path.abspath(os.curdir)
        os.chdir(self.BUILD_DIR)
        self._run_cmd(["make", f"-j{self.nthreads}"])
        self._run_cmd(["make", "install"])
        os.chdir(cwd)

    def _run_cmd(self, cmd: list):
        self._status("Running {} ...".format(" ".join(cmd)))
        subprocess.run(cmd, env=self.env, check=True)

    def _status(self, msg):
        if self.show_progress:
            print(msg)


# --------------------------------------------------------------------------------------------------
class AutoconfPackage(Package):

    def __init__(self, install_dir, env, show_progress=True):
        super().__init__(install_dir, env, show_progress)

    def _configure(self, args: list):
        if not os.path.exists(self.BUILD_DIR):
            os.mkdir(self.BUILD_DIR)
        cwd = os.path.abspath(os.curdir)
        os.chdir(self.BUILD_DIR)
        configure = os.path.join("..", self.SRC_DIR, "configure")
        self._run_cmd([configure] + args)
        os.chdir(cwd)


# --------------------------------------------------------------------------------------------------
class CMAKEPackage(Package):

    def __init__(self, install_dir, env, show_progress=True):
        super().__init__(install_dir, env, show_progress)

    def _configure(self, args: list):
        if not os.path.exists(self.BUILD_DIR):
            os.mkdir(self.BUILD_DIR)
        cwd = os.path.abspath(os.curdir)
        os.chdir(self.BUILD_DIR)
        self._run_cmd(["cmake"] + args)
        os.chdir(cwd)


# --------------------------------------------------------------------------------------------------
class GCC(AutoconfPackage):
    VERSION = "10.3.0"
    TARBALL = f"gcc-10.3.0.tar.gz"
    URL = f"https://mirrors.kernel.org/gnu/gcc/gcc-{VERSION}/{TARBALL}"
    BUILD_DIR = "gcc-build"
    SRC_DIR = f"gcc-{VERSION}"

    EXTRAS = {
        "gmp": {
            "version": "6.2.1",
            "tarball": "gmp-{version}.tar.bz2",
            "url": "https://mirrors.kernel.org/gnu/gmp/{tarball}",
        },
        "mpc": {
            "version": "1.2.1",
            "tarball": "mpc-{version}.tar.gz",
            "url": "https://mirrors.kernel.org/gnu/mpc/{tarball}",
        },
        "mpfr": {
            "version": "4.1.0",
            "tarball": "mpfr-{version}.tar.gz",
            "url": "https://mirrors.kernel.org/gnu/mpfr/{tarball}",
        }
    }

    def configure(self):
        ARGS = [
            f"--prefix={self.install_dir}",
            "--enable-languages=c,c++",
            "--disable-multilib",
        ]
        self._configure(ARGS)

    def install(self):
        self.download()
        self.extract_tarball()

        gcc_src = f"gcc-{self.VERSION}"
        for label, pkg in self.EXTRAS.items():
            tarball = pkg["tarball"].format(version=pkg["version"])
            url = pkg["url"].format(version=pkg["version"], tarball=tarball)

            self.download(url=url, tarball=tarball)
            self.extract_tarball(path=gcc_src, tarball=tarball)

            link_src = f"{label}-{pkg['version']}"
            link_dest = os.path.join(gcc_src, label)
            os.symlink(link_src, link_dest)

        self.configure()
        self.build()


# --------------------------------------------------------------------------------------------------
class Sqlite(AutoconfPackage):
    VERSION = "3410200"
    TARBALL = f"sqlite-autoconf-{VERSION}.tar.gz"
    URL = f"https://www.sqlite.org/2021/{TARBALL}"
    BUILD_DIR = "sqlite-build"
    SRC_DIR = f"sqlite-autoconf-{VERSION}"

    def configure(self):
        ARGS = [
            f"--prefix={self.install_dir}",
            "--enable-shared",
        ]
        self._configure(ARGS)


# --------------------------------------------------------------------------------------------------
class Tiff(AutoconfPackage):
    VERSION = "4.5.0"
    TARBALL = f"tiff-{VERSION}.tar.gz"
    URL = f"http://download.osgeo.org/libtiff/{TARBALL}"
    BUILD_DIR = "tiff-build"
    SRC_DIR = f"tiff-{VERSION}"

    def configure(self):
        ARGS = [
            f"--prefix={self.install_dir}",
            "--enable-shared",
        ]
        self._configure(ARGS)


# --------------------------------------------------------------------------------------------------
class OpenSSL(AutoconfPackage):
    VERSION = "3.1.0"
    TARBALL = f"openssl-{VERSION}.tar.gz"
    URL = f"https://www.openssl.org/source/{TARBALL}"
    SRC_DIR = f"openssl-{VERSION}"
    BUILD_DIR = SRC_DIR

    def configure(self):
        ARGS = [
            f"--prefix={self.install_dir}",
            f"--openssldir={self.install_dir}/share/openssl",
            "shared",
        ]
        cwd = os.path.abspath(os.curdir)
        os.chdir(self.SRC_DIR)
        configure = os.path.join(".", "config")
        self._run_cmd([configure] + ARGS)
        os.chdir(cwd)


# --------------------------------------------------------------------------------------------------
class Curl(AutoconfPackage):
    VERSION = "8.0.1"
    TARBALL = f"curl-{VERSION}.tar.gz"
    URL = f"https://curl.se/download/{TARBALL}"
    BUILD_DIR = "curl-build"
    SRC_DIR = f"curl-{VERSION}"

    def configure(self):
        ARGS = [
            f"--prefix={self.install_dir}",
            "--enable-shared",
        ]
        self._configure(ARGS)


# --------------------------------------------------------------------------------------------------
class Proj(CMAKEPackage):
    VERSION = "9.2.0"
    TARBALL = f"proj-{VERSION}.tar.gz"
    URL = f"https://download.osgeo.org/proj/{TARBALL}"
    BUILD_DIR = "proj-build"
    SRC_DIR = f"proj-{VERSION}"

    def configure(self):
        # cmake -DCMAKE_INSTALL_PREFIX=$(prefix) -DCMAKE_C_COMPILER=$(CC) -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_INSTALL_NAME_DIR=$(prefix)/lib TIFF_INCLUDE_DIR=${TIFF_INCDIR} TIFF_LIBRARY=${TIFF_LIBDIR} SQLITE3_INCLUDE_DIR=${SQLITE3_INCDIR} SQLITE3_LIBRARY=${SQLITE3_LIBDIR} -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release ../proj-${PROJ_VER} && \

        ARGS = [
            f"--prefix={self.install_dir}",
            "--enable-shared",
            f"SQLITE3_CFLAGS=-I{self.install_dir}/include",
            f"SQLITE3_LIBS=-L{self.install_dir}/lib -lsqlite3",
            f"TIFF_CFLAGS=-I{self.install_dir}/include",
            f"TIFF_LIBS=-L{self.install_dir}/lib -ltiff",
        ]
        self._configure(ARGS)

    def install(self):
        super().install()

        cmd = ["projsync", "--system-directory", "--bbox", "-128.0,34.0,-118.0,42.0"]
        self._run_cmd(cmd)


# --------------------------------------------------------------------------------------------------
class Catch2(CMAKEPackage):
    VERSION = "3.3.2"
    TARBALL = f"catch2-{VERSION}.tar.gz"
    URL = f"https://github.com/catchorg/Catch2/archive/refs/tags/v{VERSION}.tar.gz"
    BUILD_DIR = "catch2-build"
    SRC_DIR = f"Catch2-{VERSION}"

    def configure(self):
        # 	$(env_compilers) cmake -DCMAKE_INSTALL_PREFIX=$(prefix) -DCMAKE_POSITION_INDEPENDENT_CODE=ON ../Catch2-$(CATCH2_VER)
        ARGS = [
            f"--prefix={self.install_dir}",
            "--enable-shared",
            f"SQLITE3_CFLAGS=-I{self.install_dir}/include",
            f"SQLITE3_LIBS=-L{self.install_dir}/lib -lsqlite3",
            f"TIFF_CFLAGS=-I{self.install_dir}/include",
            f"TIFF_LIBS=-L{self.install_dir}/lib -ltiff",
        ]
        self._configure(ARGS)

    def install(self):
        super().install()

        cmd = ["projsync", "--system-directory", "--bbox", "-128.0,34.0,-118.0,42.0"]
        self._run_cmd(cmd)


# --------------------------------------------------------------------------------------------------
class FFI(AutoconfPackage):
    VERSION = "3.4.4"
    TARBALL = f"libffi-{VERSION}.tar.gz"
    URL = f"https://github.com/libffi/libffi/releases/download/v{VERSION}{TARBALL}}"
    BUILD_DIR = "libffi-build"
    SRC_DIR = f"libffi-{VERSION}"

    def configure(self):
        ARGS = [
            f"--prefix={self.install_dir}",
            "--enable-shared",
            "--disable-static",
        ]
        self._configure(ARGS)


# --------------------------------------------------------------------------------------------------
class Python(AutoconfPackage):
    VERSION = "3.10.10"
    TARBALL = f"Python-{VERSION}.tar.gz"
    URL = f"https://www.python.org/ftp/python/{VERSION}}/{TARBALL}}"
    BUILD_DIR = "python-build"
    SRC_DIR = f"Python-{VERSION}"

    def configure(self):
        ARGS = [
            f"--prefix={self.install_dir}",
            "--enable-shared",
            "--enable-optimizations",
        ]
        self._configure(ARGS)

    def install(self):
        super().install()

        cmd = ["pip", "--install", "--upgrade", "pip", "setuptools", "certifi"]
        self._run_cmd(cmd)

        cmd = ["pip", "--install", "pybind11", "h5py", "matplotlib"]
        self._run_cmd(cmd)


# --------------------------------------------------------------------------------------------------
class HDF5(AutoconfPackage):
    VERSION = "1.14.0"
    TARBALL = f"hdf5-{VERSION}.tar.gz"
    URL = f"https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.12/hdf5-{VERSION}/src/{TARBALL}"
    BUILD_DIR = "hdf5-build"
    SRC_DIR = f"hdf5-{VERSION}"

    def configure(self):
        ARGS = [
            f"--prefix={self.install_dir}",
            "--enable-shared",
            "--disable-static",
        ]
        self._configure(ARGS)


# --------------------------------------------------------------------------------------------------
class GeoModelGrids(AutoconfPackage):
    BUILD_DIR = "geomodelgrids-build"

    def install(self):
        self.SRC_DIR = f"geomodelgrids-{self.VERSION}"
        self.TARBALL = f"geomodelgrids-{self.VERSION}.tar.gz"

        self.extract_tarball()
        self.configure()
        self.build()

    def configure(self):
        ARGS = [
            f"--prefix={self.install_dir}",
            "--enable-shared",
            "--enable-python",
            "--disable-gdal",
            "--enable-testing",
        ]
        self._configure(ARGS)


# --------------------------------------------------------------------------------------------------
class macOS(object):

    @staticmethod
    def update_linking(install_dir):
        path = pathlib.Path(install_dir)
        for proj in path.glob("bin/*"):
            macOS.update_deplibs(proj)

        for lib in path.glob("lib/*.dylib"):
            macOS.update_deplibs(lib)

    @staticmethod
    def update_deplibs(filename):
        if filename.is_symlink() or filename.is_dir():
            return

        proc = subprocess.run(["otool", "-L", filename], stdout=subprocess.PIPE, check=True)
        output = proc.stdout.decode("utf-8")
        deplibs = []
        for line in output.split("\t")[1:]:
            deplibs.append(line.split()[0])
        for libPathAbs in deplibs:
            if libPathAbs.startswith("/usr") or libPathAbs.startswith("/System"):
                continue
            libName = os.path.split(libPathAbs)[1]
            libPathNew = f"@executable_path/../lib/{libName}"
            cmd = ["install_name_tool", "-change", libPathAbs, libPathNew, str(filename)]
            subprocess.run(cmd, check=True)


# --------------------------------------------------------------------------------------------------
class App(object):

    def __init__(self, show_progress: bool = True):
        self.show_progress = show_progress

    def main(self, **kwargs):
        args = argparse.Namespace(**kwargs) if kwargs else self._parse_command_line()
        self.show_progress = args.show_progress
        self.install_dir = os.path.abspath(args.install_dir)
        GeoModelGrids.VERSION = args.version
        self._set_env()

        if (args.install_gcc or args.all) and platform.system() == "Linux":
            self._install(GCC)
        if args.install_sqlite or args.all:
            self._install(Sqlite)
        if args.install_openssl or args.all:
            self._install(OpenSSL)
        if args.install_curl or args.all:
            self._install(Curl)
        if args.install_tiff or args.all:
            self._install(Tiff)
        if args.install_proj or args.all:
            self._install(Proj)
        if args.install_hdf5 or args.all:
            self._install(HDF5)
        if args.install_catch2 or args.all:
            self._install(Catch2)
        if args.install_python or args.all:
            self._install(FFI)
            self._install(Python)
        if args.install_gmg or args.all:
            self._install(GeoModelGrids)
        if (args.update_linking or args.all) and platform.system() == "Darwin":
            macOS.update_linking(self.install_dir)
        if args.create_setup or args.all:
            self._create_setup()
        if args.create_tarball or args.all:
            self._create_tarball()

    def _install(self, cls):
        dep = cls(self.install_dir, self.env, show_progress=self.show_progress)
        dep.install()

    def _set_env(self):
        self.env = {
            "PATH": f"{self.install_dir}/bin:/usr/bin:/bin:/usr/sbin:/sbin",
            "CPPFLAGS": f"-I{self.install_dir}/include -DNDEBUG",
            "LDFLAGS": f"-L{self.install_dir}/lib",
            "CFLAGS": "-O3",
            "CXXFLAGS": "-O3",
        }
        if platform.system() == "Darwin":
            self.env.update({
                "CC": "clang",
                "CXX": "clang++",
            })
        elif platform.system() == "Linux":
            self.env.update({
                "CC": "gcc",
                "CXX": "g++",
                "LDFLAGS": f"-L{self.install_dir}/lib -L{self.install_dir}/lib64",
                "LD_LIBRARY_PATH": f"{self.install_dir}/lib:{self.install_dir}/lib64",
            })
        else:
            raise ValueError(f"Unknown platform '{platform.system()}'.")

    def _create_setup(self):
        self._status("Creating setup.sh ...")
        filename = os.path.join(self.install_dir, "setup.sh")
        with open(filename, "w") as fout:
            lines = [
                'gmg=`pwd`',
                '',
                'if test ! -f bin/geomodelgrids_query; then',
                '    echo "ERROR: Source this script from the top-level geomodelgrids directory."',
                '    echo "    cd [directory constaining setup.sh]"',
                '    echo "    source setup.sh"',
                '',
                'else',
                '    export PATH="$gmg/bin:/bin:/usr/bin:/sbin:/usr/sbin:$PATH"',
            ]
            if platform.system() == "Linux":
                lines += [
                    '    export LD_LIBRARY_PATH="$gmg/lib:$gmg/lib64:${LD_LIBRARY_PATH}"',
                ]
            lines += [
                '    echo "Ready to run GeoModelGrids!"',
                'fi',
            ]
            fout.write("\n".join(lines))

    def _create_tarball(self):
        def exclude(tarinfo):
            EXCLUDE = (
                "gcc",
                "g++",
                "gcov",
                "gcov-dump",
                "gcov-tool",
                "cpp",
                "cc",
                "c++",
                "lto-dump",
            )
            filepath = tarinfo.name
            if os.path.splitext(filepath)[1] == ".a":
                return None
            filename = os.path.split(filepath)[1]
            if filename in EXCLUDE:
                return None
            if filename.startswith("x86_64-pc-linux-gnu"):
                return None
            if filename.startswith("libasan") or \
                    filename.startswith("libtsan") or \
                    filename.startswith("libubsan") or \
                    filename.startswith("liblsan"):
                return None
            if os.path.split(filepath)[0].endswith("libexec"):
                return None
            return tarinfo
        self._status("Creating tarball ...")
        version = GeoModelGrids.VERSION

        if platform.system() == "Darwin":
            arch = "Darwin_{}".format(platform.machine())
        elif platform.system() == "Linux":
            arch = "Linux_{}".format(platform.machine())
        tarball = f"geomodelgrids-{version}-{arch}.tar.gz"

        shutil.copytree(f"geomodelgrids-{version}", os.path.join(self.install_dir, "src"))
        with tarfile.open(tarball, mode="w:gz") as tfile:
            tfile.add(self.install_dir, arcname=f"geomodelgrids-{version}-{arch}", filter=exclude)


def cli():
    """Parse command line arguments.
    """
    DESCRIPTION = (
        "Application for creating GeoModelGrids binary packages."
    )

    parser = argparse.ArgumentParser(description=DESCRIPTION)
    parser.add_argument("--install-dir", action="store", dest="install_dir",
                        required=True, help="Install directory.")
    parser.add_argument("--version", action="store", dest="version",
                        required=True, help="Set version tag.")

    parser.add_argument("--gcc", action="store_true", dest="install_gcc", help="Install gcc.")
    parser.add_argument("--sqlite", action="store_true", dest="install_sqlite", help="Install sqlite.")
    parser.add_argument("--tiff", action="store_true", dest="install_tiff", help="Install libtiff.")
    parser.add_argument("--openssl", action="store_true", dest="install_openssl", help="Install openssl.")
    parser.add_argument("--curl", action="store_true", dest="install_curl", help="Install libcurl.")
    parser.add_argument("--proj", action="store_true", dest="install_proj", help="Install proj.")
    parser.add_argument("--hdf5", action="store_true", dest="install_hdf5", help="Install HDF5.")
    parser.add_argument("--catch2", action="store_true", dest="install_catch2", help="Install Catch2.")
    parser.add_argument("--python", action="store_true", dest="install_python", help="Install Python.")
    parser.add_argument("--geomodelgrids", action="store_true", dest="install_gmg", help="Install GeoModelGrids.")
    parser.add_argument("--create-setup", action="store_true", dest="create_setup", help="Create setup.sh.")
    parser.add_argument("--update-linking", action="store_true",
                        dest="update_linking", help="Update Darwin linking.")
    parser.add_argument("--create-tarball", action="store_true", dest="create_tarball", help="Create tarball.")

    parser.add_argument("--all", action="store_true", dest="all", help="Run all steps.")
    parser.add_argument("--quiet", action="store_false", dest="show_progress", default=True)
    args = parser.parse_args()

    app = App()

    return args

    def _status(self, msg):
        if self.show_progress:
            print(msg)


# --------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    App().main()


# End of file
