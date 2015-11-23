These are more-or-less random notes on how to use the Z80 GUI.

1. The toolbar (starting with the Cog) is:
cog:		run.
downarrow: 	trace (20)
steps:		step
hazard:		dump debug info
magnifying glass: toggle display of machine code.


2. Registers are editable - use the "Update Registers" button to save changes.
3. Registers with a radio button can b used to drive the memory dump.
4. Toggle flags with the flag charater.
5. Memory register is an arbitrary meory selector.

6. Memory Window:

Green highlight shows the byte pointed to by the register.
Red highlight show current selection (click in window to select).

buttons below move selection 1 byte, 8 bytes and 1 page (back and forth)
Address/Content/Update allows editing of selected byte.

7. Code window:

Green highlight shows next instruction to be executed.
Clicking on wndow will select an instruction (highlighted in red).
Breakpoins are highlighted in blue.

8. Other buttons:

Break:	sets breakpoint at current code selection
Restart:	sets PC to start of code
Zero Regs:	zeros all registers
Synch:		sets the "Memory" register to selected memory location
Set PC:		sets the PC to selected code line.

