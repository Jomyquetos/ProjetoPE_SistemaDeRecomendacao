#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "listaCompras.hpp"
#include "recomendacao.hpp" // OBRIGATÓRIO incluir o novo hpp

namespace py = pybind11;

PYBIND11_MODULE(meu_solver, m)
{
      m.doc() = "Sistema de Recomendacao - Integracao Python/C++";

      // Mapeia a estrutura SistemaRecomendacao
      py::class_<SistemaRecomendacao>(m, "SistemaRecomendacao")
          .def(py::init<>())
          .def_readwrite("clientes", &SistemaRecomendacao::clientes)
          .def_readwrite("produtos", &SistemaRecomendacao::produtos);

      // Mapeia a nova estrutura de ranking (Atividade 3)
      py::class_<ProdutoRanking>(m, "ProdutoRanking")
          .def(py::init<>())
          .def_readwrite("id_produto", &ProdutoRanking::idProduto)
          .def_readwrite("ranking", &ProdutoRanking::ranking);

      // Mapeia a função de recomendação por Jaccard da Atividade 3 usando Lambda para ponteiro
      m.def("recomendar_produtos", [](SistemaRecomendacao &sis, int cliente, int k)
            { return recomendarProdutos(&sis, cliente, k); }, "Retorna os k produtos mais recomendados para o cliente", py::arg("sistema"), py::arg("cliente"), py::arg("k"));

      // Funções existentes do módulo ListaCompras (Atividade 1 e 2)
      m.def("carrega_dados", [](SistemaRecomendacao &sis, const std::string &nomeArquivo)
            { carregaDados(&sis, nomeArquivo.c_str()); });
      m.def("exibe_compras_cliente", [](SistemaRecomendacao &sis, const std::string &codClienteOriginal)
            { exibeComprasCliente(&sis, codClienteOriginal); });
      m.def("constroi_matriz_compras", [](SistemaRecomendacao &sis)
            { constroiMatrizCompras(&sis); });
      m.def("calcula_similaridade", [](SistemaRecomendacao &sis, int cliente1, int cliente2)
            { return calculaSimilaridade(&sis, cliente1, cliente2); });
      m.def("encontra_cliente_mais_parecido", [](SistemaRecomendacao &sis, int cliente)
            { return encontraClienteMaisParecido(&sis, cliente); });
      m.def("constroi_matriz_similaridade_padrao", [](SistemaRecomendacao &sis)
            { constroiMatrizSimilaridadePadrao(&sis); });
      m.def("constroi_matriz_similaridade_otimizada", [](SistemaRecomendacao &sis)
            { constroiMatrizSimilaridadeOtimizada(&sis); });
      m.def("constroi_matriz_csr", [](SistemaRecomendacao &sis)
            { constroiMatrizCSR(&sis); });
      m.def("constroi_matriz_similaridade_csr", [](SistemaRecomendacao &sis)
            { constroiMatrizSimilaridadeCSR(&sis); });
}