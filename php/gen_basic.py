import sys

n = int(sys.argv[1])

v = [range(1+t*(n-1), n+t*(n-1)) for t in range(n)]
#print v
f = []

for i in range(n):
    f.append(v[i])

for j in range(n-1):
    for i in range(n):
        for k in range(i+1, n):
            f.append([-v[i][j], -v[k][j]])

print "p cnf", n*(n-1), len(f)
for c in f:
    print " ".join(map(str,c)), 0
