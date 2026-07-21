#ifndef RECOMENDACAO_HPP
#define RECOMENDACAO_HPP

#include "listaCompras.hpp"
#include <vector>

using namespace std;

struct ProdutoRanking
{
    int idProduto;
    double ranking;
};

vector<ProdutoRanking> recomendarProdutos(struct SistemaRecomendacao *sis, int cliente, int k);

#endif