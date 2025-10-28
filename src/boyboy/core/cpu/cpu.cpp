/**
 * @file cpu.cpp
 * @brief CPU emulation for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/cpu/cpu.h"

#include <cstdint>

#include "boyboy/common/log/logging.h"
#include "boyboy/common/utils.h"
#include "boyboy/core/cpu/cpu_constants.h"
#include "boyboy/core/cpu/cycles.h"
#include "boyboy/core/cpu/instructions.h"
#include "boyboy/core/cpu/instructions_table.h"
#include "boyboy/core/cpu/state.h"
#include "boyboy/core/profiling/profiler_utils.h"

namespace boyboy::core::cpu {

using namespace boyboy::common;

void Cpu::init()
{
    // Registers
    registers_.af = AFStartValue;
    registers_.bc = BCStartValue;
    registers_.de = DEStartValue;
    registers_.hl = HLStartValue;
    registers_.sp = SPStartValue;
    registers_.pc = PCStartValue;

    // Reset flags and state
    ime_ = false;
    ime_scheduled_ = 0;
    halted_ = false;
    cycles_ = 0;
    exec_state_.init();
}
void Cpu::reset()
{
    init();
}

uint8_t Cpu::get_register(Reg8Name reg) const
{
    switch (reg) {
        case Reg8Name::A:
            return registers_.a();
        case Reg8Name::F:
            return registers_.f();
        case Reg8Name::B:
            return registers_.b();
        case Reg8Name::C:
            return registers_.c();
        case Reg8Name::D:
            return registers_.d();
        case Reg8Name::E:
            return registers_.e();
        case Reg8Name::H:
            return registers_.h();
        case Reg8Name::L:
            return registers_.l();
        default:
            return 0;
    }
}

uint16_t Cpu::get_register(Reg16Name reg) const
{
    switch (reg) {
        case Reg16Name::AF:
            return registers_.af;
        case Reg16Name::BC:
            return registers_.bc;
        case Reg16Name::DE:
            return registers_.de;
        case Reg16Name::HL:
            return registers_.hl;
        case Reg16Name::SP:
            return registers_.sp;
        case Reg16Name::PC:
            return registers_.pc;
        default:
            return 0;
    }
}

void Cpu::set_register(Reg8Name reg, uint8_t value)
{
    switch (reg) {
        case Reg8Name::A:
            registers_.a(value);
            break;
        case Reg8Name::F:
            registers_.f(value);
            break;
        case Reg8Name::B:
            registers_.b(value);
            break;
        case Reg8Name::C:
            registers_.c(value);
            break;
        case Reg8Name::D:
            registers_.d(value);
            break;
        case Reg8Name::E:
            registers_.e(value);
            break;
        case Reg8Name::H:
            registers_.h(value);
            break;
        case Reg8Name::L:
            registers_.l(value);
            break;
        default:
            break;
    }
}

void Cpu::set_register(Reg16Name reg, uint16_t value)
{
    switch (reg) {
        case Reg16Name::AF:
            registers_.af = value;
            break;
        case Reg16Name::BC:
            registers_.bc = value;
            break;
        case Reg16Name::DE:
            registers_.de = value;
            break;
        case Reg16Name::HL:
            registers_.hl = value;
            break;
        case Reg16Name::SP:
            registers_.sp = value;
            break;
        case Reg16Name::PC:
            registers_.pc = value;
            break;
        default:
            break;
    }
}

void Cpu::set_halted(bool halted)
{
    if (halted != halted_) {
        log::debug("CPU HALT mode {}", halted ? "entered" : "exited");
    }
    halted_ = halted;
}

TCycle Cpu::tick()
{
    BB_PROFILE_SCOPE(profiling::FrameTimer::Cpu);

    if (tick_mode_ == TickMode::Instruction) {
        return step();
    }

    auto cycles = tickmode_to_cycles(tick_mode_);
    tick_cycles(cycles);

    return to_tcycles(cycles);
}

inline uint8_t Cpu::step()
{
    uint8_t cycles = interrupt_handler_.service();
    cycles_ += cycles;

    if (halted_) {
        // Add 4 cycles as if we were executing NOPs
        cycles_ += 4;
        return cycles + 4;
    }

#ifdef DISASSEMBLY_LOG
    trace();
#endif

    uint8_t opcode = fetch();
    InstructionType instr_type = InstructionType::Unprefixed;

    if (opcode == CBInstructionPrefix) {
        opcode = fetch();
        instr_type = InstructionType::CBPrefixed;
    }

    cycles += execute(opcode, instr_type);

    // IME is enabled after the instruction following EI
    if (is_ime_scheduled() &&
        (opcode != static_cast<uint8_t>(Opcode::EI) || instr_type != InstructionType::Unprefixed)) {
        ime_scheduled_ = 0;
        ime_ = true;
    }

    return cycles;
}

inline void Cpu::tick_cycles(Cycles cycles)
{
    // Number of T-cycles to tick
    auto tcycles = to_tcycles(cycles);
    cycles_ += tcycles;

    // Handle interrupts
    if (exec_state_.has_stage(Stage::InterruptService)) {
        interrupt_handler_.tick(cycles);
        if (!interrupt_handler_.is_servicing()) {
            // It already finished servicing the interrupt
            clear_flag(exec_state_.stage, Stage::InterruptService);
        }
        return;
    }

    // Enable IME if scheduled
    if (is_ime_scheduled()) {
        ime_scheduled_ -= tcycles;
        if (ime_scheduled_ == 0) {
            set_ime(true);
        }
    }

    // If halted skip rest of the cycle as if executing NOPs
    if (is_halted()) {
        return;
    }

    exec_state_.cycles_left -= tcycles;

    // Fetch/execute overlap: the fetch stage always overlaps with the last machine cycle of the
    // execute stage of the previous instruction
    if (exec_state_.stage == Stage::Execute && exec_state_.cycles_left <= FetchCycles) {
        exec_state_.stage |= Stage::Fetch;
    }

    // Take actions on stage end
    if (exec_state_.cycles_left == 0) {
        if (exec_state_.has_stage(Stage::Execute)) {
            execute_stage();

            // If an interrupt service was scheduled skip fetch
            if (exec_state_.has_stage(Stage::InterruptService)) {
                return;
            }
        }
        // We cascade from Execute to Fetch on purpose to allow fetch/execute overlap
        if (exec_state_.has_stage(Stage::Fetch)) {
            fetch_stage();
        }
    }
}

inline void Cpu::fetch_stage()
{
    // Fetch next byte
    exec_state_.fetched = fetch();

    if (exec_state_.fetched == CBInstructionPrefix &&
        !exec_state_.has_stage(Stage::CBInstruction)) {
        exec_state_.stage |= Stage::CBInstruction;
        exec_state_.cycles_left = FetchCycles;
    }
    else {
        InstructionType instr_type = exec_state_.has_stage(Stage::CBInstruction)
                                         ? InstructionType::CBPrefixed
                                         : InstructionType::Unprefixed;
        exec_state_.instr = &InstructionTable::get_instruction(instr_type, exec_state_.fetched);
        exec_state_.stage = Stage::Execute;
        exec_state_.cycles_left = exec_state_.instr->cycles;
    }
}

inline void Cpu::execute_stage()
{
    // Execute instruction handler
    (this->*(exec_state_.instr->execute))();

    clear_flag(exec_state_.stage, Stage::Execute);

    // If an interrupt should be serviced, do it and reset to fetch
    if (interrupt_handler_.should_service()) {
        exec_state_.stage = Stage::Fetch | Stage::InterruptService;
        exec_state_.cycles_left = FetchCycles;
        return;
    }
}

uint8_t Cpu::fetch()
{
    BB_PROFILE_START(profiling::HotSection::CpuFetch);
    uint8_t result = read_byte(registers_.pc++);
    BB_PROFILE_STOP(profiling::HotSection::CpuFetch);

    if (halt_bug_) {
        // when halt bug occurs we don't increment PC
        registers_.pc--;
        halt_bug_ = false;
        log::debug(
            "CPU HALT bug handled, not advancing PC={}", utils::PrettyHex{get_pc()}.to_string()
        );
    }

    return result;
}

[[nodiscard]] uint8_t Cpu::peek() const
{
    return read_byte(registers_.pc);
}

uint8_t Cpu::execute(uint8_t opcode, InstructionType instr_type)
{
    BB_PROFILE_START(profiling::HotSection::CpuExecute);
    const auto& instr = InstructionTable::get_instruction(instr_type, opcode);
    (this->*instr.execute)();
    cycles_ += instr.cycles;
    BB_PROFILE_STOP(profiling::HotSection::CpuExecute);
    return instr.cycles;
}

void Cpu::trace() const
{
    log::cpu_trace("--- CPU TRACE ---");
    log::cpu_trace(
        "Instruction: {} ({})",
        disassemble(registers_.pc),
        common::utils::PrettyHex(peek()).to_string()
    );
    log::cpu_trace(
        "Next bytes: {} {} {}",
        common::utils::PrettyHex(read_byte(registers_.pc + 1)).to_string(),
        common::utils::PrettyHex(read_byte(registers_.pc + 2)).to_string(),
        common::utils::PrettyHex(read_byte(registers_.pc + 3)).to_string()
    );
    log::cpu_trace(
        "CPU State: PC={}, SP={}, AF={}, BC={}, DE={}, HL={}, Flags=[Z={}, N={}, H={}, C={}], "
        "IME={}, HALT={}, Cycles={}",
        common::utils::PrettyHex(registers_.pc).to_string(),
        common::utils::PrettyHex(registers_.sp).to_string(),
        common::utils::PrettyHex(registers_.af).to_string(),
        common::utils::PrettyHex(registers_.bc).to_string(),
        common::utils::PrettyHex(registers_.de).to_string(),
        common::utils::PrettyHex(registers_.hl).to_string(),
        get_flag(Flag::Zero) ? 1 : 0,
        get_flag(Flag::Substract) ? 1 : 0,
        get_flag(Flag::HalfCarry) ? 1 : 0,
        get_flag(Flag::Carry) ? 1 : 0,
        ime_ ? "ENABLED" : "DISABLED",
        halted_ ? "HALTED" : "RUNNING",
        cycles_
    );
    log::cpu_trace("----------------");
}

[[nodiscard]] std::string_view Cpu::disassemble(uint16_t addr) const
{
    uint8_t opcode = read_byte(addr);
    InstructionType instr_type = InstructionType::Unprefixed;

    if (opcode == CBInstructionPrefix) {
        opcode = read_byte(addr + 1);
        instr_type = InstructionType::CBPrefixed;
    }

    const auto& instr = InstructionTable::get_instruction(instr_type, opcode);

    // Simple implementation: just return the mnemonic for now
    return instr.mnemonic;
}

// Private methods

void Cpu::add(uint8_t val, bool use_carry)
{
    uint8_t a = registers_.a();
    uint8_t carry_in = (use_carry && get_flag(Flag::Carry)) ? 1 : 0;

    uint16_t sum = uint16_t(a) + uint16_t(val) + carry_in;
    uint8_t result = sum & 0xFF;

    registers_.a(result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, ((a & 0xF) + (val & 0xF) + carry_in) > 0xF);
    set_flag(Flag::Carry, sum > 0xFF);
}

void Cpu::sub(uint8_t val, bool use_carry)
{
    uint8_t a = registers_.a();
    uint8_t carry_in = (use_carry && get_flag(Flag::Carry)) ? 1 : 0;

    uint16_t sub = uint16_t(val) + carry_in;
    uint8_t result = a - sub;

    registers_.a(result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, true);
    set_flag(Flag::HalfCarry, (a & 0xF) < ((val & 0xF) + carry_in));
    set_flag(Flag::Carry, a < sub);
}

void Cpu::aand(uint8_t val)
{
    auto result = registers_.a() & val;
    registers_.a(result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, true);
    set_flag(Flag::Carry, false);
}

void Cpu::xxor(uint8_t val)
{
    auto result = registers_.a() ^ val;
    registers_.a(result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, false);
}

void Cpu::oor(uint8_t val)
{
    auto result = registers_.a() | val;
    registers_.a(result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, false);
}

void Cpu::cp(uint8_t val)
{
    // Compare is essentially a subtraction without modifying A
    uint8_t a = registers_.a();
    uint8_t result = a - val;

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, true);
    set_flag(Flag::HalfCarry, (result & 0x0F) > (a & 0x0F));
    set_flag(Flag::Carry, result > a);
}

} // namespace boyboy::core::cpu