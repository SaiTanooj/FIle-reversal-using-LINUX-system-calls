echo "# File Reversal and Verification

This project implements various file reversal and verification operations using system calls in C.

## Q1 — File Reversal Operations

1. **Input Handling** — Used a structure to define required arguments. Took input from the command line using argc and argv.
2. **Creating Directory** — Used mkdir system call with error handling.
3. **Open_Input** — Opens the input file and returns the file descriptor.
4. **Open_Output** — Output file format: <flag_name><input_filename>, extracted filename, created file if not present.
5. **Reverse Blockwise** — Read in block-sized chunks, reversed buffer, wrote output.
6. **Reverse Full** — Traversed file from end to start, reversed blocks, wrote output.
7. **Partial Reversal** — Reversed before start, copied middle section, reversed after end.

## Q2 — File Verification Operations

1. **Input Handling** — Same as Q1.
2. **Take Permission** — Used struct stat to check file permissions.
3. **Open File** — Opened file and returned descriptor.
4. **same_size** — Compared sizes using lseek.
5. **Verify Blockwise Reversal** — Compared reversed input chunks with output.
6. **Verify Full File Reversal** — Compared reversed input (from EOF) with output (from start).
7. **Verify Partial Reversal** — Compared appropriate file segments for match.
"
