import sys


nvar = 0

def newvar():
    global nvar
    nvar += 1
    return nvar

# Ladder encoding for At-Most-One
def amo(x, cut=3):
    if len(x) <= cut:
        r = [[-x[i], -x[j]] for i in range(len(x)) for j in range(i)]
        return r #[[-x[0], -x[1]], [-x[0], -x[2]], [-x[1], -x[2]]]
    y = newvar()
    return amo(x[:(cut-1)]+[-y], cut) + amo([y]+x[(cut-1):], cut)

# Sequential counter encoding for At-Most-K
def amk(x, k):
    if k == 1:
        return amo(x, 5)

    n = len(x)
    R = [[newvar() for j in range(k)] for i in range(n-1)]
    f = []

    for i in range(n-1):
        f.append([-x[i], R[i][0]])

    for j in range(1, k):
        f.append([-R[0][j]])
    
    for i in range(1, n-1):
        for j in range(0, k):
            f.append([-R[i-1][j], R[i][j]])

    for i in range(1, n-1):
        for j in range(1, k):
            f.append([-x[i], -R[i-1][j-1], R[i][j]])
    
    for i in range(1, n):
        f.append([-x[i], -R[i-1][k-1]])

    return f

# At-Least-K
def alk(x, k):
    if k == 1:
        return x
    y = [-v for v in x]
    return amk(y, len(x) - k)

# Exactly-K: At-Least-K && At-Most-K
def ek(x, k):
    f1 = amk(x, k)
    f2 = alk(x, k)
    return f1 + f2

# CNF for: z <-> xnor(x, y)
def xnor(z, x, y):
    return [
            [z, -x, -y],
            [-z, -x, y],
            [-z, x, -y],
            [z, x, y],
    ]

# Periodic Autocorrelation Function
def PAF(formula, x, s):
    n = len(x)
    p = []
    for i in range(n):
        z = newvar()
        p.append(z)
        formula += xnor(z, x[i], x[(i+s) % n])
    return p, formula


# Print out the formula in DIMACS format
def dimacs(formula):
    print("p cnf", nvar, len(formula))
    for clause in formula:
        print(" ".join(map(str,clause)), 0)


### main part

L =  int(sys.argv[1])
m = (L-1) // 2

a = [newvar() for i in range(L)]
b = [newvar() for i in range(L)]

formula = []

formula += ek(a, m+1)
formula += ek(b, m+1)

for s in range(1, m+1):
    p_a, formula = PAF(formula, a, s)
    p_b, formula = PAF(formula, b, s)
    formula += ek(p_a + p_b, L-1)

dimacs(formula)


