import os
import pandas as pd
import matplotlib.pyplot as plt

# Percorso assoluto sul server
data_dir = "/data/mfulghieri/mesmer/test-run/"
file_name = "eenlab_oal_100.txt"
input_path = os.path.join(data_dir, file_name)

print(f"Sto leggendo i dati da: {input_path}")

# Carica il file a 4 colonne
df = pd.read_csv(input_path, sep=r'\s+', comment='#', header=None)

# Rinomina le 4 colonne reali del file
df.columns = ['E_electron', 'Cross_Section', 'Stat_Error', 'Alt_Error']

# Creazione del plot quantistico
plt.figure(figsize=(10, 6))
plt.errorbar(
    df['E_electron'], 
    df['Cross_Section'], 
    yerr=df['Stat_Error'], 
    fmt='o-', 
    color='darkblue',
    ecolor='crimson',
    elinewidth=1,
    capsize=2,
    markersize=4, 
    label=r'MESMER $\mathcal{O}(\alpha)$ Simulation'
)

# Configurazione estetica del grafico (scala logaritmica sull'asse Y consigliata per lo scattering)
plt.yscale('log')
plt.title("MUonE Simulation: Electron Energy Distribution in LAB", fontsize=14, fontweight='bold')
plt.xlabel("Electron Energy $E_e$ (GeV)", fontsize=12)
plt.ylabel(r"Differential Cross Section $d\sigma/dE_e$ (a.u.)", fontsize=12)
plt.grid(True, which="both", linestyle='--', alpha=0.5)
plt.legend(fontsize=12)

# Salva il grafico
os.makedirs("plots", exist_ok=True)
output_plot = "plots/eenlab_distribution.png"
plt.savefig(output_plot, dpi=300, bbox_inches='tight')
print(f"Grafico generato con successo in: {output_plot}")

