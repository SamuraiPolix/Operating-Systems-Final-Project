'''
    Example input:
    Newgraph 10000 10000
    0 1 5
    1 2 7
    2 3 8
    3 4 2
    4 5 10
    5 6 3

    Connected undirected graph, 10000 nodes, 10000 edges, weights randomly generated
    save into a file named 'gprof_input.txt'
'''

import random

def generate_gprof_input(n, m):
    with open('gprof_input.txt', 'w') as f:
        f.write('Newgraph %d 0\n' % (n))
        for i in range(m-2):
            f.write('Newedge %d %d %d\n' % (i, i+1, random.randint(1, 20)))
        f.write('Newedge %d %d %d\n' % (m-2, m-1, random.randint(1, 20)))
        f.write('Prim\n')
        f.write('Boruvka\n')


if __name__ == '__main__':
    generate_gprof_input(5000, 5000)
    print('Done')

