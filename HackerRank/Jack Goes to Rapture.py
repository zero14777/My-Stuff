from collections import deque

n,e = input().strip().split(' ')
n,e = [int(n),int(e)]
edge_array = []
distances = []
for i in range ((e+1)):
    edge_array.append([])
for i in range ((e+1)):
    distances.append(0)
for i in range (e):
    n1, n2, w = input().strip().split(' ')
    n1, n2, w = [int(n1),int(n2),int(w)]
    tup = (n2, w)
    edge_array[n1].append(tup)
    tup = (n1, w)
    edge_array[n2].append(tup)

q = []
tup = (1, 0)
q.append(tup)
graveyard = []

while not(len(q) == 0):
    q.sort(key=lambda tup: tup[1], reverse=True)
    curr = q.pop()
    if graveyard.count(curr[0]) != 0:
            continue
    graveyard.append(curr[0])
    
    if curr[0] == n:
        distances[curr[0]] = curr[1]
        break
    else:
        distances[curr[0]] = curr[1]
    
    for edge in edge_array[curr[0]]:
        if graveyard.count(edge[0]) == 0:
            tup = (edge[0], max(0, edge[1] - distances[curr[0]]) + distances[curr[0]])
            q.append(tup)

if distances[n] != 0:
    print(distances[n])
else:
    print("NO PATH EXISTS")