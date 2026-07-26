#include "listaCompras.hpp"
#include <stdio.h>
#include <ctime>
#include "recomendacao.hpp"

int main()
{
    struct SistemaRecomendacao sistema;

    char nome_arquivo[100];

    cout << "\nDigite o nome do arquivo CSV (ex: dados_venda_cluster_0.csv): ";
    scanf("%99s", nome_arquivo);

    carregaDados(&sistema, nome_arquivo);

    constroiMatrizCompras(&sistema);

    cout << "\nCalculando matriz de similaridade..." << endl;
    // =========
    // constroiMatrizSimilaridade(&sistema);

    int opcao;

    cout << "\nEscolha como voce quer calcular a similaridade:\n";
    cout << "1 - Padrao\n";
    cout << "2 - Otimizado\n";
    cout << "Opcao: ";
    scanf("%d", &opcao);

    cout << "\nCalculando matriz de similaridade..." << endl;

    clock_t inicio = clock();

    if (opcao == 1)
    {
        constroiMatrizSimilaridadePadrao(&sistema);
    }
    else
    {
        constroiMatrizSimilaridadeOtimizada(&sistema);
    }

    clock_t fim = clock();

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    cout << "\nMatriz de similaridade concluida." << endl;

    cout << "Tempo de execucao: " << tempo << " segundos." << endl;
    // =========
    char codigo_digitado[50];

    for (int i = 0; i < 3; i++)
    {
        cout << "\nDigite o codigo original do cliente " << (i + 1) << ": ";
        scanf("%49s", codigo_digitado);

        exibeComprasCliente(&sistema, codigo_digitado);

        if (sistema.mapa_clientes.find(codigo_digitado) != sistema.mapa_clientes.end())
        {
            int id_cliente = sistema.mapa_clientes[codigo_digitado];

            int semelhante = encontraClienteMaisParecido(&sistema, id_cliente);

            cout << "Cliente mais parecido: " << sistema.clientes[semelhante] << endl;

            cout << "Similaridade: " << sistema.matriz_similaridade[id_cliente][semelhante] << endl;

            int k;

            cout << "Digite k: ";
            scanf("%d", &k);

            vector<ProdutoRanking> recomendados = recomendarProdutos(&sistema, id_cliente, k);

            cout << "\nProdutos recomendados:\n";

            for (int j = 0; j < recomendados.size(); j++)
            {
                cout << j + 1 << " - " << sistema.produtos[recomendados[j].idProduto] << endl;
            }
        }
    }

    return 0;
}