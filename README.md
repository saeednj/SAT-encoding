# SAT Encoding

Each folder contains codes for encoding some problems into Boolean satisfiability
instances. The generated instances are in DIMACS format which is the default input format
of modern SAT solvers.

* `core`: the encoder for the basic operations and other encoders use it.
* `coloring`: Graph coloring problems
    - `total-coloring`: Generates an instance of [Total Coloring][1] of a complete graph with the
      conjectured upper limit.
* `crypto`: Cryptographic problems
    - `sha1`, `sha256`: Generate preimage/collision attack instances of [SHA-1][2] and [SHA-256][3].
* `arith`: Integer arithmetic problems
    - `long`: Long multiplication (elementary) style of a*b
    - `mod`: Modular multiplication of the form (a*b) mod (2^n-delta)
    - `ppa`: Parallel prefix addition

[1]: https://en.wikipedia.org/wiki/Total_coloring
[2]: https://en.wikipedia.org/wiki/SHA-1
[3]: https://en.wikipedia.org/wiki/SHA-2
