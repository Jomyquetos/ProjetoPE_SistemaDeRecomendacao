#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <string>
#include <vector>

#include "listaCompras.hpp"
#include "recomendacao.hpp"

using namespace std;

//----------------------------------------------------
// Carrega os dados do sistema e constrói a matriz
// de compras.
//----------------------------------------------------

SistemaRecomendacao carregarSistema(const char *arquivo);

//----------------------------------------------------
// Calcula a matriz de similaridade.
//
// algoritmo:
// 1 - Padrão
// 2 - Otimizado
// 3 - CSR
//----------------------------------------------------

void calcularSimilaridade(SistemaRecomendacao &sis,
                          int algoritmo);

//----------------------------------------------------
// Retorna o código do cliente mais parecido.
// Retorna uma string vazia caso o cliente não exista.
//----------------------------------------------------

string clienteMaisParecido(SistemaRecomendacao &sis,
                           const string &codigoCliente);

//----------------------------------------------------
// Retorna os k produtos recomendados para um cliente.
//----------------------------------------------------

vector<ProdutoRanking> recomendar(SistemaRecomendacao &sis,
                                  const string &codigoCliente,
                                  int k);

#endif