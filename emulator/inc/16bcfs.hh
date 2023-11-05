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
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtGui/QPalette>
#include <QtGui/QPicture>
#include <QtGui/QPixmap>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDial>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>


//
// --  forward declaration of all classes
//     ----------------------------------
class HW_Clock4Phase_t;
class HW_Computer_t;
class HW_Oscillator_t;
class HW_TriStateLatch_t;
class IC_74xx00_t;
class IC_74xx02_t;
class IC_74xx08_t;
class IC_74xx32_t;
class IC_74xx138_t;
class IC_74xx193_t;
class IC_74xx574_t;
class GUI_Application_t;
class GUI_Clock4PhaseGroup_t;
class GUI_ClockFunctionGroup_t;
class GUI_ClockSpeed_t;
class GUI_EmulationWindow_t;
class GUI_Led_t;
class GUI_OscillatorGroup_t;
class GUI_StepGroup_t;



//
// -- project speific include files here
//    ----------------------------------
#include "hw/hw-clock4phase.hh"
#include "hw/hw-computer.hh"
#include "hw/hw-oscillator.hh"
#include "hw/hw-tristate-latch.hh"
#include "ic/ic-74xx00.hh"
#include "ic/ic-74xx02.hh"
#include "ic/ic-74xx08.hh"
#include "ic/ic-74xx32.hh"
#include "ic/ic-74xx138.hh"
#include "ic/ic-74xx193.hh"
#include "ic/ic-74xx574.hh"
#include "gui/gui-application.hh"
#include "gui/gui-clock-function-group.hh"
#include "gui/gui-clock4phase-group.hh"
#include "gui/gui-clock-speed.hh"
#include "gui/gui-emu-window.hh"
#include "gui/gui-led.hh"
#include "gui/gui-oscillator-group.hh"
#include "gui/gui-step-group.hh"


