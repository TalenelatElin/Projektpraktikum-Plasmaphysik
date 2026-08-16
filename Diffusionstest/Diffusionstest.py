import sys

sys.path.insert(
    0,
    r"C:\Code\Projektpraktikum Plasmaphysik\x64\Release"
)

import Simulator_Pythonmodul

import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

print("Modul:", Simulator_Pythonmodul.__file__)

def fillArray(dateiname):
    ordner = Path(__file__).parent
    print(ordner)
    datei = ordner / "Data" / dateiname
    print("Load Datei:", datei.is_file(), datei)
    #print("Existiert:", datei.exists())
    #print("Ist Datei:", datei.is_file())
    #print("Ordner existiert:", datei.parent.exists())
    print("____")

    return np.loadtxt(datei, usecols=0)

def plott(z, lab):
    num = np.linspace(0,len(z),len(z))
    plt.scatter(num, z, label = lab)
    plt.xlabel("Simulationsschritt")
    plt.ylabel("MSD")
    #plt.yscale('log')
    #plt.xscale('log')
    plt.title("Diffusion der simulierten Enwticklung")
    plt.legend()
    plt.grid()

def plottE(z, y_err, lab):
    num = np.linspace(0,len(z),len(z))
    plt.errorbar(num, z, yerr = y_err, label = lab)
    plt.xlabel("Simulationsschritt")
    plt.ylabel("MSD")
    #plt.yscale('log')
    #plt.xscale('log')
    plt.title("Diffusion der simulierten Enwticklung")
    plt.legend()
    plt.grid()

def plotDiffusion(lab, dateiname):
    z = fillArray(dateiname+".txt")
    # Plot erstellen
    plott(z, lab)

def changeConfigLine(dateiname, li, text):
    pfad = Path("..") / "PlasmaphysikV00" / "Configs" / dateiname
    with open(pfad, "r", encoding="utf-8") as datei:
        zeilen = datei.readlines()

    zeilen[li-1] = text

    with open(pfad, "w", encoding="utf-8") as datei:
        datei.writelines(zeilen)


def simulateAndPlotOften(num, lab, dateiname):
    for i in range(num):
        setFileName(dateiname+str(i))
        Simulator_Pythonmodul.main()
        print("_____")

    for i in range(num):
        plotDiffusion(lab+str(i), dateiname+str(i))
        print("_____")

def plotAverageData(num, lab, dateiname, pddof = 1):
    z = []
    for i in range(num):
        z.append(fillArray(dateiname+str(i)+".txt"))
    daten = np.stack(z)

    mittelwert = np.mean(daten, axis=0)

    standardabweichung = np.std(daten, axis=0, ddof=pddof)

    N = daten.shape[0]
    standardfehler = standardabweichung / np.sqrt(N)

    plottE(mittelwert, standardfehler, lab)

    time = np.linspace(1,len(mittelwert)+1,len(mittelwert))
    diffusion = mittelwert/time
    delta_diffusion = np.abs((standardfehler - diffusion))/time

    print("Diffusionskonstante: ", np.mean(diffusion))
    print("Fehler: ", np.sqrt((np.std(diffusion)**2 + np.mean(delta_diffusion)**2)/np.sqrt(N)))


def setFileName(filename):
    changeConfigLine("Simulation_config.txt",6 ,"dateiname="+filename+".txt\n") # Dateiname
def setNrParticles(N):
    changeConfigLine("Konstanten_config.inc",6 ,f"inline constexpr int N = {N};\n") # Teilchenzahl
def setSimulationLengeth(sim_len):
    changeConfigLine("Simulation_config.txt",9 ,f"sim_len={sim_len}\n") # Simulationslänge
def setNrOfSaves(nr_saves):
    changeConfigLine("Simulation_config.txt",18 ,f"nr_saves={nr_saves}\n") # Anzahl der zu speichernden Zustände


setNrOfSaves(100000)
setNrParticles(1000)
setSimulationLengeth(10000)
setFileName("Diskrete_Braunsche_Bewegung")

("_____")
#Simulator_Pythonmodul.main()
print("_____")


# Konstante Diffusion:
simulateAndPlotOften(10, "dis const diff","Diskrete_Braunsche_Bewegung")
plt.show()
plotAverageData(10, "dis const diff","Diskrete_Braunsche_Bewegung")
plt.show()



