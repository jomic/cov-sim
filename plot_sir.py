import matplotlib.pyplot as plt
from operator import itemgetter

def read_data():
    f = open("data.txt", "r")
    data_points = f.readlines()

    new_data = []
    for data_point in data_points:
        new_data.append(list(map(int, data_point.split())))
    return new_data

def main():

    data = read_data()

    s = list( map(itemgetter(0), data ))
    a = list( map(itemgetter(1), data ))
    i = list( map(itemgetter(2), data ))
    v = list( map(itemgetter(3), data ))
    r = list( map(itemgetter(4), data ))

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
