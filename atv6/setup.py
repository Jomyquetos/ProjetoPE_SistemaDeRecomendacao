from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "nosso_solver",
        ["solver.cpp"], 
    ),
]

setup(
    name="nosso_solver",
    version="1.0",
    authors="Josué e Álefe",
    description="Modulo de recomendacao em C++",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
)