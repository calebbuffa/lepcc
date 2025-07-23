import subprocess
from pathlib import Path

import pybind11
from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext

HERE = Path(__file__).parent.absolute()


class CMakeBuildExt(build_ext):
    def run(self):
        # Run CMake to build lepcc.lib
        lepcc_dir = HERE / "lepcc"
        build_dir = lepcc_dir / "build"
        if not build_dir.exists() or not (build_dir / "lepcc.lib").exists():
            build_dir.mkdir(exist_ok=True)
            cmake_cmd = [
                "cmake",
                "-S",
                lepcc_dir.as_posix(),
                "-B",
                build_dir.as_posix(),
                "-DCMAKE_BUILD_TYPE=Release",
            ]
            subprocess.check_call(cmake_cmd)
            build_cmd = [
                "cmake",
                "--build",
                build_dir.as_posix(),
                "--config",
                "Release",
            ]
            subprocess.check_call(build_cmd)
        super().run()


ext_modules = [
    Extension(
        "lepcc",
        sources=["src/lepcc.cpp"],
        include_dirs=[pybind11.get_include(), "lepcc/src/include"],
        library_dirs=["lepcc/build/Release"],
        libraries=["lepcc"],
        language="c++",
    )
]

setup(
    name="lepcc",
    version="0.1.0",
    author="Caleb Buffa",
    description="Python bindings for Esri LEPCC library",
    long_description=(HERE / "README.md").read_text(),
    long_description_content_type="text/markdown",
    ext_modules=ext_modules,
    cmdclass={"build_ext": CMakeBuildExt},
    zip_safe=False,
    license="Apache-2.0",
    classifiers=[
        "License :: OSI Approved :: Apache Software License",
        "Programming Language :: Python :: 3",
        "Programming Language :: C++",
    ],
    include_package_data=True,
)
