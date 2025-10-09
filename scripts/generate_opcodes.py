#!/usr/bin/env python3

"""
Generate opcode tables and CPU function declarations/implementations from a JSON file.
Reads from data/Opcodes.json and writes to src/generated/.
"""

import argparse
import json
from pathlib import Path
from datetime import datetime, timezone

JSON_FILE = Path(__file__).parent.parent / "data/Opcodes.json"
PRIVATE_OUTPUT_DIR = Path(__file__).parent.parent / "src/boyboy/core/cpu/generated"
PUBLIC_OUTPUT_DIR = Path(__file__).parent.parent / "include/boyboy/core/cpu/generated"
PRIVATE_OUTPUT_DIR.mkdir(exist_ok=True)
PUBLIC_OUTPUT_DIR.mkdir(exist_ok=True)


def get_cycles(cycle_list):
    """Return the first cycle count or 0 if empty."""
    return cycle_list[0] if cycle_list else 0


def get_operands(ops_list):
    """Format operands for display, adding + or - if increment/decrement."""
    formatted = []
    for op in ops_list:
        name = op["name"]
        # Append + or - for display
        if op.get("increment", False):
            name += "+"
        elif op.get("decrement", False):
            name += "-"
        # Wrap in [] for memory access
        if not op.get("immediate", False):
            name = f"[{name}]"
        formatted.append(name)
    return ", ".join(formatted)


def get_func_name(mnemonic, ops_list):
    """
    Generate a C++ function name from a mnemonic and operand list.

    Rules:
    - Memory (non-immediate) operands are marked with '_at'.
    - Increment/decrement flags are appended as '_inc' or '_dec'.
    - Registers and immediate operands are just the lowercase name.
    """
    name = mnemonic.lower().replace(" ", "_").replace("-", "_")

    for op in ops_list:
        op_name = op["name"].lower().replace("$", "")
        # Memory / indirect operands
        if not op.get("immediate", False):
            op_name = f"at_{op_name}"
        # Increment / decrement
        if op.get("increment", False):
            op_name += "_inc"
        elif op.get("decrement", False):
            op_name += "_dec"

        name += f"_{op_name}"

    return name


def get_mnemonic(mnemonic, ops_list):
    """Generate full mnemonic with operands."""
    if not ops_list:
        return mnemonic
    ops = get_operands(ops_list)
    return f"{mnemonic} {ops}"


def print_opcode(opcode, info):
    """Print opcode details for debugging."""
    code = int(opcode, 16)
    mnemonic = info["mnemonic"]
    length = info["bytes"]
    cycles = get_cycles(info.get("cycles", []))
    operands = get_operands(info["operands"])
    func_name = get_func_name(mnemonic, info["operands"])
    full_mnemonic = get_mnemonic(info["mnemonic"], info["operands"])

    print(f"Opcode: 0x{code:02X}")
    print(f"  Mnemonic: {mnemonic}")
    print(f"  Full Mnemonic: {full_mnemonic}")
    print(f"  Length: {length}")
    print(f"  Cycles: {cycles}")
    print(f"  Operands: {operands}")
    print(f"  Function Name: {func_name}")
    print()


def write_file_header(f, description, source_json):
    now = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%S UTC")
    f.write(f"// ============================================================\n")
    f.write(f"// {description}\n")
    f.write(f"// AUTO-GENERATED FILE — DO NOT EDIT MANUALLY\n")
    f.write(f"// Generated from {source_json} on {now}\n")
    f.write(f"// ============================================================\n\n")


