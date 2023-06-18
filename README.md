# FISC-Assembler-and-Simulator
 
I have created an assembler that converts FISC (four instruction set computers) assembly code to hexadecimal. It takes in an assembly file as input and generates an output of hexadecimal values on a separate file. The assembler first converts each assembly instruction to machine code (1's and 0's). The machine code is then converted to a hexadecimal value and written to the output file. I also created a simulator which display the execution of the program line by line. It first takes the input file (the assembler generated hex file) and converts it to machine code. It also outputs a step by step visualization stating the values of each register, the value of the zero flag, and the program count after the execution of each instruction. It also behaves as a disassembler as it generates the assembly instruction for the respective machine code.

Command to run the assembler:
  ./a.out t2.s pp.hex 
  t2.s is the input assembly file and pp.hex is the hexadecimal file produced as an output.
  
  
  If you do no want to write to the output file:
  ./a.out t2.s pp.hex -l
  
  This produces the output:
  *** LABEL LIST ***
  endloop : 11
  loop : 5
  start : 0
  *** MACHINE LIST ***
  0:90    not r0 r1
  1:44    and r0 r0 r1
  2:81    not r1 r0
  3:15    add r1 r1 r1
  4:91    not r1 r1
  5:43    and r3 r0 r0
  6:1c    add r0 r1 r3
  7:cb    bnz endloop
  8:7f    and r3 r3 r3
  9:b2    not r2 r3
  10:6d    and r1 r2 r3
  11:00    add r0 r0 r0
 
 
  However, nothing will be written to the output file pp.hex.
  


Command to run the simulator:
  ./a.out pp.hex
  
  The above command will output:
  Cycle: 1 State:PC:1 Z: 0 R0: ff R1: 0 R2: 0 R3: 0
  Disassembly: not r0 r1 
  Cycle: 2 State:PC:2 Z: 1 R0: 0 R1: 0 R2: 0 R3: 0
  Disassembly: and r0 r0 r1 
  Cycle: 3 State:PC:3 Z: 0 R0: 0 R1: ff R2: 0 R3: 0
  Disassembly: not r1 r0 
  Cycle: 4 State:PC:4 Z: 0 R0: 0 R1: fe R2: 0 R3: 0
  Disassembly: add r1 r1 r1 
  Cycle: 5 State:PC:5 Z: 0 R0: 0 R1: 1 R2: 0 R3: 0
  Disassembly: not r1 r1 
  Cycle: 6 State:PC:6 Z: 1 R0: 0 R1: 1 R2: 0 R3: 0
  Disassembly: and r3 r0 r0 
  Cycle: 7 State:PC:7 Z: 0 R0: 1 R1: 1 R2: 0 R3: 0
  Disassembly: add r0 r1 r3 
  Cycle: 8 State:PC:8 Z: 0 R0: 1 R1: 1 R2: 0 R3: 0
  Disassembly: bnz 11 
  Cycle: 9 State:PC:12 Z: 0 R0: 2 R1: 1 R2: 0 R3: 0
  Disassembly: add r0 r0 r0 
  
However, if you want all the cycles to print as it might take too much time. You can specify the number of cycles to print in the command line argument.

Command line argument:
  ./a.out pp.hex 5
  
  The above command will output:
  Cycle: 1 State:PC:1 Z: 0 R0: ff R1: 0 R2: 0 R3: 0
  Disassembly: not r0 r1 
  Cycle: 2 State:PC:2 Z: 1 R0: 0 R1: 0 R2: 0 R3: 0
  Disassembly: and r0 r0 r1 
  Cycle: 3 State:PC:3 Z: 0 R0: 0 R1: ff R2: 0 R3: 0
  Disassembly: not r1 r0 
  Cycle: 4 State:PC:4 Z: 0 R0: 0 R1: fe R2: 0 R3: 0
  Disassembly: add r1 r1 r1 
  Cycle: 5 State:PC:5 Z: 0 R0: 0 R1: 1 R2: 0 R3: 0
  Disassembly: not r1 r1 
  
