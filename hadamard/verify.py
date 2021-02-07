import sys

L = int(sys.argv[1])
m = (L-1) // 2

alpha = list(map(int, input().split()))

#print("alpha:", list(alpha))

assert len(alpha) >= 2*L

# Reading the solution from assignment
a = []
b = []
for i in range(L):
    a.append(-1 if alpha[i]<0 else 1)
    b.append(-1 if alpha[i+L]<0 else 1)

# Checking the linear constraints
assert (sum(a) == 1), "Linear constraint: a_1 + ... + a_L = 1"
assert (sum(b) == 1), "Linear constraint: b_1 + ... + b_L = 1"

# Checking PAF constraints
for s in range(1, m+1):
    PAF_a = sum([a[i]*a[(i+s)%L] for i in range(L)])
    PAF_b = sum([b[i]*b[(i+s)%L] for i in range(L)])
    assert (PAF_a + PAF_b == -2), f"Quadratic constraint: PAF(a, {s}) + PAF(b, {s}) == -2"

print("All constraints satisfied")
print("A: ", a)
print("B: ", b)
