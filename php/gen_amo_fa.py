import sys

n = int(sys.argv[1])

nvar = 0

def newvar():
    global nvar
    nvar += 1
    return nvar

def amo(x):
    if len(x) == 2:
        return [[-x[0], -x[1]]]
    if len(x) == 3:
        return [[-x[0], -x[1]], [-x[0], -x[2]], [-x[1], -x[2]]]
    y = newvar()
    return amo(x[:3]) + [[y, -x[0]], [y, -x[1]], [y, -x[2]], [-y, x[0], x[1], x[2]]] + amo(x[3:] + [y])

v = [range(1+t*(n-1), n+t*(n-1)) for t in range(n)]
nvar = n*(n-1)

f = []
for i in range(n):
    f.append(v[i])

for j in range(n-1):
    x = [v[i][j] for i in range(n)]
    f = f + amo(x)

print "p cnf", nvar, len(f)
for c in f:
    print " ".join(map(str,c)), 0
