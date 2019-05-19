# riscv-in-nutshell
Cycle-accurate RISC-V simulator for Intel/MIPT educational course

Simple project aimed at gaining knowledge in CPU modelling.
The performance simulator ([perfsim](Code/perfsim)) wraps the
functional simulator and provides cycle-accurate estimation of
operations latency.

## Description
- Traditional 5-stage pipeline
- Long-latency memory (with memory requests)
- Complete RV32I instruction set
- Configurable I- and D- caches
- Simple testing infrastructure (Catch2)
