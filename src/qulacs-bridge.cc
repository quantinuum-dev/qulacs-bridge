#pragma once
#include <cppsim/circuit.hpp>
#include <cppsim/gate.hpp>
#include <cppsim/gate_factory.hpp>
#include <cppsim/gate_general.hpp>
#include <cppsim/gate_matrix_diagonal.hpp>
#include <cppsim/gate_merge.hpp>
#include <cppsim/gate_named_one.hpp>
#include <cppsim/gate_named_two.hpp>
#include <cppsim/type.hpp>
#include <csim/type.hpp>
#include <memory>

#include "qulacs-bridge/include/qulacs-bridge.h"
#include "qulacs-bridge/src/lib.rs.h"

std::unique_ptr<QuantumStateBase> new_quantum_state(unsigned int qubit_count_,
                                                    bool use_multi_cpu) {
  return std::unique_ptr<QuantumStateBase>(
      new QuantumState(qubit_count_, use_multi_cpu));
}

rust::Vec<uint64_t>
quantum_state_sampling(const std::unique_ptr<QuantumStateBase> &state,
                       uint32_t sampling_count, uint32_t seed) {
  rust::Vec<uint64_t> rust_samples;
  auto samples = state->sampling(sampling_count, seed);
  std::copy(samples.begin(), samples.end(), std::back_inserter(rust_samples));
  return rust_samples;
}

std::unique_ptr<QuantumCircuit> new_quantum_circuit(unsigned int qubit_count_) {
  return std::unique_ptr<QuantumCircuit>(new QuantumCircuit(qubit_count_));
}

void update_quantum_state(const std::unique_ptr<QuantumCircuit> &circuit,
                          const std::unique_ptr<QuantumStateBase> &state) {
  return circuit->update_quantum_state(&*state);
}


void add_h_gate(const std::unique_ptr<QuantumCircuit> &circuit,
                uint32_t index) {
  return circuit->add_H_gate(index);
}

void add_x_gate(const std::unique_ptr<QuantumCircuit> &circuit,
                uint32_t index) {
  return circuit->add_X_gate(index);
}

void add_y_gate(const std::unique_ptr<QuantumCircuit> &circuit,
                uint32_t index) {
  return circuit->add_Y_gate(index);
}

void add_z_gate(const std::unique_ptr<QuantumCircuit> &circuit,
                uint32_t index) {
  return circuit->add_Z_gate(index);
}

void add_r_x_gate(const std::unique_ptr<QuantumCircuit> &circuit,
                  uint32_t index, double angle) {
  return circuit->add_RotX_gate(index, angle);
}

void add_r_y_gate(const std::unique_ptr<QuantumCircuit> &circuit,
                  uint32_t index, double angle) {
  return circuit->add_RotY_gate(index, angle);
}

void add_r_z_gate(const std::unique_ptr<QuantumCircuit> &circuit,
                  uint32_t index, double angle) {
  return circuit->add_RotZ_gate(index, angle);
}

void add_cnot_gate(const std::unique_ptr<QuantumCircuit> &circuit,
                   uint32_t control, uint32_t target) {
  return circuit->add_CNOT_gate(control, target);
}

void add_gate_copy(const std::unique_ptr<QuantumCircuit> &circuit,
                   const std::unique_ptr<QuantumGateBase> &gate) {
  return circuit->add_gate_copy(gate->copy());
}

/// Gates!

std::unique_ptr<QuantumGateBase> new_identity_gate(uint32_t index) {
  auto ptr = std::unique_ptr<ClsOneQubitGate>(new ClsOneQubitGate());
  ptr->IGateinit(index);
  return ptr;
}

std::unique_ptr<QuantumGateBase> new_h_gate(uint32_t index) {
  auto ptr = std::unique_ptr<ClsOneQubitGate>(new ClsOneQubitGate());
  ptr->HGateinit(index);
  return ptr;
}

