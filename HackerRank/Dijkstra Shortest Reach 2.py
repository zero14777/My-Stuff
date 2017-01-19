from collections import deque
t = input().strip()
t = int(t)
for x in range(t):
    n,e = input().strip().split(' ')
    n,e = [int(n),int(e)]
    edge_array = []
    for i in range ((e+1)):
        edge_array.append([])
    for i in range (e):
        n1, n2, w = input().strip().split(' ')
        n1, n2, w = [int(n1),int(n2),int(w)]
        tup = (n2, w)
        edge_array[n1].append(tup)
        tup = (n1, w)
        edge_array[n2].append(tup)
    s = input().strip()
    s = int(s)

    distances = [0] * (e+1)
    q = []
    tup = (s, 0)
    q.append(tup)
    first_one = 1

    while not(len(q) == 0):
        q.sort(key=lambda tup: tup[1], reverse=True)
        curr = q.pop()
        if distances[curr[0]] != 0 and not(first_one == 1):
                continue
        first_one = 0
                
        distances[curr[0]] = curr[1]

        for edge in edge_array[curr[0]]:
            if distances[edge[0]] == 0 and edge[0] != s:
                tup = (edge[0], edge[1] + distances[curr[0]])
                q.append(tup)
    for r in range(n+1):
        if r == 0 or r == s:
            continue
        if distances[r] != 0:
            print(str(distances[r]) + " ", end="")
        else:
            print("-1 ", end="")
    print("")