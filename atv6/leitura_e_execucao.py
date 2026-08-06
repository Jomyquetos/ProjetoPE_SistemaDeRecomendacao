from pathlib import Path
import nosso_solver



print("Implementação de um Sistema de Recomendação")

client=[]

data_compra=[]
cod_cliente=[]
cod_produto=[]
nome_produto=[]

for i in range(3):
    cliente = input(f"Digite o nome do cliente {i+1} que deseja ver as compras: ")
    client.append(cliente)

def ler_clientes(caminho_arquivo: str):
    clientes = []
    caminho = Path(caminho_arquivo)

    with caminho.open("r", encoding="utf-8") as f:
        for numero_linha, linha in enumerate(f, start=1):
            linha = linha.strip()
            if not linha or linha.startswith(" "):
                continue

            partes = linha.split()
            if len(partes) != 4:
                raise ValueError(
                    f"Linha {numero_linha}: esperado 4 campos (id x y demanda), mas encontrei {len(partes)}."
                )

            cliente = Cliente(
                id=int(partes[0]),
                x=float(partes[1]),
                y=float(partes[2]),
                demanda=float(partes[3]),
            )
            clientes.append(cliente)

    return clientes