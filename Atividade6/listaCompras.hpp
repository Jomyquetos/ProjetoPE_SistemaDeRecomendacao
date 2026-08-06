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

double calculaSimilaridade(struct SistemaRecomendacao *sis, int cliente1, int cliente2);

int encontraClienteMaisParecido(struct SistemaRecomendacao *sis, int cliente);

void constroiMatrizSimilaridadePadrao(struct SistemaRecomendacao *sis);

void constroiMatrizSimilaridadeOtimizada(struct SistemaRecomendacao *sis);

void constroiMatrizCSR(struct SistemaRecomendacao *sis);

void constroiMatrizSimilaridadeCSR(struct SistemaRecomendacao *sis);

#endif