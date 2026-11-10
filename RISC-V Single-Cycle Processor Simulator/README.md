# Single-Cycle RISC-V Processor Simulator

A dependency-free Python simulator for a subset of the 32-bit RISC-V (RV32I) instruction set. It models a **single-cycle datapath**: each instruction is fetched, decoded, executed, and written back in one clock cycle. The simulator reads machine code written as binary text and prints every change to the registers, data memory, and program counter after each instruction.

## Features

- Runs programs supplied as 32-bit binary instructions, one per line
- Single-cycle datapath with separate Fetch, Decode, Execute, Memory, and Write Back functions
- Control unit and ALU control unit modeled after the textbook design
- 32 general-purpose registers (`x0` is hardwired to zero) and a 32-word data memory
- Two selectable initial machine states ("Part 1" and "Part 2")
- Per-cycle trace output using RISC-V ABI register names (`ra`, `sp`, `a0`, ...)
- Clear errors for unsupported opcodes, unsupported function codes, and out-of-range memory accesses

## Supported Instructions

| Format | Opcode | Instructions |
| --- | --- | --- |
| R-type | `0x33` | `add`, `sub`, `and`, `or` |
| I-type (arithmetic) | `0x13` | `addi`, `andi`, `ori` |
| Load | `0x03` | `lw` |
| Store | `0x23` | `sw` |
| Branch | `0x63` | `beq` (the ALU subtracts and the branch is taken when the result is zero) |
| Jump | `0x6F` | `jal` |
| Jump register | `0x67` | `jalr` (target is `rs1 + immediate` with the lowest bit cleared) |

Immediates are decoded and sign-extended for I, S, B, and J formats. Arithmetic results are wrapped to signed 32-bit values. For `jal` and `jalr`, the return address (`pc + 4`) is written to `rd`.

## Requirements

- Python 3 (standard library only, no packages to install)

## Usage

1. Save the simulator as a Python file (for example `simulator.py`).
2. Prepare a program file containing one 32-bit instruction per line, written as binary text:

   ```
   00000000010100000000000010010011
   00000000000100001000000100110011
   ```

3. Run the simulator and answer the two prompts:

   ```bash
   python simulator.py
   ```

   ```
   Which part to run? (1 or 2):
   1
   Enter the program file name to run:
   program.txt
   ```

If the file can't be found or contains something other than binary strings, the simulator prints a message and exits.

## Initial State

The part you choose sets the starting values. Everything not listed starts at zero, and the program counter starts at `0`.

**Part 1**

| Location | Value |
| --- | --- |
| `x1` (`ra`) | `0x20` |
| `x2` (`sp`) | `0x5` |
| `x10` (`a0`) | `0x70` |
| `x11` (`a1`) | `0x4` |
| Memory `0x70` (word 28) | `0x5` |
| Memory `0x74` (word 29) | `0x10` |

**Part 2**

| Location | Value |
| --- | --- |
| `x8` (`s0`) | `0x20` |
| `x10` (`a0`) | `0x5` |
| `x11` (`a1`) | `0x2` |
| `x12` (`a2`) | `0xA` |
| `x13` (`a3`) | `0xF` |
| Data memory | all zeros |

Data memory holds 32 words and is byte-addressed: the ALU's byte address is divided by 4 to get the word index. Accessing an address outside that range raises an error.

## Output

After each instruction, the simulator prints only what changed:

```
total_clock_cycles 1 :
a0 is modified to 0x5
memory 0xc is modified to 0x10
pc is modified to 0x4
```

Memory changes are reported by byte address. When the program finishes, the simulator prints:

```
program terminated:
total execution time is N cycles
```

Because this is a single-cycle design, `N` equals the number of instructions executed.

## How It Works

| Component | Purpose |
| --- | --- |
| `ControlUnit(opcode)` | Sets control signals (`RegWrite`, `MemRead`, `MemWrite`, `Branch`, `ALUSrc`, `MemtoReg`, `JAL`, `JALR`, `WritePC`, `ALUOp`) and rejects unsupported opcodes |
| `ALUControl(...)` | Chooses the ALU operation (AND, OR, ADD, SUB) from `ALUOp`, `funct3`, and `funct7` |
| `Fetch(instrs)` | Reads the instruction at `pc // 4` and computes `next_pc = pc + 4` |
| `Decode(...)` | Extracts instruction fields, reads source registers, sign-extends the immediate, and returns the ALU control code |
| `Execute(...)` | Performs the ALU operation, sets the zero flag, and computes the branch or jump target |
| `Mem(rs2_val)` | Performs `lw` and `sw` accesses |
| `WriteBack(rd)` | Writes the result to `rd` (ALU result, loaded value, or `pc + 4` for jumps) and advances the cycle counter |
| `main()` | Handles prompts, loads the program, runs the instruction loop, selects the next PC, and prints the trace |

**Next-PC selection:** after each instruction, `pc` becomes the jump target for `jal`/`jalr`, the branch target for a taken `beq`, or `pc + 4` otherwise. The simulation stops when `pc` points past the last instruction.

## Limitations

- **Limited instruction set.** Only `beq` is supported for branches and only word loads and stores (`lw`/`sw`). Shifts, `slt`, `lui`, `auipc`, and byte/halfword memory operations are not implemented.
- **Unsupported instructions stop the program.** An unknown opcode or function code raises a `ValueError`, and the error is not caught, so Python prints a traceback.
- **No alignment check.** Memory addresses are divided by 4 without checking that they're word-aligned.
- **Small memory.** Only 32 words of data memory are available.
- **No infinite-loop protection.** A program that never moves `pc` past its last instruction will run forever.
