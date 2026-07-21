#include "listaCompras.hpp" //cpp precisa conhecer o header para saber o que tem dentro da struct SistemaRecomendacao
#include <string.h>         // Necessário para a função strcmp do C

// Atividade 1
// Função que ler o arquivo csv
void carregaDados(struct SistemaRecomendacao *sis, const char *nomeArquivo)
{
    // Abre o arquivo em modo de leitura (read) usando C puro
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        cout << "Não foi possível abrir o arquivo." << endl;
        return;
    }

    // Variáveis em C (vetores de char) para guardar os pedaços do texto temporariamente
    char data[20], cod_cliente[20], cod_produto[20], nome_produto[100];

    // Mapeamento
    while (fscanf(arquivo, "%19[^,],%19[^,],%19[^,],%99[^\n]\n", data, cod_cliente, cod_produto, nome_produto) == 4)
    {

        // Se for a linha de cabeçalho do CSV, a ignoramos usando strcmp
        if (strcmp(cod_cliente, "COD_CLIENTE") == 0) // Ela compara duas strings do tipo char* e retorna 0 se forem iguais
            continue;

        // Se o cliente (string) ainda não estiver no nosso mapa
        if (sis->mapa_clientes.find(cod_cliente) == sis->mapa_clientes.end())
        {
            sis->mapa_clientes[cod_cliente] = sis->clientes.size(); // Gera um novo índice (0, 1, 2...)
            sis->clientes.push_back(cod_cliente);
        }

        // Se o produto (string) ainda não estiver no nosso mapa
        if (sis->mapa_produtos.find(cod_produto) == sis->mapa_produtos.end())
        {
            sis->mapa_produtos[cod_produto] = sis->produtos.size();
            sis->produtos.push_back(nome_produto);
        }
    }

    // Com todos os clientes descobertos, ajustamos o tamanho da matriz principal da Lista de Compras
    sis->lista_compras.resize(sis->clientes.size());

    rewind(arquivo);

    // Relacionamento
    while (fscanf(arquivo, "%19[^,],%19[^,],%19[^,],%99[^\n]\n", data, cod_cliente, cod_produto, nome_produto) == 4)
    {
        if (strcmp(cod_cliente, "COD_CLIENTE") == 0)
            continue;

        // Pega os índices numéricos únicos que criamos na Fase 1
        int id_cliente = sis->mapa_clientes[cod_cliente];
        int id_produto = sis->mapa_produtos[cod_produto];

        // Adiciona o produto na cesta de compras deste cliente
        sis->lista_compras[id_cliente].push_back(id_produto);
    }

    fclose(arquivo); // Encerra o arquivo corretamente

    // Agora usamos o cout para exibir o sucesso!
    cout << "Base de dados carregada com sucesso!" << endl;
    cout << "Total de Clientes cadastrados: " << sis->clientes.size() << endl;
    cout << "Total de Produtos cadastrados: " << sis->produtos.size() << endl;
}

// Função para exibir o que um cliente comprou (Entregável 1)
void exibeComprasCliente(struct SistemaRecomendacao *sis, const string &codClienteOriginal)
{

    // Verifica se o código do cliente realmente existe na base
    if (sis->mapa_clientes.find(codClienteOriginal) != sis->mapa_clientes.end())
    {

        int id_cliente = sis->mapa_clientes[codClienteOriginal];

        cout << "\nProdutos comprados pelo cliente " << codClienteOriginal << ":" << endl;

        // Varre a lista de produtos daquele cliente e exibe
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

// Atividade 2 - Similaridade
void constroiMatrizCompras(struct SistemaRecomendacao *sis)
{
    int qtd_clientes = sis->clientes.size();
    int qtd_produtos = sis->produtos.size();

    // Cria a matriz preenchida com zeros
    sis->matriz_compras.resize(qtd_clientes, vector<int>(qtd_produtos, 0));

    // Percorre todos os clientes
    for (int i = 0; i < qtd_clientes; i++)
    {
        // Percorre todos os produtos comprados pelo cliente
        for (int j = 0; j < sis->lista_compras[i].size(); j++)
        {
            int id_produto = sis->lista_compras[i][j]; // Pega o ID do produto comprado pelo cliente i

            // Marca que o cliente comprou esse produto
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