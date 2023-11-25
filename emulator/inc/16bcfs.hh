//===================================================================================================================
//  16bcfs.hh -- This is the singular header file to include for the entire project
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-24  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


//
// -- want to use this macro to set the number of pins properly
//    ---------------------------------------------------------
#define PIN_CNT(x) ((x)+1)


//
// -- some hinting for the compiler
//    -----------------------------
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)


//
// -- c/c++ standard include files here
//    ---------------------------------
typedef enum {
    LOW = 0,
    GND = 0,
    HIGH = 1,
    VCC = 1,
    Z = -1,
} TriState_t;


//
// -- Qt6 include files here
//    ----------------------
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtGui/QPalette>
#include <QtGui/QPicture>
#include <QtGui/QPixmap>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QSlider>


//
// --  forward declaration of all classes
//     ----------------------------------
class IC_74xx00_t;
class IC_74xx02_t;
class IC_74xx08_t;
class IC_74xx32_t;
class IC_74xx74_t;
class IC_74xx138_t;
class IC_74xx193_t;
class IC_74xx283_t;
class IC_74xx540_t;
class IC_74xx541_t;
class IC_74xx574_t;
class HW_Alu_t;
class HW_AluAdder_t;
class HW_Bus_t;
class HW_BusDriver_t;
class HW_Computer_t;
class HW_MomentarySwitch_t;
class HW_Oscillator_t;
class HW_Pot_t;
class HW_SpdtSwitch_t;
class GUI_Application_t;
class GUI_BusLeds_t;
class GUI_BusTester_t;
class GUI_DipSwitch_t;
class GUI_Led_t;
class ClockModule_t;



//
// -- project speific include files here
//    ----------------------------------
#include "ic/ic-74xx00.hh"
#include "ic/ic-74xx02.hh"
#include "ic/ic-74xx08.hh"
#include "ic/ic-74xx32.hh"
#include "ic/ic-74xx74.hh"
#include "ic/ic-74xx138.hh"
#include "ic/ic-74xx193.hh"
#include "ic/ic-74xx283.hh"
#include "ic/ic-74xx540.hh"
#include "ic/ic-74xx541.hh"
#include "ic/ic-74xx574.hh"
#include "hw/hw-alu.hh"
#include "hw/hw-alu-adder.hh"
#include "hw/hw-bus.hh"
#include "hw/hw-bus-driver.hh"
#include "hw/hw-computer.hh"
#include "hw/hw-momentary-switch.hh"
#include "hw/hw-oscillator.hh"
#include "hw/hw-pot.hh"
#include "hw/hw-spdt-switch.hh"
#include "gui/gui-application.hh"
#include "gui/gui-bus-leds.hh"
#include "gui/gui-bus-tester.hh"
#include "gui/gui-dip-switch.hh"
#include "gui/gui-led.hh"
#include "mod/mod-clock.hh"


