##===================================================================================================================
## Makefile -- wrapper for tup to make the assembler executable
##
##	Other targets will be added here to perform other functions.
##
##      Date     Tracker  Version  Description
##  -----------  -------  -------  ---------------------------------------------------------------------------------
##  2023-Feb-28  Initial  v0.0.1   Initial Version
##===================================================================================================================


all:
	tup

test: all
	cd 16bcfs-asm && make -f makefile test


