import sys

n = int(sys.argv[1])
k = 10

nvar = 0

def amo(x, k):
    if len(x) <= k:
        r = [[-x[i], -x[j]] for i in range(len(x)) for j in range(i)]
        return r #[[-x[0], -x[1]], [-x[0], -x[2]], [-x[1], -x[2]]]
    y = newvar()
    return amo(x[:(k-1)]+[-y], k) + amo([y]+x[(k-1):], k)


def newvar():
    global nvar
    nvar += 1
    return nvar

v = [range(1+t*(n-1), n+t*(n-1)) for t in range(n)]
nvar = n*(n-1)

f = []

for i in range(n):
    f.append(v[i])

for j in range(n-1):
    x = [v[i][j] for i in range(n)]
    f = f + amo(x, k)

print "p cnf", nvar, len(f)
for c in f:
    print " ".join(map(str,c)), 0
