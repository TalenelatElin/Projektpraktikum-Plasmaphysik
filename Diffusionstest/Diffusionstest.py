import sys

sys.path.insert(
    0,
    r"C:\Code\Projektpraktikum Plasmaphysik\x64\Release"
)

import Simulator_Pythonmodul

import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path




def fillArray(dateiname):
    ordner = Path(__file__).parent
    print(ordner)
    datei = ordner / "Data" / dateiname
    print("Load Datei:", datei)
    print("Existiert:", datei.exists())
    print("Ist Datei:", datei.is_file())
    print("Ordner existiert:", datei.parent.exists())
    print("____")

    return np.loadtxt(datei, usecols=0)

def plotDiffusion(lab, dateiname):
    zahlen = fillArray(dateiname)

    # Plot erstellen
    plt.plot(zahlen, label = lab)
    plt.xlabel("Simulationsschritt")
    plt.ylabel("MSD")
    plt.yscale('log')
    plt.xscale('log')
    plt.title("Diffusion der simulierten Enwticklung")
    plt.legend()
    plt.grid()


def changeConfigLine(dateiname, li, text):
    pfad = Path("..") / "PlasmaphysikV00" / "Configs" / dateiname
    with open(pfad, "r", encoding="utf-8") as datei:
        zeilen = datei.readlines()

    zeilen[li-1] = text

    with open(pfad, "w", encoding="utf-8") as datei:
        datei.writelines(zeilen)




# Funktioniert nicht, da das Modul nicht so dynamisch ist?
changeConfigLine("Simulation_config.txt",6,"dateiname=test_test.txt\n")


print("Modul:", Simulator_Pythonmodul.__file__)


print("_____")
Simulator_Pythonmodul.main()
print("_____")

# Konstante Diffusion:
plotDiffusion("const diff 0", "neutron_radial_approx.txt")
plotDiffusion("const diff 1", "test10_10.txt")

plt.show()
