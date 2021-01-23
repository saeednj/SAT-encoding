# SAT Encoding

Each folder contains codes for encoding some problems into Boolean satisfiability
instances. The generated instances are in DIMACS format which is the default input format
of modern SAT solvers.

The following is a high-level description of each folder, for more detailed description check out
the README in each folder. To make each of the encoders, run the `Makefile` within that folder.

* `core`: the encoder for the basic operations. Other encoders use it.
* `graph`: Graph problems
    - `total-coloring`: Generates an instance of [Total Coloring][1] of a complete graph with the
      conjectured upper limit.
* `crypto`: Cryptographic problems
    - `sha1`, `sha256`, `md4`: Generate preimage/collision attack instances of [MD4][4], [SHA-1][2] and [SHA-256][3].
* `arith`: Integer arithmetic problems
    - `long`: Long multiplication (elementary) style of a*b
    - `mod`: Modular multiplication of the form (a*b) mod (2^n-delta)
    - `ppa`: Parallel prefix addition

[1]: https://en.wikipedia.org/wiki/Total_coloring
[2]: https://en.wikipedia.org/wiki/SHA-1
[3]: https://en.wikipedia.org/wiki/SHA-2
[4]: https://en.wikipedia.org/wiki/MD4
