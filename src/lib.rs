pub use cxx::UniquePtr;
use ffi::Complex;

#[cxx::bridge]
pub mod ffi {
    #[derive(Debug, PartialEq)]
    struct Complex {
        real: f64,
        imag: f64,
    }

    unsafe extern "C++" {
        include!("qulacs-bridge/include/qulacs-bridge.h");

        pub type QuantumStateBase;

        pub fn new_quantum_state(qubit_count: u32, multi_cpu: bool) -> UniquePtr<QuantumStateBase>;
        pub fn quantum_state_sampling(
            state: &UniquePtr<QuantumStateBase>,
            sampling_count: u32,
            seed: u32,
        ) -> Vec<u64>;

        pub type QuantumCircuit;
        pub fn new_quantum_circuit(qubit_count: u32) -> UniquePtr<QuantumCircuit>;
        pub fn update_quantum_state(
            circuit: &UniquePtr<QuantumCircuit>,
            state: &UniquePtr<QuantumStateBase>,
        );
        pub fn add_h_gate(circuit: &UniquePtr<QuantumCircuit>, index: u32);
        pub fn add_x_gate(circuit: &UniquePtr<QuantumCircuit>, index: u32);
        pub fn add_y_gate(circuit: &UniquePtr<QuantumCircuit>, index: u32);
        pub fn add_z_gate(circuit: &UniquePtr<QuantumCircuit>, index: u32);
        pub fn add_r_x_gate(circuit: &UniquePtr<QuantumCircuit>, index: u32, angle: f64);
        pub fn add_r_y_gate(circuit: &UniquePtr<QuantumCircuit>, index: u32, angle: f64);
        pub fn add_r_z_gate(circuit: &UniquePtr<QuantumCircuit>, index: u32, angle: f64);
        pub fn add_gate_copy(
            circuit: &UniquePtr<QuantumCircuit>,
            gate: &UniquePtr<QuantumGateBase>,
        );

        pub type QuantumGateBase;
        pub type ClsOneQubitGate;

        pub fn new_identity_gate(index: u32) -> UniquePtr<QuantumGateBase>;
        pub fn new_h_gate(index: u32) -> UniquePtr<QuantumGateBase>;
        pub fn new_x_gate(index: u32) -> UniquePtr<QuantumGateBase>;
        pub fn new_y_gate(index: u32) -> UniquePtr<QuantumGateBase>;
        pub fn new_z_gate(index: u32) -> UniquePtr<QuantumGateBase>;
        pub fn new_r_x_gate(index: u32, angle: f64) -> UniquePtr<QuantumGateBase>;
        pub fn new_r_y_gate(index: u32, angle: f64) -> UniquePtr<QuantumGateBase>;
        pub fn new_r_z_gate(index: u32, angle: f64) -> UniquePtr<QuantumGateBase>;
        pub fn new_cnot_gate(control: u32, target: u32) -> UniquePtr<QuantumGateBase>;
        pub fn new_diagonal_matrix_gate(
            target_qubits: &[u32],
            elements: &[Complex],
        ) -> UniquePtr<QuantumGateBase>;
        pub fn new_measurement(index: u32, reg: u32, seed: u32) -> UniquePtr<QuantumGateBase>;

        pub fn merge(
            applied_first: &UniquePtr<QuantumGateBase>,
            applied_later: &UniquePtr<QuantumGateBase>,
        ) -> UniquePtr<QuantumGateBase>;

        pub type Observable;
        pub fn new_observable(qubit_count: u32) -> UniquePtr<Observable>;
        pub fn add_operator(observable: &UniquePtr<Observable>, coef: Complex, pauli_string: &str);
        pub fn get_expectation_value(
            observable: &UniquePtr<Observable>,
            state: &UniquePtr<QuantumStateBase>,
        ) -> Complex;

    }
}

impl From<f64> for Complex {
    fn from(value: f64) -> Self {
        Self {
            real: value,
            imag: 0.0,
        }
    }
}

#[cfg(feature = "num")]
impl From<Complex> for num::Complex<f64> {
    fn from(value: Complex) -> Self {
        num::Complex {
            re: value.real,
            im: value.imag,
        }
    }
}

#[cfg(feature = "num")]
impl From<num::Complex<f64>> for Complex {
    fn from(value: num::Complex<f64>) -> Self {
        Complex {
            real: value.re,
            imag: value.im,
        }
    }
}

