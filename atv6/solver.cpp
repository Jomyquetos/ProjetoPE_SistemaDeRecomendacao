#include "listaCompras.hpp" 
#include "recomendacao.hpp"      

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <algorithm>
#include <iostream> 
#include <vector>  
#include <map>   
#include <string>


namespace py = pybind11;

void carregaDados(struct SistemaRecomendacao *sis, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        cout << "Não foi possível abrir o arquivo." << endl;
        return;
    }

    char data[20], cod_cliente[20], cod_produto[20], nome_produto[100];

    while (fscanf(arquivo, "%19[^,],%19[^,],%19[^,],%99[^\n]\n", data, cod_cliente, cod_produto, nome_produto) == 4)
    {

        if (strcmp(cod_cliente, "COD_CLIENTE") == 0)
            continue;

        if (sis->mapa_clientes.find(cod_cliente) == sis->mapa_clientes.end())
        {
            sis->mapa_clientes[cod_cliente] = sis->clientes.size();
            sis->clientes.push_back(cod_cliente);
        }

        if (sis->mapa_produtos.find(cod_produto) == sis->mapa_produtos.end())
        {
            sis->mapa_produtos[cod_produto] = sis->produtos.size();
            sis->produtos.push_back(nome_produto);
        }
    }

    sis->lista_compras.resize(sis->clientes.size());

    rewind(arquivo);

    while (fscanf(arquivo, "%19[^,],%19[^,],%19[^,],%99[^\n]\n", data, cod_cliente, cod_produto, nome_produto) == 4)
    {
        if (strcmp(cod_cliente, "COD_CLIENTE") == 0)
            continue;

        int id_cliente = sis->mapa_clientes[cod_cliente];
        int id_produto = sis->mapa_produtos[cod_produto];

        sis->lista_compras[id_cliente].push_back(id_produto);
    }

    fclose(arquivo);

    cout << "Base de dados carregada com sucesso!" << endl;
    cout << "Total de Clientes cadastrados: " << sis->clientes.size() << endl;
    cout << "Total de Produtos cadastrados: " << sis->produtos.size() << endl;
}

void exibeComprasCliente(struct SistemaRecomendacao *sis, const string &codClienteOriginal)
{

    if (sis->mapa_clientes.find(codClienteOriginal) != sis->mapa_clientes.end())
    {

        int id_cliente = sis->mapa_clientes[codClienteOriginal];

        cout << "\nProdutos comprados pelo cliente " << codClienteOriginal << ":" << endl;

        for (int i = 0; i < sis->lista_compras[id_cliente].size(); i++)
        {
            int id_produto = sis->lista_compras[id_cliente][i];

            cout << "- " << sis->produtos[id_produto] << endl;
        }
    }
    else
    {
        cout << "\nCliente " << codClienteOriginal << " nao encontrado." << endl;
    }
}

void constroiMatrizCompras(struct SistemaRecomendacao *sis)
{
    int qtd_clientes = sis->clientes.size();
    int qtd_produtos = sis->produtos.size();

    sis->matriz_compras.resize(qtd_clientes, vector<int>(qtd_produtos, 0));

    for (int i = 0; i < qtd_clientes; i++)
    {
        for (int j = 0; j < sis->lista_compras[i].size(); j++)
        {
            int id_produto = sis->lista_compras[i][j];

            sis->matriz_compras[i][id_produto] = 1;
        }
    }
}

void constroiMatrizSimilaridade(struct SistemaRecomendacao *sis)
{
    int qtd_clientes = sis->clientes.size();

    sis->matriz_similaridade.resize(
        qtd_clientes,
        vector<double>(qtd_clientes, 0.0));

    for (int i = 0; i < qtd_clientes; i++)
    {
        for (int j = 0; j < qtd_clientes; j++)
        {
            sis->matriz_similaridade[i][j] =
                calculaSimilaridade(sis, i, j);
        }
    }
}

