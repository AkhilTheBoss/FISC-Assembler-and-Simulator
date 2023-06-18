# FISC-Assembler-and-Simulator

I have created an assembler that converts FISC (four instruction set computers) assembly code to hexadecimal. It takes in an assembly file as input and generates an output of hexadecimal values on a separate file. The assembler first converts each assembly instruction to machine code (1's and 0's). The machine code is then converted to a hexadecimal value and written to the output file.

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
  
  
