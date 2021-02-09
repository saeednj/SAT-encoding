import sys

LTE = "<="
GTE = ">="

nvar = 0
def newvar():
    global nvar
    nvar += 1
    return nvar

# CNF for: z <-> xnor(x, y)
def xnor(z, x, y):
    return [
            [z, -x, -y],
            [-z, -x, y],
            [-z, x, -y],
            [z, x, y],
    ]

# Periodic Autocorrelation Function
def PAF(cnf, x, s):
    n = len(x)
    p = []
    for i in range(n):
        z = newvar()
        p.append(z)
        cnf += xnor(z, x[i], x[(i+s) % n])
    return cnf, p

def print_formula(cnf, pb):
    print("p cnf", nvar, len(cnf)+len(pb))
    for clause in cnf:
        print(" ".join(map(str,clause)), 0)
    for clause in pb:
        var = clause[0]
        pred = clause[1]
        bound = clause[2]
        dest_lit = clause[3]
        print(" ".join(map(str,var)), pred, bound, "#", dest_lit)



### main part

L = int(sys.argv[1])
m = (L - 1) // 2

cnf = []
pb = []

a = [newvar() for i in range(L)]
b = [newvar() for i in range(L)]

a_lte = newvar()
a_gte = newvar()
b_lte = newvar()
b_gte = newvar()

pb += [[a, LTE, m+1, a_lte], [a, GTE, m+1, a_gte]]
pb += [[b, LTE, m+1, b_lte], [b, GTE, m+1, b_gte]]
cnf += [[a_lte], [a_gte], [b_lte], [b_gte]]

for s in range(1, m+1):
    cnf, p_a = PAF(cnf, a, s)
    cnf, p_b = PAF(cnf, b, s)
    p = p_a + p_b

    p_lte = newvar()
    p_gte = newvar()
    pb += [[p, LTE, L-1, p_lte], [p, GTE, L-1, p_gte]]
    cnf += [[p_lte], [p_gte]]


print_formula(cnf, pb)
