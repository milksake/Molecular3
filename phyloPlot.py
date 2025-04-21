from Bio import Phylo
from io import StringIO
import matplotlib.pyplot as plt
import sys

def plotNewick(newick_str):
    tree = Phylo.read(StringIO(newick_str.strip()), "newick")
    Phylo.draw(tree, branch_labels=lambda c: c.branch_length)

if __name__ == "__main__":
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        print(f"\nPlotting: {line}")
        plotNewick(line)