double calculaSimilaridade(struct SistemaRecomendacao *sis, int cliente1, int cliente2)
{
    int intersecao = 0;
    int quantidadeCliente1 = 0;

    int qtd_produtos = sis->produtos.size();

    for (int i = 0; i < qtd_produtos; i++)
    {
        if (sis->matriz_compras[cliente1][i] == 1)
        {
            quantidadeCliente1++;
        }

        if (sis->matriz_compras[cliente1][i] == 1 &&
            sis->matriz_compras[cliente2][i] == 1)
        {
            intersecao++;
        }
    }

    if (quantidadeCliente1 == 0)
    {
        return 1.0;
    }

    return 1.0 - ((double)intersecao / quantidadeCliente1);
}

int encontraClienteMaisParecido(struct SistemaRecomendacao *sis, int cliente)
{
    double menorSimilaridade = 2.0;
    int clienteMaisParecido = -1;

    int qtd_clientes = sis->clientes.size();

    for (int i = 0; i < qtd_clientes; i++)
    {
        if (i == cliente)
        {
            continue;
        }

        if (sis->matriz_similaridade[cliente][i] < menorSimilaridade)
        {
            menorSimilaridade = sis->matriz_similaridade[cliente][i];
            clienteMaisParecido = i;
        }
    }

    return clienteMaisParecido;
}



bool comparaRanking(const struct ProdutoRanking &a, const struct ProdutoRanking &b)
{
    return a.ranking < b.ranking;
}

vector<ProdutoRanking> recomendarProdutos(struct SistemaRecomendacao *sis, int cliente, int k)
{
    vector<ProdutoRanking> ranking;

    for (int i = 0; i < sis->produtos.size(); i++)
    {
        struct ProdutoRanking produto;

        produto.idProduto = i;
        produto.ranking = 1.0;

        ranking.push_back(produto);
    }

    for (int vizinho = 0; vizinho < sis->clientes.size(); vizinho++)
    {
        if (vizinho == cliente)
        {
            continue;
        }

        double similaridade = sis->matriz_similaridade[cliente][vizinho];

        if (similaridade == 1.0)
        {
            continue;
        }

        for (int i = 0; i < sis->lista_compras[vizinho].size(); i++)
        {
            int produto = sis->lista_compras[vizinho][i];

            if (sis->matriz_compras[cliente][produto] == 1)
            {
                continue;
            }

            ranking[produto].ranking *= similaridade;
        }
    }

    for (int i = 0; i < sis->lista_compras[cliente].size(); i++)
    {
        int produto = sis->lista_compras[cliente][i];

        ranking[produto].ranking = 999999.0;
    }

    sort(ranking.begin(), ranking.end(), comparaRanking);

    if (k > ranking.size())
    {
        k = ranking.size();
    }

    vector<ProdutoRanking> resposta;

    for (int i = 0; i < k; i++)
    {
        resposta.push_back(ranking[i]);
    }

    return resposta;
}


PYBIND11_MODULE(meu_solver, m) {
    m.doc() = "Modulo C++ de Recomendacao integrado via pybind11";

    m.def("carregar dados", &carregaDados, 
          "Carrega os dados do sistema de recomendacao",
          py::arg("nome_arquivo"));

    m.def("exibir compras cliente", &exibeComprasCliente,
          "Exibe as compras de um cliente especifico",
          py::arg("cod_cliente"));

    m.def("constroi matriz compras", &constroiMatrizCompras,
          "Constroi a matriz de compras (clientes x produtos)");
    
    m.def("constroi matriz similaridade", &constroiMatrizSimilaridade,
          "Constroi a matriz de similaridade (clientes x clientes)");
    
    m.def("calcula similaridade", &calculaSimilaridade,
          "Calcula a similaridade entre dois clientes",
          py::arg("cliente1"), py::arg("cliente2"));

    m.def("encontra cliente mais parecido", &encontraClienteMaisParecido,
          "Encontra o cliente mais parecido com o cliente especificado",
          py::arg("cliente"));
    
    m.def("recomendar produtos", &recomendarProdutos,
          "Recomenda produtos para um cliente especifico",
          py::arg("cliente"), py::arg("k"));
}