std::unique_ptr<QuantumGateBase> new_x_gate(uint32_t index) {
  auto ptr = std::unique_ptr<ClsOneQubitGate>(new ClsOneQubitGate());
  ptr->XGateinit(index);
  return ptr;
}

std::unique_ptr<QuantumGateBase> new_y_gate(uint32_t index) {
  auto ptr = std::unique_ptr<ClsOneQubitGate>(new ClsOneQubitGate());
  ptr->YGateinit(index);
  return ptr;
}

std::unique_ptr<QuantumGateBase> new_z_gate(uint32_t index) {
  auto ptr = std::unique_ptr<ClsOneQubitGate>(new ClsOneQubitGate());
  ptr->ZGateinit(index);
  return ptr;
}

std::unique_ptr<QuantumGateBase> new_r_x_gate(uint32_t index, double angle) {
  auto ptr =
      std::unique_ptr<ClsOneQubitRotationGate>(new ClsOneQubitRotationGate());
  ptr->RXGateinit(index, -angle);
  return ptr;
}

std::unique_ptr<QuantumGateBase> new_r_y_gate(uint32_t index, double angle) {
  auto ptr =
      std::unique_ptr<ClsOneQubitRotationGate>(new ClsOneQubitRotationGate());
  ptr->RYGateinit(index, -angle);
  return ptr;
}

std::unique_ptr<QuantumGateBase> new_r_z_gate(uint32_t index, double angle) {
  auto ptr =
      std::unique_ptr<ClsOneQubitRotationGate>(new ClsOneQubitRotationGate());
  ptr->RZGateinit(index, -angle);
  return ptr;
}

std::unique_ptr<QuantumGateBase> new_cnot_gate(uint32_t control,
                                               uint32_t target) {
  auto ptr = std::unique_ptr<ClsOneControlOneTargetGate>(
      new ClsOneControlOneTargetGate());
  ptr->CNOTGateinit(control, target);
  return ptr;
}

std::unique_ptr<QuantumGateBase>
new_diagonal_matrix_gate(rust::Slice<const uint32_t> target_qubits,
                         rust::Slice<const Complex> elements) {
  std::vector<uint32_t> target_qubits_vec;
  std::copy(target_qubits.begin(), target_qubits.end(),
            std::back_inserter(target_qubits_vec));

  // This would be nicer with c++23 but we only have up to c++17 on fugaku.
  ComplexVector elements_vec(elements.size());
  for (std::size_t index = 0; index < elements.size(); index++) {
    auto element = elements[0];
    elements_vec[index] = std::complex<double>(element.real, element.imag);
  }

  return std::unique_ptr<QuantumGateDiagonalMatrix>(
      new QuantumGateDiagonalMatrix(target_qubits_vec, elements_vec));
}

std::unique_ptr<QuantumGateBase> new_measurement(uint32_t index, uint32_t reg, uint32_t seed) {
  return std::unique_ptr<QuantumGate_Instrument>(gate::Measurement(index, reg, seed));
}

std::unique_ptr<QuantumGateBase>
merge(const std::unique_ptr<QuantumGateBase> &applied_first,
      const std::unique_ptr<QuantumGateBase> &applied_later) {
  return std::unique_ptr<QuantumGateBase>(
      gate::merge(&*applied_first, &*applied_later));
}

/// Observables!

std::unique_ptr<Observable> new_observable(uint32_t qubit_count_) {
  return std::unique_ptr<Observable>(new Observable(qubit_count_));
}

void add_operator(const std::unique_ptr<Observable> &observable,
                  const Complex coef, const rust::Str pauli_string) {
  return observable->add_operator(std::complex<double>(coef.real, coef.imag),
                                  std::string(pauli_string));
}

Complex get_expectation_value(const std::unique_ptr<Observable> &observable,
                              const std::unique_ptr<QuantumStateBase> &state) {
  auto expectation_value = observable->get_expectation_value(&*state);
  Complex value = {expectation_value.real(), expectation_value.imag()};
  return value;
}
