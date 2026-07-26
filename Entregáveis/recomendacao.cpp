#include "recomendacao.hpp"
#include <algorithm>

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