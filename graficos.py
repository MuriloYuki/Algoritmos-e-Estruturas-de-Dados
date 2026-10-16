from pathlib import Path

import matplotlib
matplotlib.use("Agg")

import matplotlib.pyplot as plt
import pandas as pd


PASTA = Path("resultados")
SAIDA = PASTA / "graficos"
SAIDA.mkdir(parents=True, exist_ok=True)

dados = pd.concat(
    [
        pd.read_csv(PASTA / "medias_sequencial.csv"),
        pd.read_csv(PASTA / "medias_encadeada.csv"),
    ],
    ignore_index=True,
)

# O CSV usa nanossegundos; os gráficos mostrarão milissegundos.
dados["TempoMedioMs"] = dados["TempoMedioNs"] / 1_000_000

experimentos = {
    "insercoes_inicio": "100 inserções no início",
    "acessos_aleatorios": "100 acessos por índice",
    "remocoes_inicio": "100 remoções no início",
    "remocoes_meio": "100 remoções no meio",
    "percurso_completo": "Percurso completo",
}

rotulos = {
    "sequencial": "Sequencial",
    "encadeada": "Duplamente encadeada",
}

for experimento, titulo in experimentos.items():
    fig, ax = plt.subplots(figsize=(8, 5))

    for implementacao, rotulo in rotulos.items():
        selecao = dados[
            (dados["Experimento"] == experimento)
            & (dados["Implementacao"] == implementacao)
        ].sort_values("N")

        ax.plot(
            selecao["N"],
            selecao["TempoMedioMs"],
            marker="o",
            label=rotulo,
        )

    # Os tamanhos e tempos variam bastante: log facilita a comparação.
    ax.set_xscale("log")
    ax.set_yscale("log")

    ax.set_title(titulo)
    ax.set_xlabel("Tamanho inicial da lista (N) — escala log")
    ax.set_ylabel("Tempo médio do bloco (ms) — escala log")
    ax.grid(True, which="both", alpha=0.25)
    ax.legend()

    fig.tight_layout()
    fig.savefig(SAIDA / f"{experimento}.png", dpi=300)
    plt.close(fig)

# Tabela auxiliar para conferir os números e usar no relatório.
tabela = dados.pivot(
    index=["Experimento", "N"],
    columns="Implementacao",
    values="TempoMedioMs",
)
tabela.to_csv(PASTA / "comparacao_ms.csv")

print(f"Gráficos salvos em: {SAIDA}")
print(tabela)
