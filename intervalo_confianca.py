from scipy import stats
import numpy as np


diferencas = np.array([0.449001,7.659000,17.311888,6.538544,236.382111,2.328543,0.970678,413.310564,6.623654,0.223675])
media = np.mean(diferencas)
desvio = np.std(diferencas, ddof=1)
n = len(diferencas)
t_critico = stats.t.ppf(1 - 0.05/2, df=n - 1)
erro_margem = t_critico * (desvio / np.sqrt(n))
ic = (media - erro_margem, media + erro_margem)
print(f"Intervalo de confiança 95%: {ic}")
#se intervalo conter 0 => empate estatístico
#se intervalo está totalmente acima de 0 => o segundo algoritmo é melhor
#se intervalo está totalmente abaixo de 0 => o primeiro algoritmo é melhor
# -1.644432,-0.135895,-177.508216,-2.563697,-8.044409,-13.854696
#                        -0.654044,-3.594482,-0.885442,-6.854459