def write_table(opcodes, path, description="Opcode Table"):
    """Write the opcode table to a file."""
    # Determine the maximum length of mnemonics for formatting
    mnemonic_len = max(
        len(get_mnemonic(info["mnemonic"], info.get("operands", [])))
        for info in opcodes.values()
    )

    # Generate file with the following format:
    # table[0x00] = {.mnemonic = "NOP", .length = 1, .cycles = 4, .execute = &Cpu::nop};
    with open(path, "w") as f:
        write_file_header(f, description, JSON_FILE.name)
        for code_str, info in sorted(opcodes.items(), key=lambda x: int(x[0], 16)):
            code = int(code_str, 16)
            mnemonic = info["mnemonic"]
            length = info["bytes"]
            # cycles = get_cycles(info.get('cycles', []))
            cycles = info["cycles"][0]
            operands = get_operands(info["operands"])
            func_name = get_func_name(mnemonic, info["operands"])
            full_mnemonic = get_mnemonic(info["mnemonic"], info["operands"])

            padding = " " * (mnemonic_len - len(full_mnemonic) + 1)

            f.write(
                f"table[0x{code:02X}] = {{"
                f'.mnemonic = "{full_mnemonic}",{padding}'
                f".length = {length}, "
                f'.cycles = {cycles},{" " * (2 - len(str(cycles)) + 1)}'
                f".execute = &Cpu::{func_name}}};\n"
            )
    print(f"[INFO] Wrote opcode table to {path}")


def write_cpu_decls(opcodes, path, description="CPU Function Declarations"):
    """Write CPU function declarations to a file."""
    with open(path, "w") as f:
        write_file_header(f, description, JSON_FILE.name)
        for code, info in sorted(opcodes.items(), key=lambda x: int(x[0], 16)):
            func_name = get_func_name(info["mnemonic"], info.get("operands", []))
            f.write(f"    void {func_name}();\n")
    print(f"[INFO] Wrote CPU function declarations to {path}")


def write_cpu_impls(opcodes, path, description="CPU Function Implementations"):
    """
    Generate CPU stub implementations with per-function guards and runtime error.
    Each stub is wrapped in:
    #ifndef CPU_FUNC_NAME
    void Cpu::FUNC_NAME() {
        throw UnimplementedOpcode(opcode, "MNEMONIC");
    }
    #endif
    """
    with open(path, "w") as f:
        write_file_header(f, description, JSON_FILE.name)
        f.write('#include "boyboy/common/errors.h"\n\n')

        for code, info in sorted(opcodes.items(), key=lambda x: int(x[0], 16)):
            func_name = get_func_name(info["mnemonic"], info.get("operands", []))
            macro_name = f"CPU_{func_name.upper()}"
            mnemonic = get_mnemonic(info["mnemonic"], info.get("operands", []))
            opcode_int = int(code, 16)

            f.write(f"#ifndef {macro_name}\n")
            f.write(f"void boyboy::cpu::Cpu::{func_name}() {{\n")
            f.write(
                f'    throw boyboy::common::errors::UnimplementedOpcode(0x{opcode_int:02X}, "{mnemonic}");\n'
            )
            f.write("}\n")
            f.write(f"#endif // {macro_name}\n\n")
    print(f"[INFO] Wrote CPU function implementations to {path}")


def check_duplicates(opcodes, prefix):
    """Check for repeated CPU function names and print detailed info."""
    func_map = {}  # func_name -> list of opcode hex codes
    for code_str, info in opcodes.items():
        func_name = get_func_name(info["mnemonic"], info.get("operands", []))
        func_map.setdefault(func_name, []).append(code_str)

    duplicates = {name: codes for name, codes in func_map.items() if len(codes) > 1}

    if duplicates:
        print(f"\n[ERROR] Duplicate function names detected in {prefix} opcodes:")
        for name, codes in duplicates.items():
            mnemonics = [
                get_mnemonic(opcodes[c]["mnemonic"], opcodes[c].get("operands", []))
                for c in codes
            ]
            for code, mnem in zip(codes, mnemonics):
                print(f"  {code}: {mnem} -> {name}")
        raise RuntimeError(f"Duplicate function names found in {prefix} opcodes")
    else:
        print(f"[INFO] No duplicates found in {prefix} opcodes")


