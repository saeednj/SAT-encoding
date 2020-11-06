import sys
import subprocess

n = int(sys.argv[1])
cache = {}
nvar = 0

def amo_esp(v):
    """
    passes truth table of AMO(v) to espresso and gets the result in CNF
    """
    global cache
    m = len(v)
    if m in cache:
        cls = cache[len(v)]
    else:
        esp = subprocess.Popen(["espresso"], stdin=subprocess.PIPE, stdout=subprocess.PIPE)

        inp = ".i {0}\n.o 1\n".format(m)

        for x in range(1 << m):
            r = "{0:b}".format(x).zfill(m)
            s = sum([1 - int(d) for d in r])
            o = '1' if s > 1 else '0'
            inp += r + " " + o + "\n"

        inp += ".e\n"

        out, err = esp.communicate(inp)

        out = out.splitlines()[3:-1]
        cls = []
        for x in out:
            c = []
            for i in range(m):
                if x[i] == '1':
                    c.append(i+1)
                elif x[i] == '0':
                    c.append(-(i+1))
            cls.append(c)

        cache[m] = cls

    r = []
    for c in cls:
        cl = []
        for x in c:
            idx = abs(x) - 1
            var = v[idx]
            cl.append(var if x>0 else -var)
        r.append(cl)
    return r


def newvar():
    global nvar
    nvar += 1
    return nvar

def amo(v):
    if len(v) > 10:
        y = newvar()
        return amo(v[:9] + [-y]) + amo([y] + v[9:])
    return amo_esp(v)

#print amo([1, 2, 3, 4, 5])
#print amo([10, 20, 30, 40])

for i in range(3, 13):
    a = amo(range(1, i+1))
    print i, len(a)#, a
exit()

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


