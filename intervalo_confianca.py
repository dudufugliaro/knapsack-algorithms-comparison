from scipy import stats
import numpy as np


diferencas = np.array([])
media = np.mean(diferencas)
desvio = np.std(diferencas, ddof=1)
n = len(diferencas)
t_critico = stats.t.ppf(1 - 0.1/2, df=n - 1)
erro_margem = t_critico * (desvio / np.sqrt(n))
ic = (media - erro_margem, media + erro_margem)
print(f"Intervalo de confiança 95%: {ic}")
#se intervalo conter 0 => empate estatístico
#se intervalo está totalmente acima de 0 => o segundo algoritmo é melhor
#se intervalo está totalmente abaixo de 0 => o primeiro algoritmo é melhor