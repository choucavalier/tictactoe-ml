#!/bin/env python

import matplotlib.pyplot as plt
import os
import sys

if __name__ == "__main__":
    if (len(sys.argv) < 2):
        print("Please provide the path to the file containing the data")
        sys.exit(0)
    path = os.getcwd() + "/" + sys.argv[1]
    data = {
        'generation': [],
        'max_fitness': [],
        'min_fitness': [],
        'mean_fitness': [],
    }
    with open(path) as file:
        while True:
            line = file.readline()
            if not line:
                break
            data['generation'].append(int(line))
            data['max_fitness'].append(float(file.readline()))
            data['min_fitness'].append(float(file.readline()))
            data['mean_fitness'].append(float(file.readline()))

    plt.plot(data['max_fitness'])
    plt.xlabel('Generation')
    plt.ylabel('Max fitness')
    plt.title('Evolution of max fitness of the population')
    #plt.axis([40, 160, 0, 0.03])
    plt.grid(True)
    plt.show()

