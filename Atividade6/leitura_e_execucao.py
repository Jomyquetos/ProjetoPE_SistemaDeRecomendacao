import time
import meu_solver  

def main():
    print("[1/4] Inicializando a estrutura do Sistema de Recomendação...")
    sistema = meu_solver.SistemaRecomendacao()

    print("[2/4] Carregando dados do arquivo CSV...")
    meu_solver.carrega_dados(sistema, "dados_vendas.csv")

    # Limitando a base para uma amostra segura de 500 clientes
    # Isso permite alocar a matriz de similaridade exigida pelo seu recomendacao.cpp
    tamanho_amostra = min(500, len(sistema.clientes))
    sistema.clientes = sistema.clientes[:tamanho_amostra]
    print(f" -> Amostra reduzida para os primeiros {tamanho_amostra} clientes para viabilizar o cálculo em memória RAM.")

    print("[3/4] Construindo a matriz de compras dos clientes...")
    meu_solver.constroi_matriz_compras(sistema)

    print("[4/4] Computando Matrizes de Similaridade...")
    inicio = time.time()
    meu_solver.constroi_matriz_similaridade_padrao(sistema)
    fim = time.time()
    print(f"    * Tempo Abordagem Padrão (Amostra {tamanho_amostra}): {fim - inicio:.4f} segundos")

    # Teste da Abordagem Otimizada
    inicio = time.time()
    meu_solver.constroi_matriz_similaridade_otimizada(sistema)
    fim = time.time()
    print(f"    * Tempo Abordagem Otimizada (Amostra {tamanho_amostra}): {fim - inicio:.4f} segundos")

    # Executa a recomendação para 3 clientes conforme exigido pela Atividade 3 do PDF
    print("\n[5/5] Gerando recomendações para os clientes selecionados...")
    
    # Vamos selecionar 3 índices de clientes dentro da nossa amostra
    clientes_para_teste = [0, 1, 2] 
    k_produtos = 3 # Quantidade de recomendações desejadas

    for id_cliente in clientes_para_teste:
        cod_original = sistema.clientes[id_cliente]
        
        print(f"\n============================================================")
        print(f" CLIENTE ANALISADO: {cod_original} (Índice Interno: {id_cliente})")
        print(f"============================================================")
        
        # Exibe o que ele já comprou
        meu_solver.exibe_compras_cliente(sistema, cod_original)
        
        # Chama a sua função contida no recomendacao.cpp
        inicio_rec = time.time()
        recomendacoes = meu_solver.recomendar_produtos(sistema, id_cliente, k_produtos)
        fim_rec = time.time()
        
        print(f"\n -> Top {k_produtos} Produtos Recomendados (Tempo: {fim_rec - inicio_rec:.4f}s):")
        if len(recomendacoes) == 0:
            print("    Nenhuma recomendação gerada.")
        else:
            for idx, item in enumerate(recomendacoes):
                nome_prod = sistema.produtos[item.id_produto]
                # Se o ranking continuou 1.0, significa que nenhum vizinho comprou algo diferente
                if item.ranking >= 999999.0:
                    print(f"    {idx + 1}. [Sem sugestões inéditas dos vizinhos]")
                    break
                print(f"    {idx + 1}. [Score: {item.ranking:.4f}] -> {nome_prod}")
    print(f"============================================================\n")

if __name__ == "__main__":
    main()