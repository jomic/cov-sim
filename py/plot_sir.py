#!/usr/bin/env python

import matplotlib.pyplot as plt
from operator import itemgetter
import json
import sys

def read_data():
    f = open("src/data.txt", "r")
    data_points = f.readlines()

    new_data = []
    for data_point in data_points:
        new_data.append(list(map(int, data_point.split())))
    return new_data

def read_json_stdin():
    inp = sys.stdin.readlines()[0]
    data_json = json.loads(inp)
    return data_json['results']
    

def main():

    data = read_json_stdin()

    s = list( map(itemgetter('s'), data ))
    a = list( map(itemgetter('a'), data ))
    i = list( map(itemgetter('i'), data ))
    v = list( map(itemgetter('v'), data ))
    r = list( map(itemgetter('r'), data ))

    t = [i for i in range(len(data))]
    
    fig, ax = plt.subplots()
    ax.plot(t, s, color='b')
    ax.plot(t, a, color='yellow')
    ax.plot(t, i, color='r')
    ax.plot(t, v, color='magenta')
    ax.plot(t, r, color='g')
    ax.set_xlabel('t (days)')  # Add an x-label to the axes.
    ax.set_ylabel('population')  # Add a y-label to the axes.
    ax.set_title("SAIVR Model")  # Add a title to the axes.
    ax.legend(['Susceptible', 'Asymptomatic', 'Infectious', 'Vaccinated','Removed'])  # Add a legend.
    plt.show()

if __name__ == "__main__":
    main()
