# Boyboy CPU Instruction Implementation Checklist

> **Note:** This is a living checklist for implementing and testing CPU instructions in Boyboy.
>
> ✅ in the “Test” note means the corresponding Blargg ROM passes.
>
> Bold items are **boot-ROM-critical**: needed to run the Nintendo logo and basic startup.

---

## Step 1 – Already Done

- [x] **All 8-bit ALU/logical ops (`ADD`, `ADC`, `SUB`, `SBC`, `AND`, `OR`, `XOR`, `CP`)** — Test: `09-op r,r.gb`
- [x] **`INC r`, `DEC r`, `INC [HL]`, `DEC [HL]`** — Test: `09-op r,r.gb`
- [x] **`NOP`** — Test: Boot ROM

## Step 2 – Loads

- [x] **`LD r, imm`** — Test: `04-op r,imm.gb`
- [x] **`LD r, r`** — Test: `06-ld r,r.gb`
- [x] `LD r, [HL]`, `LD [HL], r` — Test: `06-ld r,r.gb`
- [x] `LD [HL], n` — Test: `06-ld r,r.gb`
- [x] **`LD A, [BC]`, `LD A, [DE]`, `LD [BC], A`, `LD [DE], A`** — Test: `06-ld r,r.gb`
- [x] `LD A, [nn]`, `LD [nn], A` — Test: `06-ld r,r.gb`
- [x] `LDH A, [FF00+n]`, `LDH A, [FF00+C]`, `LDH [FF00+n], A`, `LDH [FF00+C], A` — Test: `06-ld r,r.gb`
- [x] `LD A, [HL+]`, `LD A, [HL-]`, `LD [HL+], A`, `LD [HL-], A` — Test: `06-ld r,r.gb`

## Step 3 – 16-bit basics

- [x] `LD rr, nn` — Test: `05-op rp.gb`
- [x] `INC rr` — Test: `05-op rp.gb`
- [x] `DEC rr` — Test: `05-op rp.gb`
- [x] `ADD HL, rr` — Test: `05-op rp.gb`

## Step 4 – Control flow

- [x] **`JP nn`, `JP cc,nn`** — Test: `07-jr,jp,call,ret,rst.gb`
- [x] **`JP HL`** — Test: `07-jr,jp,call,ret,rst.gb`
- [x] **`JR e`, `JR cc,e`** — Test: `07-jr,jp,call,ret,rst.gb`
- [x] `CALL nn`, `CALL cc,nn` — Test: `07-jr,jp,call,ret,rst.gb`
- [x] `RET`, `RET cc`, `RETI` — Test: `07-jr,jp,call,ret,rst.gb`
- [x] `RST n` — Test: `07-jr,jp,call,ret,rst.gb`

## Step 5 – Stack & SP/HL ops

- [x] `PUSH rr` — Test: `03-op sp,hl.gb`
- [x] `POP rr` — Test: `03-op sp,hl.gb`
- [x] `LD SP, nn` — Test: `03-op sp,hl.gb`
- [x] `LD HL, SP+e` — Test: `03-op sp,hl.gb`
- [x] `ADD SP, e` — Test: `03-op sp,hl.gb`

## Step 6 – Misc instructions

- [x] `DAA` — Test: `01-special.gb`
- [x] `CPL` — Test: `01-special.gb`
- [x] `SCF` — Test: `01-special.gb`
- [x] `CCF` — Test: `01-special.gb`
- [x] `DI` — Test: `01-special.gb`
- [x] `EI` — Test: `01-special.gb`
- [x] `HALT` — Test: `01-special.gb`
- [x] `STOP` — Test: `01-special.gb`
- [x] `RLCA`, `RLA`, `RRCA`, `RRA` — Test: `10-bit ops.gb`

## Step 7 – CB-prefix ops

- [x] Rotates/shifts (`RLC`, `RL`, `RRC`, `RR`, `SLA`, `SRA`, `SRL`, `SWAP`) — Test: `10-bit ops.gb`
- [x] Bit ops (`BIT b,r`, `RES b,r`, `SET b,r`) — Test: `10-bit ops.gb` / `11-op a,(hl).gb`

---

## Boot-ROM Critical Instructions

- [x] All 8-bit ALU/logical ops
- [x] INC / DEC / INC [HL] / DEC [HL]
- [x] NOP
- [x] LD r, imm
- [x] LD r, r
- [x] LD A, [BC] / [DE] / [BC] → A / [DE] → A
- [x] JP / JR