# generate enum class Opcode and enum class CBOpcode
def write_opcode_enums(opcodes, path, cbprefix=False, description="Opcode Enums"):
    """Generate enum class for opcodes."""
    with open(path, "w") as f:
        enum_name = "CBOpcode" if cbprefix else "Opcode"
        write_file_header(f, description, JSON_FILE.name)
        f.write("#include <cstdint>\n\n")
        f.write("namespace boyboy::cpu {\n\n")
        f.write("// NOLINTBEGIN(readability-identifier-naming)\n\n")
        f.write(f"enum class {enum_name} : uint8_t {{\n")
        for code_str, info in sorted(opcodes.items(), key=lambda x: int(x[0], 16)):
            code = int(code_str, 16)
            mnemonic = get_mnemonic(info["mnemonic"], info.get("operands", []))
            func_name = get_func_name(
                info["mnemonic"], info.get("operands", [])
            ).upper()
            f.write(f"    {func_name} = 0x{code:02X}, // {mnemonic}\n")
        f.write("};\n\n")
        f.write("// NOLINTEND(readability-identifier-naming)\n\n")
        f.write("} // namespace boyboy::cpu\n")
    print(f"[INFO] Wrote opcode enums to {path}")


def main():
    parser = argparse.ArgumentParser(
        description="Generate opcode tables and CPU instruction declarations/implementations from a JSON file."
    )
    parser.add_argument("--table", action="store_true", help="Generate opcode tables")
    parser.add_argument(
        "--decls", action="store_true", help="Generate CPU instruction declarations"
    )
    parser.add_argument(
        "--impls",
        action="store_true",
        help="Generate CPU instruction implementations stubs",
    )
    parser.add_argument("--enums", action="store_true", help="Generate opcode enums")
    parser.add_argument("--all", action="store_true", help="Generate all files")
    args = parser.parse_args()

    with open(JSON_FILE) as f:
        data = json.load(f)

    check_duplicates(data["unprefixed"], "unprefixed")
    check_duplicates(data["cbprefixed"], "CB-prefixed")

    if args.table or args.all:
        write_table(
            data["unprefixed"],
            PRIVATE_OUTPUT_DIR / "opcodes.inc",
            "Opcode table for InstructionsTable (unprefixed)",
        )
        write_table(
            data["cbprefixed"],
            PRIVATE_OUTPUT_DIR / "cbopcodes.inc",
            "Opcode table for InstructionsTable (CB-prefixed)",
        )

    if args.decls or args.all:
        write_cpu_decls(
            data["unprefixed"],
            PUBLIC_OUTPUT_DIR / "cpu_opcodes.inc",
            "CPU function declarations (unprefixed)",
        )
        write_cpu_decls(
            data["cbprefixed"],
            PUBLIC_OUTPUT_DIR / "cpu_cbopcodes.inc",
            "CPU function declarations (CB-prefixed)",
        )

    if args.impls or args.all:
        write_cpu_impls(
            data["unprefixed"],
            PRIVATE_OUTPUT_DIR / "cpu_opcodes_impl.inc",
            "CPU stub implementations (unprefixed)",
        )
        write_cpu_impls(
            data["cbprefixed"],
            PRIVATE_OUTPUT_DIR / "cpu_cbopcodes_impl.inc",
            "CPU stub implementations (CB-prefixed)",
        )

    if args.enums or args.all:
        write_opcode_enums(
            data["unprefixed"],
            PUBLIC_OUTPUT_DIR / "opcode_enum.h",
            cbprefix=False,
            description="Opcode Enums (unprefixed)",
        )

        write_opcode_enums(
            data["cbprefixed"],
            PUBLIC_OUTPUT_DIR / "cbopcode_enum.h",
            cbprefix=True,
            description="Opcode Enums (CB-prefixed)",
        )


if __name__ == "__main__":
    main()
