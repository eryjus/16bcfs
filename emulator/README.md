# 16-Bit Computer From Scratch -- Emulator

This is the source code that supports my *16-Bit Computer From Scratch* project.  This project in particular is the assembler for the 16-Bit Computer.


---

## Videos

I am documenting my 16-Bit Computer build in video form.  You can find the entire playlist [here](https://www.youtube.com/playlist?list=PLUF7WfBe0k3g9wpTtg41QP3jl5y8T1aGV).


---

## License

See [LICENSE.md](LICENSE.md).


---

## Build System

I use `tup` as my primary build system.  I usually will wrap `tup` in `make` commands.  You can find `tup` [here](https://gittup.org/tup/).  I simply find `tup` to more reliable detect changed sources with less work.


--

## Application Framework

The application framework for this emulator is [Qt6.6](https://www.qt.io/).  Please be aware that there is a very steep learning curve with this application framework.  The documentation, while technically complete enough, really lacks substance when looking for the following:
* You believe that something would logically be included in the framework but are not sure what it to call it.
* You are browsing for capabilities in the hundreds of classes, but brief descriptions are missing from the "all classes" documentation
* You would like to see the exact definition and meaning of an attribute and how it is used for a certain class or sub-classes (too many impressions of the reader knowledge are assumed)

That is not to say the framework is not capable or unworthy.  Quite the opposite.  It is mature and very capable.  However, finding what you need to code a solution is rather difficult, typically requiring several Google searches and a plethora of web pages opened.  The project needs a good technical writer.

Please also keep in mind that I am not using the bundled build tools (see Build System).  I want the details.

This project assumes that Qt is installed in your $HOME folder in `Qt`.



