# Encoding Cryptanalysis of hash functions into SAT

This folder implements encoding of cryptanalysis of cryptographic hash functions
into SAT.

Three standard hash functions of MD4, SHA-1 and SHA-256 are supported. The main
implemented attack is preimage (given output of a hash function find an input
that hashes to that output value).

## Build and Run
To build the encoder, run `make`, which builds the executable `main`. For
available encoding options you can run `./main -h`.  You can specify the type
of function, number of rounds for a round-reduced version of your function.
For example:

```
./main --function sha256 --rounds 20 --target random > sha256-20-preimage.cnf
```

generates a preimage instance for a 20 round SHA-256 with a randomly generated
target output. You can also specify the type of adder encodings, which is
implemented in the `core` encoder. Note that using `espresso` adders requires
having `espresso` logic minimizer installed.

## Verify
To verify that the solution found by a SAT solver actually hashes to
the target, you can use vrifiers for each hash function. First, to build them,
run `make verify`. For example, consider that you solved the above created
instance and recorded the solution in `sha256-20-preimage.sol`. To verify the
solution, you can run:


```
./verify-sha256 20 < sha256-20-preimage.sol
```

which will print the solution and whether it hashes to the initially given
output target. The first argument is the number of rounds, which is a required
argument. The solution file is expected to be in the minisat-style format
(first line being `SAT` or `UNSAT`, and the next line the assignment to the
variables, e.g. `-1 2 3 -4 ...`).

