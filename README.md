# qulacs-bridge

"medium" level bindings to qulacs cppsim using cxxbridge.

## Usage

This library requires that QULACS_PATH is set and points at the direction of a qulacs install.

Additionally you will need to include a `build.rs` in your project that provides the -fopenmp
flag to your linker.

```rust
fn main() {
    println!("cargo:rustc-link-arg=-fopenmp");
}
```

## Sample Code

```rust
let state = new_quantum_state(3, false);
set_haar_random_state(&state);

let circuit = new_quantum_circuit(3);
add_x_gate(&circuit, 0);
let merged_gate = merge(&new_cnot_gate(0, 1), &new_y_gate(1));
add_gate_copy(&circuit, &merged_gate);
add_r_x_gate(&circuit, 1, 0.5);
update_quantum_state(&circuit, &state, 1);

let observable = new_observable(3);
add_operator(&observable, 2.0.into(), "X 2 Y 1 Z 0");
add_operator(&observable, (-3.0).into(), "Z 2");
let value = get_expectation_value(&observable, &state);
```

## Development

The recommended way to develop is using [devenv.sh](https://devenv.sh/) and [direnv](https://direnv.net/).

With devenv and direnv installed you can simply allow the enviroment and then run

```
  cargo build
```

Or similar cargo commands without issue.

If you do not wish to use devenv you can instead set the QULACS_PATH environment variable to the
location of a qulacs install that contains a built copy of the library.
