# 16-Bit Computer From Scratch

This repository contains the complete works associated with the 16-Bit Computer From Scrath (16bcfs) project.


---

## Videos

I am documenting my 16-Bit Computer build in video form.  You can find the entire playlist [here](https://www.youtube.com/playlist?list=PLUF7WfBe0k3g9wpTtg41QP3jl5y8T1aGV).


---

## License

See [LICENSE.md](LICENSE.md).


---

## Build System

I use `tup` as my primary build system.  I usually will wrap `tup` in `make` commands.  You can find `tup` [here](https://gittup.org/tup/).  I simply find `tup` to more reliable detect changed sources with less work.


### Qt6_DIR Environment Variable

The `Qt6_DIR` Environment Variable needs to point to the installation location of Qt6, for example `~/Qt/6.6.0/gcc_64/`.  Your installation location may be different, so copy-and-paste is not advised.

---

## EEPROM Programmer

I do not have a commercial EEPROM programmer.  I use the TommyPROM programmer.  You can find all the relevant information on TommyPROM [here](https://tomnisbet.github.io/TommyPROM/).  In Linux, I use `minicom` as the interface.  Hint: use `sudo minicom -s` to set up the defaults.



