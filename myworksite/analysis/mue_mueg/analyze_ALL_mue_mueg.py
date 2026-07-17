import matplotlib.pyplot as plt
import matplotlib
import os
import glob
matplotlib.use('Agg')   
import numpy as np


# ---- Useful paths ----
data_dir = "/data/mfulghieri/mesmer/outputs/first_test"
save_path = "/data/mfulghieri/mesmer/myworksite/analysis/mue_mueg/outputs"

os.makedirs(save_path, exist_ok=True)   


# ---- Spot all the Mesmer distribution files ----
file_pattern = os.path.join(data_dir, "*.txt") # .txt to avoid logs
file_paths   = glob.glob(file_pattern)         # collect all

print(f"\nFound {len(file_paths)} distribution files to be analyzed.\n")


# ---- Anaylize all the outputs distributions ----
for path in file_paths:
    filename = os.path.basename(path)                # extract filename (last comp of the path)
    var_name = filename.replace("_oal_100.txt", "")  # remove the suffix, lefting the physical variable name
    
    print(f"Analysing {var_name}...")
    
    try:
        x_bin, cross_section, stat_error, alt_error = np.loadtxt(  # load the file with numpy
            path, 
            comments=('#', '*'), # skip # and *
            unpack=True          # invert the matrix to assign the columns
        )
        
        plt.figure(figsize=(10, 6))
        
        plt.errorbar(
            x_bin, 
            cross_section, 
            yerr=stat_error, 
            fmt='o-', 
            color='teal',
            ecolor='orange',
            elinewidth=1,
            capsize=2,
            markersize=3,
            label=f'MESMER {var_name}'
        )
        
        plt.yscale('log')  # log scale for cross sections
        plt.title(f"Differential Distribution: {var_name}", fontsize=14, fontweight='bold')
        plt.xlabel(f"{var_name}", fontsize=12)
        plt.ylabel(r"Differential cross section", fontsize=12)
        plt.grid(True, which="both", linestyle='--', alpha=0.5)
        plt.legend(fontsize=12)
        
        plot_filename = f"plot_{var_name}.png"
        plot_save_path = os.path.join(save_path, plot_filename)
        plt.savefig(plot_save_path, dpi=200, bbox_inches='tight')
        plt.close()  # free the ra,
        
    except Exception as e:
        print(f"[ERROR] Impossibile to analyse {filename}: {e}")

print("\nAll done/'.")






###### INSPIRATIONS ############à

# # 1. Carica il file specificando il percorso corretto sull'HPC
# # np.loadtxt gestisce automaticamente le 4 colonne di testo
# data = np.loadtxt("/data/mfulghieri/mesmer/outputs/first_test/aco_oal_100.txt")

# # 2. Mappatura delle colonne secondo la struttura di MESMER
# x_values = data[:, 0]  # Colonna 1: Acoplanarità (mrad)
# y_values = data[:, 1]  # Colonna 2: dSigma/dAco (Sezione d'urto differenziale)
# y_errors = data[:, 2]  # Colonna 3: Errore statistico (puoi usare la colonna 2 o 3, sono simmetrici)

# # 3. Creazione del grafico
# plt.figure(figsize=(8, 6))

# # Usiamo un grafico a linee con i marker (o barre) visto che x_values rappresenta i punti centrali dei bin
# plt.errorbar(
#     x_values,
#     y_values,
#     yerr=y_errors,
#     fmt="o-",
#     color="darkblue",
#     ecolor="red",
#     markersize=4,
#     capsize=3,
#     label="Dati MESMER",
# )

# # 4. Configurazione degli assi e dei titoli
# plt.xlabel("Acoplanarità |$\pi$ - |$\phi_e$ - $\phi_\mu$|| (mrad)")
# plt.ylabel("$d\sigma / d\phi$ (Sezione d'urto differenziale per bin)")
# plt.title("Distribuzione dell'Acoplanarità in MESMER (Setup 1)")

# # Consiglio dell'occhio clinico: poiché il primo bin è enorme (7.729) 
# # e gli altri crollano subito a 10^-3, la scala LOGARITMICA sull'asse Y 
# # è indispensabile per vedere la forma del grafico!
# plt.yscale("log")

# plt.grid(True, which="both", linestyle="--", alpha=0.5)
# plt.legend()

# # 5. Salvataggio del file 
# plt.tight_layout()
# plt.savefig("/data/mfulghieri/mesmer/myworksite/analysis/mue_mueg/outputs/acoplanarita_mesmer.png", dpi=300)
# plt.close()




###################