#[cfg(test)]
mod tests {
    use rand::Rng;

    use crate::ffi::*;

    #[test]
    fn basic_example() {
        let state = new_quantum_state(10, false);
        let circuit = new_quantum_circuit(10);
        let observable = new_observable(10);

        add_x_gate(&circuit, 0);
        update_quantum_state(&circuit, &state);

        let value = get_expectation_value(&observable, &state);
        println!("{:?}", value);
    }

    #[test]
    fn first_class_gates() {
        let state = new_quantum_state(10, false);
        let circuit = new_quantum_circuit(10);
        let observable = new_observable(10);

        let gate = new_x_gate(0);
        add_gate_copy(&circuit, &gate);
        update_quantum_state(&circuit, &state);

        let value = get_expectation_value(&observable, &state);
        println!("{:?}", value);
    }

    #[test]
    fn merging_gates() {
        let state = new_quantum_state(10, false);
        let circuit = new_quantum_circuit(10);
        let observable = new_observable(10);

        // My cool error mitigation strategy
        let gate_1 = new_x_gate(0);
        let gate_2 = new_x_gate(0);
        let merged_gate = merge(&gate_1, &gate_2);
        add_gate_copy(&circuit, &merged_gate);
        update_quantum_state(&circuit, &state);

        let value = get_expectation_value(&observable, &state);
        println!("{:?}", value);
    }

    #[test]
    fn double_merging_gates() {
        let state = new_quantum_state(10, false);
        let circuit = new_quantum_circuit(10);
        let observable = new_observable(10);

        // Making a PhasedX gate
        let gate_1 = new_r_z_gate(0, 0.5);
        let gate_2 = new_r_x_gate(0, 0.3);
        let gate_3 = new_r_z_gate(0, -0.5);
        let merged_gate_1 = merge(&gate_1, &gate_2);
        let merged_gate_2 = merge(&merged_gate_1, &gate_3);
        add_gate_copy(&circuit, &merged_gate_2);
        update_quantum_state(&circuit, &state);

        let value = get_expectation_value(&observable, &state);
        println!("{:?}", value);
    }

    #[test]
    fn diagonal_matrix_gate() {
        let state = new_quantum_state(10, false);
        let circuit = new_quantum_circuit(10);
        let observable = new_observable(10);

        // Making a ZZPhase
        let gate = new_diagonal_matrix_gate(
            &[0, 1],
            &[
                Complex {
                    real: 1.0,
                    imag: 0.0,
                },
                Complex {
                    real: 1.0,
                    imag: 0.0,
                },
                Complex {
                    real: 1.0,
                    imag: 0.0,
                },
                Complex {
                    real: 1.0,
                    imag: 0.0,
                },
            ],
        );
        add_gate_copy(&circuit, &gate);
        update_quantum_state(&circuit, &state);

        let value = get_expectation_value(&observable, &state);
        println!("{:?}", value);
    }

    #[test]
    fn sample_code() {
        let state = new_quantum_state(3, false);
        // Not a random state here.

        let circuit = new_quantum_circuit(3);
        add_x_gate(&circuit, 0);
        let merged_gate = merge(&new_cnot_gate(0, 1), &new_y_gate(1));
        add_gate_copy(&circuit, &merged_gate);
        add_r_x_gate(&circuit, 1, 0.5);
        update_quantum_state(&circuit, &state);

        let observable = new_observable(3);
        add_operator(&observable, 2.0.into(), "X 2 Y 1 Z 0");
        add_operator(&observable, (-3.0).into(), "Z 2");
        let value = get_expectation_value(&observable, &state);
        // Value obtained by running the same code from qulacs python.
        assert_eq!(value, Complex::from(-2.9999999999999996));
    }

    #[test]
    fn measurement_and_sampling() {
        let mut rng = rand::rng();
        let state = new_quantum_state(1, false);

        let circuit = new_quantum_circuit(1);
        add_h_gate(&circuit, 0);
        let measurement = new_measurement(0, 0, rng.random());
        add_gate_copy(&circuit, &measurement);
        update_quantum_state(&circuit, &state);

        let samples = quantum_state_sampling(&state, 50, 3);
        assert_eq!(samples.len(), 50);
    }
}
