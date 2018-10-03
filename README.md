This repo contains encoding of some problems into Boolean satisfiability
instances.

* `core`: the encoder for the basic operations and other encoders use it.
* `coloring`: Graph coloring problems
    - `total-coloring`: Generates an instance of [Total Coloring][1] of a complete graph
* `crypto`: Cryptographic problems
    - `sha1`, `sha256`: Generate preimage/collision attack instances of [SHA-1][2] and [SHA-256][3]
* `multiplication`: Integer multiplication problems
    - `long`: Long multiplication (elementary) style of a*b
    - `mod`: Modular multiplication of the form (a*b) mod (2^n-delta)

[1]: https://en.wikipedia.org/wiki/Total_coloring
[2]: https://en.wikipedia.org/wiki/SHA-1
[3]: https://en.wikipedia.org/wiki/SHA-2
