#include "listaCompras.hpp"
#include <string.h>

void carregaDados(struct SistemaRecomendacao *sis, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        cout << "Nao foi possivel abrir o arquivo." << endl;
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

    cout << "\nTotal de Clientes cadastrados: " << sis->clientes.size() << endl;
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

// void constroiMatrizSimilaridade(struct SistemaRecomendacao *sis)
//{
// int qtd_clientes = sis->clientes.size();

// sis->matriz_similaridade.resize(
// qtd_clientes,
// vector<double>(qtd_clientes, 0.0));

// for (int i = 0; i < qtd_clientes; i++)
//{
// for (int j = 0; j < qtd_clientes; j++)
//{
// sis->matriz_similaridade[i][j] =
// calculaSimilaridade(sis, i, j);
//}
//}
//}

// Atividade 5:

void constroiMatrizCSR(struct SistemaRecomendacao *sis)
{
    // Limpa os vetores caso já tenham sido utilizados
    sis->values.clear();
    sis->col_index.clear();
    sis->row_ptr.clear();

    int qtd_clientes = sis->matriz_compras.size();

    // A primeira linha sempre começa na posição 0
    sis->row_ptr.push_back(0);

    for (int i = 0; i < qtd_clientes; i++)
    {
        int qtd_produtos = sis->matriz_compras[i].size();

        for (int j = 0; j < qtd_produtos; j++)
        {
            if (sis->matriz_compras[i][j] != 0)
            {
                sis->values.push_back(sis->matriz_compras[i][j]);
                sis->col_index.push_back(j);
            }
        }

        // Marca onde começa a próxima linha
        sis->row_ptr.push_back(sis->values.size());
    }
}

void constroiMatrizSimilaridadeCSR(struct SistemaRecomendacao *sis)
{
    int qtd_clientes = sis->clientes.size();

    sis->matriz_similaridade.assign(qtd_clientes, vector<double>(qtd_clientes, 0.0));

    for (int i = 0; i < qtd_clientes; i++)
    {
        for (int j = i; j < qtd_clientes; j++)
        {
            int inicio1 = sis->row_ptr[i];
            int fim1 = sis->row_ptr[i + 1];

            int inicio2 = sis->row_ptr[j];
            int fim2 = sis->row_ptr[j + 1];

            int p1 = inicio1;
            int p2 = inicio2;

            double produtoInterno = 0.0;
            int quantidadeCliente1 = fim1 - inicio1;

            while (p1 < fim1 && p2 < fim2)
            {
                if (sis->col_index[p1] == sis->col_index[p2])
                {
                    produtoInterno += sis->values[p1] * sis->values[p2];

                    p1++;
                    p2++;
                }
                else if (sis->col_index[p1] < sis->col_index[p2])
                {
                    p1++;
                }
                else
                {
                    p2++;
                }
            }

            double similaridade;

            if (quantidadeCliente1 == 0)
            {
                similaridade = 1.0;
            }
            else
            {
                similaridade = 1.0 - (produtoInterno / quantidadeCliente1);
            }

            sis->matriz_similaridade[i][j] = similaridade;
            sis->matriz_similaridade[j][i] = similaridade;
        }
    }
}
// Atividade 4:

void constroiMatrizSimilaridadePadrao(struct SistemaRecomendacao *sis)
{
    int qtd_clientes = sis->clientes.size();

    sis->matriz_similaridade.assign(qtd_clientes, vector<double>(qtd_clientes, 0.0));

    for (int i = 0; i < qtd_clientes; i++)
    {
        for (int j = 0; j < qtd_clientes; j++)
        {
            sis->matriz_similaridade[i][j] = calculaSimilaridade(sis, i, j);
        }
    }
}

void constroiMatrizSimilaridadeOtimizada(struct SistemaRecomendacao *sis)
{
    int qtd_clientes = sis->clientes.size();

    sis->matriz_similaridade.assign(qtd_clientes, vector<double>(qtd_clientes, 0.0));

    for (int i = 0; i < qtd_clientes; i++)
    {
        for (int j = i; j < qtd_clientes; j++)
        {
            double similaridade = calculaSimilaridade(sis, i, j);

            sis->matriz_similaridade[i][j] = similaridade;
            sis->matriz_similaridade[j][i] = similaridade;
        }
    }
}
// =========
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