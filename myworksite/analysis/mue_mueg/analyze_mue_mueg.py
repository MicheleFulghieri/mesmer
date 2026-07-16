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
    except Exception as e:
        print(f"[ERROR] Impossibile analyse {filename}: {e}")


# ---- Analyze aco_oal_100.txt ----

# codice per estrarre questo out dalla lista