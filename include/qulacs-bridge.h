#pragma once
#include <cppsim/gate.hpp>
#include <cstdint>
#include <memory>

#include <cppsim/circuit.hpp>
#include <cppsim/gate_factory.hpp>
#include <cppsim/gate_merge.hpp>
#include <cppsim/observable.hpp>
#include <cppsim/state.hpp>
#include <csim/type.hpp>

#include "rust/cxx.h"

struct Complex;

std::unique_ptr<QuantumStateBase> new_quantum_state(uint32_t qubit_count_,
                                                    bool use_multi_cpu);

rust::Vec<uint64_t>
quantum_state_sampling(const std::unique_ptr<QuantumStateBase> &state,
                       uint32_t sampling_count, uint32_t seed);

std::unique_ptr<QuantumCircuit> new_quantum_circuit(uint32_t qubit_count_);

void update_quantum_state(const std::unique_ptr<QuantumCircuit> &circuit,
                          const std::unique_ptr<QuantumStateBase> &state);

void add_h_gate(const std::unique_ptr<QuantumCircuit> &circuit, uint32_t index);
void add_x_gate(const std::unique_ptr<QuantumCircuit> &circuit, uint32_t index);
void add_y_gate(const std::unique_ptr<QuantumCircuit> &circuit, uint32_t index);
void add_z_gate(const std::unique_ptr<QuantumCircuit> &circuit, uint32_t index);

void add_r_x_gate(const std::unique_ptr<QuantumCircuit> &circuit,
                  uint32_t index, double angle);

void add_r_y_gate(const std::unique_ptr<QuantumCircuit> &circuit,
                  uint32_t index, double angle);

void add_r_z_gate(const std::unique_ptr<QuantumCircuit> &circuit,
                  uint32_t index, double angle);

void add_gate_copy(const std::unique_ptr<QuantumCircuit> &circuit,
                   const std::unique_ptr<QuantumGateBase> &gate);

std::unique_ptr<QuantumGateBase> new_identity_gate(uint32_t index);
std::unique_ptr<QuantumGateBase> new_h_gate(uint32_t index);
std::unique_ptr<QuantumGateBase> new_x_gate(uint32_t index);
std::unique_ptr<QuantumGateBase> new_y_gate(uint32_t index);
std::unique_ptr<QuantumGateBase> new_z_gate(uint32_t index);

std::unique_ptr<QuantumGateBase> new_r_x_gate(uint32_t index, double angle);
std::unique_ptr<QuantumGateBase> new_r_y_gate(uint32_t index, double angle);
std::unique_ptr<QuantumGateBase> new_r_z_gate(uint32_t index, double angle);

std::unique_ptr<QuantumGateBase> new_cnot_gate(uint32_t control,
                                               uint32_t target);

std::unique_ptr<QuantumGateBase>
new_diagonal_matrix_gate(rust::Slice<const uint32_t> target_qubits,
                         rust::Slice<const Complex> elements);

std::unique_ptr<QuantumGateBase> new_measurement(uint32_t index, uint32_t reg,
                                                 uint32_t seed);

std::unique_ptr<QuantumGateBase>
merge(const std::unique_ptr<QuantumGateBase> &applied_first,
      const std::unique_ptr<QuantumGateBase> &applied_later);

std::unique_ptr<Observable> new_observable(uint32_t qubit_count_);

void add_operator(const std::unique_ptr<Observable> &observable,
                  const Complex coef, const rust::Str pauli_string);

Complex get_expectation_value(const std::unique_ptr<Observable> &observable,
                              const std::unique_ptr<QuantumStateBase> &state);
