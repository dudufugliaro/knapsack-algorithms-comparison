import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats

# === 1. Ler CSV ===
df = pd.read_csv("execucoes2.csv")

# === 2. Calcular média e intervalo de confiança 95% por (n, algoritmo) ===
resultados = []

for (n, algoritmo), grupo in df.groupby(["instancia", "algoritmo"]):
    tempos = grupo["tempo"].values
    media = np.mean(tempos)
    desvio = np.std(tempos, ddof=1)
    n_exec = len(tempos)
    t_critico = stats.t.ppf(1 - 0.05/2, df=n_exec - 1)
    erro_margem = t_critico * (desvio / np.sqrt(n_exec))
    resultados.append([n, algoritmo, media, erro_margem])

dados = pd.DataFrame(resultados, columns=["instancia", "algoritmo", "media", "erro"])

# === 3. Plotar gráfico com barras de erro ===
plt.figure(figsize=(10, 6))

for algoritmo in dados["algoritmo"].unique():
    subset = dados[dados["algoritmo"] == algoritmo]
    plt.errorbar(
        subset["instancia"], subset["media"],
        yerr=subset["erro"], capsize=5,
        marker="o", label=algoritmo
    )

plt.yscale("log")
plt.xlabel("Tamanho da instância (n)")
plt.ylabel("Tempo médio de execução (s)")
plt.title("Tempo médio com IC 95% para cada algoritmo")
plt.legend()
plt.grid(True, linestyle="--", alpha=0.6)
plt.tight_layout()
plt.show()
