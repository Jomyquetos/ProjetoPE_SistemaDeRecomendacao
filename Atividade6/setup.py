from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        "meu_solver",
        sources=[
            "bindings.cpp",
            "solver.cpp",
            "listaCompras.cpp",
            "recomendacao.cpp"
        ],
        include_dirs=[
            pybind11.get_include()
        ],
        language="c++",
        extra_compile_args=["-std=c++11"]
    )
]

setup(
    name="meu_solver",
    version="1.0",
    author="Álefe Monteiro e Josué Henrique",
    description="Sistema de Recomendação - Integração Python/C++",
    ext_modules=ext_modules,
    zip_safe=False,
)