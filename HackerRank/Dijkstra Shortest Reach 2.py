from heapq import heappush, heappop

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
        tup = (w, n2)
        edge_array[n1].append(tup)
        tup = (w, n1)
        edge_array[n2].append(tup)
    s = input().strip()
    s = int(s)

    distances = [0] * (e+1)
    heap = []
    tup = (0, s)
    heappush(heap, tup)
    first_one = 1

    while not(len(heap) == 0):
        curr = heappop(heap)
        if distances[curr[1]] != 0 and not(first_one == 1):
                continue
        first_one = 0
                
        distances[curr[1]] = curr[0]

        for edge in edge_array[curr[1]]:
            if distances[edge[1]] == 0 and edge[1] != s:
                tup = (edge[0] + distances[curr[1]], edge[1])
                heappush(heap, tup)
                
    for r in range(n+1):
        if r == 0 or r == s:
            continue
        if distances[r] != 0:
            print(str(distances[r]) + " ", end="")
        else:
            print("-1 ", end="")
    print("")