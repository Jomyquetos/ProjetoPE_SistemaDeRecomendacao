#include "listaCompras.hpp"
#include <stdio.h> // Para a função scanf
#include "recomendacao.hpp"

int main()
{
    struct SistemaRecomendacao sistema;

    char nome_arquivo[100]; // Variável para guardar o nome do arquivo digitado

    // Pergunta qual cluster o usuário quer ler
    cout << "Digite o nome do arquivo CSV (ex: dados_venda_cluster_0.csv): ";
    scanf("%99s", nome_arquivo);

    // Chama a função passando o nome do arquivo que você digitou
    carregaDados(&sistema, nome_arquivo);

    constroiMatrizCompras(&sistema);

    cout << "\nCalculando matriz de similaridade..."
         << endl;

    constroiMatrizSimilaridade(&sistema);

    cout << "Matriz de similaridade concluida!\n"
         << endl;

    // Variável temporária do tipo "C clássico" para podermos usar o scanf
    char codigo_digitado[50];

    // Laço para testar 3 clientes diferentes conforme exigido
    for (int i = 0; i < 3; i++)
    {
        cout << "Digite o codigo original do cliente " << (i + 1) << ": ";

        // O scanf lê a string (texto) do teclado guardando no nosso vetor de char
        scanf("%49s", codigo_digitado);

        // Passamos a estrutura por referência e o código digitado para a função.
        // O C++ converte automaticamente o char[] para std::string aqui!
        exibeComprasCliente(&sistema, codigo_digitado);

        if (sistema.mapa_clientes.find(codigo_digitado) != sistema.mapa_clientes.end())
        {
            // Obtém o ID interno do cliente
            int id_cliente = sistema.mapa_clientes[codigo_digitado];

            // Encontra o cliente mais parecido
            int semelhante = encontraClienteMaisParecido(&sistema, id_cliente);

            // Exibe o resultado
            cout << "Cliente mais parecido: " << sistema.clientes[semelhante] << endl;

            cout << "Similaridade: " << sistema.matriz_similaridade[id_cliente][semelhante] << endl;

            int k;

            cout << "Digite k: ";
            scanf("%d", &k);

            vector<ProdutoRanking> recomendados =
                recomendarProdutos(&sistema, id_cliente, k);

            cout << "\nProdutos recomendados:\n";

            for (int j = 0; j < recomendados.size(); j++)
            {
                cout << j + 1 << " - " << sistema.produtos[recomendados[j].idProduto] << endl;
            }
        }
    }

    return 0;
}