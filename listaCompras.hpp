#ifndef LISTACOMPRAS_HPP
#define LISTACOMPRAS_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

struct SistemaRecomendacao
{

    vector<string> clientes;

    map<string, int> mapa_clientes;

    vector<string> produtos;

    map<string, int> mapa_produtos;

    vector<vector<int>> lista_compras;

    vector<vector<int>> matriz_compras;

    vector<vector<double>> matriz_similaridade;

    vector<int> values;

    vector<int> col_index;

    vector<int> row_ptr;
};

void carregaDados(struct SistemaRecomendacao *sis, const char *nomeArquivo);

void exibeComprasCliente(struct SistemaRecomendacao *sis, const string &codClienteOriginal);

void constroiMatrizCompras(struct SistemaRecomendacao *sis);

// void constroiMatrizSimilaridade(struct SistemaRecomendacao *sis);
// Foi removido porque agora existirão duas implementações, conforme exigido pelo roteiro:
// constroiMatrizSimilaridadePadrao()
// constroiMatrizSimilaridadeOtimizada()
// O main.cpp decidirá qual delas executar.

double calculaSimilaridade(struct SistemaRecomendacao *sis, int cliente1, int cliente2);

int encontraClienteMaisParecido(struct SistemaRecomendacao *sis, int cliente);

// Atividade 4:

// Implementação tradicional (calcula toda a matriz)
void constroiMatrizSimilaridadePadrao(struct SistemaRecomendacao *sis);

// Implementação otimizada (explora a simetria da matriz)
void constroiMatrizSimilaridadeOtimizada(struct SistemaRecomendacao *sis);

// Atividade 5:

// Converte a matriz de compras para o formato CSR
void constroiMatrizCSR(struct SistemaRecomendacao *sis);

// Calcula a matriz de similaridade utilizando CSR
void constroiMatrizSimilaridadeCSR(struct SistemaRecomendacao *sis);

#endif