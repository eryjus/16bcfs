# 16-Bit Computer From Scratch -- Control Logic

This is the source code that supports my *16-Bit Computer From Scratch* project.  This project in particular is the breadboard control logic for the computer.  The PCB control logic will be implemented using other methods.


---

## Videos

I am documenting my 16-Bit Computer build in video form.  You can find the entire playlist [here](https://www.youtube.com/playlist?list=PLUF7WfBe0k3g9wpTtg41QP3jl5y8T1aGV).


---

## License

See [LICENSE.md](LICENSE.md).


---

## Build System

I use `tup` as my primary build system.  I usually will wrap `tup` in `make` commands.  You can find `tup` [here](https://gittup.org/tup/).  I simply find `tup` to more reliable detect changed sources with less work.


---

## EEPROM Programmer

I do not have a commercial EEPROM programmer.  I use the TommyPROM programmer.  You can find all the relevant information on TommyPROM [here](https://tomnisbet.github.io/TommyPROM/).  In Linux, I use `minicom` as the interface.  Hint: use `sudo minicom -s` to set up the defaults.


