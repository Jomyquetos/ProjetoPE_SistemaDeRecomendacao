#include "recomendacao.hpp"
#include <algorithm>

// Função auxiliar utilizada pelo std::sort
bool comparaRanking(const struct ProdutoRanking &a, const struct ProdutoRanking &b)
{
    return a.ranking < b.ranking;
}

// Atividade 3
vector<ProdutoRanking> recomendarProdutos(struct SistemaRecomendacao *sis, int cliente, int k)
{
    vector<ProdutoRanking> ranking;

    // Cria um ranking para todos os produtos
    for (int i = 0; i < sis->produtos.size(); i++)
    {
        struct ProdutoRanking produto;

        produto.idProduto = i;
        produto.ranking = 1.0;

        ranking.push_back(produto);
    }

    // Percorre todos os clientes da base
    for (int vizinho = 0; vizinho < sis->clientes.size(); vizinho++)
    {
        // Não compara o cliente com ele mesmo
        if (vizinho == cliente)
        {
            continue;
        }

        double similaridade = sis->matriz_similaridade[cliente][vizinho];

        // Ignora clientes sem produtos em comum
        if (similaridade == 1.0)
        {
            continue;
        }

        // Percorre todos os produtos comprados pelo vizinho
        for (int i = 0; i < sis->lista_compras[vizinho].size(); i++)
        {
            int produto = sis->lista_compras[vizinho][i];

            // Se o cliente já comprou esse produto,
            // não devemos recomendá-lo.
            if (sis->matriz_compras[cliente][produto] == 1)
            {
                continue;
            }

            ranking[produto].ranking *= similaridade;
        }
    }

    // Coloca os produtos já comprados no final do ranking
    for (int i = 0; i < sis->lista_compras[cliente].size(); i++)
    {
        int produto = sis->lista_compras[cliente][i];

        ranking[produto].ranking = 999999.0;
    }

    // Ordena do menor ranking para o maior
    sort(ranking.begin(), ranking.end(), comparaRanking);

    // Ajusta caso k seja maior que a quantidade de produtos
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