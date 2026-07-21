#ifndef LISTACOMPRAS_HPP
#define LISTACOMPRAS_HPP

#include <iostream> // Liberado para usarmos o cout e cin!
#include <vector>   // Para os vetores dinâmicos, igual ao list no python
#include <map>      // Para o mapeamento de códigos (dicionários), igual ao dict no python
#include <string>   // Para manipularmos os textos do CSV

using namespace std;

// Estrutura principal que guarda a base de dados na memória
struct SistemaRecomendacao
{

    // 1. Vetor de clientes: guarda os códigos originais (ex: "05090301")
    vector<string> clientes;

    // 2. Mapa de clientes: associa o Código Original (texto) -> Índice Interno (0, 1, 2...)
    map<string, int> mapa_clientes;

    // 3. Vetor de produtos: guarda os nomes descritivos de todos os produtos
    vector<string> produtos;

    // 4. Mapa de produtos: associa o Código do Produto (texto) -> Índice Interno (0, 1, 2...)
    map<string, int> mapa_produtos;

    // 5. A Lista de Compras: Um vetor onde cada posição é um cliente,
    // e dentro dele há outro vetor com os IDs dos produtos que ele comprou.
    vector<vector<int>> lista_compras;

    vector<vector<int>> matriz_compras; // atividade 2 - matriz de compras (clientes x produtos)

    vector<vector<double>> matriz_similaridade; // atividade 2 - matriz de similaridade (clientes x clientes)
};

// Função para ler o arquivo CSV e preencher as estruturas acima
void carregaDados(struct SistemaRecomendacao *sis, const char *nomeArquivo);

// Função para exibir o que um cliente comprou (aqui usaremos o cout!)
void exibeComprasCliente(struct SistemaRecomendacao *sis, const string &codClienteOriginal);

// Protótipos da Atividade 2 - Similaridade:

// Constrói a matriz de compras (clientes x produtos)
void constroiMatrizCompras(struct SistemaRecomendacao *sis);

// Calcula a matriz de similaridade
void constroiMatrizSimilaridade(struct SistemaRecomendacao *sis);

// Calcula quantos produtos dois clientes possuem em comum
double calculaSimilaridade(struct SistemaRecomendacao *sis, int cliente1, int cliente2);

// Encontra o cliente mais semelhante ao cliente informado
int encontraClienteMaisParecido(struct SistemaRecomendacao *sis, int cliente);

#endif