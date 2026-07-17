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


# ---- Load all the outputs distributions ----
distrib_dic = {}    

for path in file_paths:
    filename = os.path.basename(path)                # extract filename (last comp of the path)
    var_name = filename.replace("_oal_100.txt", "")  # remove the suffix, lefting the physical variable name
    
    distrib_dic[var_name] = path    


# ---- Analyze aco_oal_100.txt ----
aco_oal_path = distrib_dic["aco"]

try:
    x_bin, cross_section, stat_error, tot_error = np.loadtxt(  # load the file with numpy
        aco_oal_path, 
        comments=('#', '*'), # skip # and *
        unpack=True          # invert the matrix to assign the columns
    )
    print(f"Analysing {aco_oal_path} distribution...")
except Exception as e:
    print(f"[ERROR] Impossibile to analyse: {e}")

# total_error = np.sqrt(stat_error**2 + tot_error**2)   # stat + sys quadrate sum

fig_aco, ax_aco = plt.subplots(figsize=(8, 6))
ax_aco.errorbar(
        x_bin, 
        cross_section, 
        yerr=tot_error,       # y error bars
        fmt='o',                # single points
        markersize=4, 
        capsize=3,              # orizontal wings for the err bars
        label=r'$e - \mu$ acollinearity', 
        linewidth=1.5
    )

ax_aco.set_xlabel(r'Acollinearity angle $\Delta\theta$ [rad o deg]', fontsize=11)
ax_aco.set_ylabel(r'Differential cross section $d\sigma/d\Delta\theta$', fontsize=11)
ax_aco.set_yscale('log')
ax_aco.set_title('Differential Cross Section: Acollinearity', fontsize=14, fontweight='bold')
ax_aco.legend()
ax_aco.tick_params(direction='in', which='both')     
ax_aco.grid(True, linestyle='--', alpha=0.5)

plt.tight_layout()
filename = os.path.basename(aco_oal_path).replace('.txt', '.png')
plt.savefig(os.path.join(save_path, f"plot_{filename}"), dpi=300)
plt.close()



# acollinearità: quanto e- e mu back to back (mom trasverso con nullo)

# col 1: bin in rad
# col 2: sdu diff (prob mubarn)
# col 3: errore stat (probab dal MC dalla varianza dei pesi)
# col 4: ? err syst o err tot??

