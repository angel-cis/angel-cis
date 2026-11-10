# RISC-V Pipeline Processor Simulator

A small, dependency-free Python simulator for a subset of the 32-bit RISC-V (RV32I) instruction set. It reads machine code written as binary text, executes it on a model of a 5-stage pipelined datapath, and prints every change to the registers, data memory, and program counter on each clock cycle.

## Features

- Runs programs supplied as 32-bit binary instructions, one per line
- Models a classic 5-stage pipeline: **IF → ID → EX → MEM → WB**
- Hazard detection that stalls the pipeline on data dependencies (no forwarding)
- Separate control unit and ALU control unit, as in the textbook datapath
- 32 general-purpose registers and a 32-word data memory
- Per-cycle trace output using RISC-V ABI register names (`ra`, `sp`, `a0`, ...)
- A `PIPELINE` flag in `main()` intended to switch between pipelined and single-cycle execution

## Supported Instructions

| Format | Opcode | Instructions |
| --- | --- | --- |
| R-type | `0x33` | `add`, `sub`, `and`, `or` |
| I-type (arithmetic) | `0x13` | `addi`, `andi`, `ori` |
| Load | `0x03` | `lw` |
| Store | `0x23` | `sw` |
| Branch | `0x63` | `beq` (the ALU subtracts and the branch is taken when the result is zero) |
| Jump | `0x6F` | `jal` |
| Jump register | `0x67` | `jalr` |

Immediates are decoded and sign-extended for I, S, B, and J formats. Arithmetic results are wrapped to signed 32-bit values.

## Requirements

- Python 3 (standard library only, no packages to install)

## Usage

1. Save the simulator as a Python file (for example `simulator.py`).
2. Prepare a program file containing one 32-bit instruction per line, written as binary text:

   ```
   00000000010100000000000010010011
   00000000000100001000000100110011
   ```

3. Run the simulator and enter the program file name when prompted:

   ```bash
   python simulator.py
   ```

   ```
   Enter the program file name to run:
   program.txt
   ```

## Initial State

Before execution, the simulator sets these values:

| Location | Value |
| --- | --- |
| `x1` (`ra`) | `0x20` |
| `x2` (`sp`) | `0x5` |
| `x10` (`a0`) | `0x70` |
| `x11` (`a1`) | `0x4` |
| `data_memory[28]` | `0x5` |
| `data_memory[29]` | `0x10` |

All other registers and memory words start at zero. The program counter starts at `0`.

Data memory is word-addressed: a byte address from the ALU is divided by 4 to find the word index, and only indices 0–31 are valid. Accesses outside that range are ignored.

## Output

Each cycle, the simulator prints only what changed:

```
total_clock_cycles: 5 :
a0 is modified to 0x5
data_memory[3] is modified to 0x10
pc is modified to 0x14
```

In pipeline mode, trace output begins at cycle 5, once the pipeline has filled. When the program finishes, the simulator prints:

```
program terminated:
total execution time is N cycles
```

## How It Works

| Component | Purpose |
| --- | --- |
| `ControlUnit(opcode)` | Sets control signals (`RegWrite`, `MemRead`, `MemWrite`, `Branch`, `ALUSrc`, `MemtoReg`, `JAL`, `JALR`, `WritePC`, `ALUOp`) |
| `ALUControlUnit(...)` | Chooses the ALU operation from `ALUOp`, `funct3`, and `funct7` (AND, OR, add, subtract) |
| `Decode`, `Execute`, `Mem`, `WriteBack` | Shared logic for each datapath stage |
| `*_stage()` functions | Pipelined versions that read from and write to the `IF_ID`, `ID_EX`, `EX_MEM`, and `MEM_WB` pipeline registers |
| `hazard_detected()` | Compares the instruction in decode against the destination registers in EX/MEM and MEM/WB and stalls on a match |
| `main()` | Loads the program, sets the initial state, runs the clock loop, and prints the trace |

In the pipeline loop, stages run in reverse order each cycle (WB, MEM, EX, then decode and fetch). This lets each stage read the previous cycle's pipeline register values before they are overwritten. The simulation ends when the fetch stage runs off the end of the program and all pipeline registers are empty.

## Known Limitations

- **Single-cycle mode does not run as written.** With `PIPELINE = False`, `main()` calls `Mem(rs2_val)` with one argument although `Mem` takes three, and it prints changes using `old_reg` and `old_mem`, which are never defined in that branch. Pipeline mode is the working path.
- **Initial values for "2" programs are overwritten.** If the file name contains `2`, extra registers (`x8`, `x10`–`x13`) are set, but the shared initialization after it then overrides `x10` and `x11`.
- **No data forwarding.** Dependencies are handled only by stalling.
- **No flushing on taken branches or jumps.** Wrong-path instructions already in the pipeline are not squashed.
- **`jalr` target.** The jump target is computed as `pc + immediate` rather than `rs1 + immediate`.
- **Limited instruction set.** Only `beq` is supported for branches, and shifts, comparisons (`slt`), `lui`, `auipc`, and byte/halfword loads and stores are not implemented.
- **Memory is small.** Only 32 words of data memory are available.
