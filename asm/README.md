# Assembly Logic (`asm/`)

This directory contains all assembly-related source and data files for the project. The assembly code is responsible for injecting logic into the target process and managing runtime data using a simple double-buffered list structure.

---

## Overview

The injected code maintains two lists of addresses and determines whether a new address should be added based on existing entries. It avoids duplicates and manages memory directly through a minimal runtime structure.

---

## File Descriptions

### `final.asm`
This is the **final, human-readable version** of the injected assembly code. It is written in Cheat Engine's assembly format and includes clearly labeled sections and logic. This file is assembled and injected into the target process.

- Implements a double-buffered list (two address pools)
- Avoids duplicate entries
- Appends new entries to the active list
- Includes register preservation and original instruction restoration
- Uses no locks; relies on controlled access timing

### `bytes.asm`
This is a **raw byte representation** of the same logic in `final.asm`, with helpful annotations. It shows exact offsets and corresponding opcodes as they appear in memory.

- One-to-one mapping with `final.asm`
- Includes instruction addresses and byte sequences

### `firstDraft.asm`
An **initial sketch of the injection logic**, written in a loose, informal style. This file was used to prototype the overall approach before porting the logic to Cheat Engine format.

- Describes memory layout with labels and offsets
- Includes early locking/semaphore logic (later removed)
- Serves as a reference for the logical structure of the code

---

## Notes

- The final injection code no longer uses locking or atomic instructions. To make this viable, a short sleep exists after the list flag flip. This gives any working threads some time to finish up.
