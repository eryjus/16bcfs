//===================================================================================================================
//  16bcfs.hh -- This is the singular header file to include for the entire project
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-24  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


//
// -- conditionally compile the emulator for a pedantic copy of the EEPROM into SRAM
//    ------------------------------------------------------------------------------
#define PEDANTIC_COPY 0


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
#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtCore/QSettings>
#include <QtGui/QPalette>
#include <QtGui/QPicture>
#include <QtGui/QPixmap>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>


//
// --  forward declaration of all classes
//     ----------------------------------
class IC_25lc256_t;
class IC_74xx00_t;
class IC_74xx02_t;
class IC_74xx03_t;
class IC_74xx04_t;
class IC_74xx08_t;
class IC_74xx30_t;
class IC_74xx32_t;
class IC_74xx74_t;
class IC_74xx86_t;
class IC_74xx138_t;
class IC_74xx151_t;
class IC_74xx157_t;
class IC_74xx164_t;
class IC_74xx165_t;
class IC_74xx193_t;
class IC_74xx238_t;
class IC_74xx283_t;
class IC_74xx540_t;
class IC_74xx541_t;
class IC_74xx574_t;
class IC_as6c62256_t;
class IC_at28c256_t;

class HW_Alu_t;
class HW_AluAdder_t;
class HW_Bus_1_t;
class HW_Bus_8_t;
class HW_Bus_16_t;
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
class GUI_SettingsDialog_t;

class SubDemux3_t;
class SubDemux4_t;
class SubDemux7_t;

class AluFlagsModule_t;
class ClockModule_t;
class CtrlRomCtrlModule_t;
class CtrlRomModule_t;
class FetchRegisterModule_t;
class InstructionRegisterModule_t;
class GpRegisterModule_t;
class PgmRomModule_t;

class CtrlRomModule_t;


//
// -- I cannot see a way around this, app needs to be a global variable
//    -----------------------------------------------------------------
extern GUI_Application_t *app;
const QString key = "control-rom/folder";   // -- I expect the linker to handle the duplicate constants here
const QString lastPgm = "pgm-rom/last-pgm";


//
// -- project speific include files here
//    ----------------------------------
#include "ic/ic-74xx00.hh"
#include "ic/ic-74xx02.hh"
#include "ic/ic-74xx03.hh"
#include "ic/ic-74xx04.hh"
#include "ic/ic-74xx08.hh"
#include "ic/ic-74xx30.hh"
#include "ic/ic-74xx32.hh"
#include "ic/ic-74xx74.hh"
#include "ic/ic-74xx86.hh"
#include "ic/ic-74xx138.hh"
#include "ic/ic-74xx151.hh"
#include "ic/ic-74xx157.hh"
#include "ic/ic-74xx164.hh"
#include "ic/ic-74xx165.hh"
#include "ic/ic-74xx193.hh"
#include "ic/ic-74xx238.hh"
#include "ic/ic-74xx283.hh"
#include "ic/ic-74xx540.hh"
#include "ic/ic-74xx541.hh"
#include "ic/ic-74xx574.hh"
#include "ic/ic-as6c62256.hh"
#include "ic/ic-25lc256.hh"
#include "ic/ic-at28c256.hh"

#include "sub/sub-demux-3.hh"
#include "sub/sub-demux-4.hh"
#include "sub/sub-demux-7.hh"

#include "mod/mod-alu-flags.hh"
#include "mod/mod-clock.hh"
#include "mod/mod-ctrl-ctrl.hh"
#include "mod/mod-ctrl-rom.hh"
#include "mod/mod-fetch-register.hh"
#include "mod/mod-instr-register.hh"
#include "mod/mod-gp-register.hh"
#include "mod/mod-pgm-rom.hh"

#include "planes/ctrl-mid-plane.hh"

#include "hw/hw-alu.hh"
#include "hw/hw-alu-adder.hh"
#include "hw/hw-bus-1.hh"
#include "hw/hw-bus-8.hh"
#include "hw/hw-bus-16.hh"
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
#include "gui/gui-settings-dialog.hh"


#define CNN_TYPE ((Qt::ConnectionType)(Qt::DirectConnection | Qt::UniqueConnection))

