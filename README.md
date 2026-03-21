# EliasGammaPacker

⚠️ WORK IN PROGRESS ⚠️

Programs currently may have several issues.
But they ensure that input file remains unchanged.

Don't compress any sensible file without any backup.

## Requirements

(currently) POSIX operating system

## Compilation

There are two compilation flag that can be used:
``-DNO_COMPUTED_GOTO`` to use a switch instead of computed goto.
``-DEGP_NOCHECK`` to remove boundary checks (implies that bitpacking vector size is enough) because no checks will be done.

```bash
make
```

## Usage [experimental]

Compression
```bash
#Input can be any file
./rle_egp <input> <output>
```
Decompression
```bash
#Input must be a file compressed using `rle_egp` program
./unrle_egp <input> <output>
```

## File format
File format remains "simple".  
  
**HEADER**
```
+-------------------------------------------------------------+
|             uncompressed file size (64 bits)                |
+-------------------------------------------------------------+
|                   bit position (64 bits)                    |
+-----------------------------------+-------------------------+
| number of packed values (63 bits) | first bit value (1 bit) |
+-----------------------------------+-------------------------+
```
**PAYLOAD** (concatenated Elias-&gamma; words)
```
+-------------------------------------------------------------+
|       little endian payload (bit position / 64 bytes)       |
+-------------------------------------------------------------+
|          big endian payload (bit position % 64 bytes)       |
+-------------------------------------------------------------+
```
