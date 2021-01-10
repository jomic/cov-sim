from json import dumps
from sys import argv, stdout
from fileinput import input


def get_offsets_and_neighbors():
    offset_line = ""
    neighbor_line = ""
    line_no = 0
    for line in input():
        if line_no == 1:
            offset_line = line
        elif line_no == 2:
            neighbor_line = line
        elif line_no > 2:
            break
        line_no += 1
    offsets = [int(i) for i in offset_line.split(" ")]
    neighbors = [int(i) for i in neighbor_line.split(" ")]
    return (offsets, neighbors)

def offsets_and_neighbors_to_graph(offsets, neighbors):
    graph = {"nodes": [], "links": []}
    for i, offset in enumerate(offsets):
        if i == len(offsets) - 1:
            end = len(neighbors)
        else:
            end = offsets[i + 1]
        targets = neighbors[offset:end]
        graph["nodes"].append({"id": i, "state": 0})
        for target in targets:
            graph["links"].append({"source": i, "target": target, "value": 1})
    return graph

def write_dict_as_json_to_stdout(output):
    output_json = dumps(output)
    print(output_json, file = stdout)

def main():
    (offsets, neighbors) = get_offsets_and_neighbors()
    graph = offsets_and_neighbors_to_graph(offsets, neighbors)
    write_dict_as_json_to_stdout(graph)

main()
