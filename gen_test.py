from random import randint
from queue import Queue
import tempfile
import os

def gen_test_case(filename, V, source, sink, cap_min, cap_max):
    E = 0
    s = set()
    node_set = set()

    q = Queue()
    q.put(source)
    node_set.add(source)

    rows = []


    with open(filename, 'w') as f:
        while len(node_set) < V:
            u = q.get()
            for _ in range(randint(1, V // 3)):
                v = randint(0, V - 1)
                while v == source or (u, v) in s or (v, u) in s:
                    v = randint(0, V - 1)

                node_set.add(v)
                E += 1
                rows.append((u, v, randint(cap_min, cap_max)))
                s.add((u, v))
                s.add((v, u))
                q.put(v)

        print(V, E, source, sink, file=f)
        for row in rows:
            print(row[0], row[1], row[2], file=f)


n_verticies = [100, 200, 300, 400, 500, 600, 700, 800, 1000]
num_tests = 10

for V in n_verticies:
    for i in range(num_tests):
        try:
            os.makedirs(f'input/{V}')
        except FileExistsError:
            pass
        gen_test_case(f'input/{V}/test{i}.in', V, 0, V - 1, 10, 100)

