//===================================================================================================================
//  mod-pgm-rom.cc -- This class represents the entirety of the Program ROM Module
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Aug-18  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/mod-pgm-rom.moc.cc"


//
// -- construct a new General Purpose Register
//    ----------------------------------------
PgmRomModule_t::PgmRomModule_t(const QString &f) : QGroupBox(QString("Mem: " + f))
{
    folder = f;

    setFixedWidth(230);
    setFixedHeight(50);

    AllocateComponents();
    BuildGui();
    WireUp();
    TriggerFirstUpdate();
}



//
// -- allocate all the components needed for this module
//    --------------------------------------------------
void PgmRomModule_t::AllocateComponents(void)
{
    lsb = new IC_at28c256_t(folder + "/lsb.bin");
    msb = new IC_at28c256_t(folder + "/msb.bin");
    nand1 = new IC_74xx00_t;

    bit0 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit1 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit2 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit3 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit4 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit5 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit6 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit7 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit8 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit9 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bitA = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bitB = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bitC = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bitD = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bitE = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bitF = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
}



//
// -- Put the GUI components together
//    -------------------------------
void PgmRomModule_t::BuildGui(void)
{
    // -- the layout for this module will be a grid
    QHBoxLayout *contentsLayout = new QHBoxLayout;
    contentsLayout->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    contentsLayout->setContentsMargins(0, 0, 0, 0);
    setAlignment(Qt::AlignHCenter);
    contentsLayout->setSpacing(1);
    setLayout(contentsLayout);


    contentsLayout->addWidget(bitF, Qt::AlignHCenter);
    contentsLayout->addWidget(bitE, Qt::AlignHCenter);
    contentsLayout->addWidget(bitD, Qt::AlignHCenter);
    contentsLayout->addWidget(bitC, Qt::AlignHCenter);
    contentsLayout->addSpacerItem(new QSpacerItem(4, 10));
    contentsLayout->addWidget(bitB, Qt::AlignHCenter);
    contentsLayout->addWidget(bitA, Qt::AlignHCenter);
    contentsLayout->addWidget(bit9, Qt::AlignHCenter);
    contentsLayout->addWidget(bit8, Qt::AlignHCenter);
    contentsLayout->addSpacerItem(new QSpacerItem(4, 10));
    contentsLayout->addWidget(bit7, Qt::AlignHCenter);
    contentsLayout->addWidget(bit6, Qt::AlignHCenter);
    contentsLayout->addWidget(bit5, Qt::AlignHCenter);
    contentsLayout->addWidget(bit4, Qt::AlignHCenter);
    contentsLayout->addSpacerItem(new QSpacerItem(4, 10));
    contentsLayout->addWidget(bit3, Qt::AlignHCenter);
    contentsLayout->addWidget(bit2, Qt::AlignHCenter);
    contentsLayout->addWidget(bit1, Qt::AlignHCenter);
    contentsLayout->addWidget(bit0, Qt::AlignHCenter);
}



//
// -- Trigger all the proper initial updates
//    --------------------------------------
void PgmRomModule_t::TriggerFirstUpdate(void)
{
    lsb->TriggerFirstUpdate();
    msb->TriggerFirstUpdate();
    nand1->TriggerFirstUpdate();
}




//
// -- complete the wire-up for this module
//    ------------------------------------
void PgmRomModule_t::WireUp(void)
{
    // -- We only need 1 inverter, but we will use a less expensive IC here
    nand1->ProcessA2Low();
    nand1->ProcessA3Low();
    nand1->ProcessA4Low();
    nand1->ProcessB2Low();
    nand1->ProcessB3Low();
    nand1->ProcessB4Low();


    // -- for the lsb and the msb ICs, wire up the inputs
    HW_Bus_16_t *addr1 = HW_Computer_t::GetAddr1Bus();

    connect(nand1, &IC_74xx00_t::SignalY1Updated, lsb, &IC_at28c256_t::ProcessUpdateCE);
    connect(addr1, &HW_Bus_16_t::SignalBit0Updated, lsb, &IC_at28c256_t::ProcessUpdateA0);
    connect(addr1, &HW_Bus_16_t::SignalBit1Updated, lsb, &IC_at28c256_t::ProcessUpdateA1);
    connect(addr1, &HW_Bus_16_t::SignalBit2Updated, lsb, &IC_at28c256_t::ProcessUpdateA2);
    connect(addr1, &HW_Bus_16_t::SignalBit3Updated, lsb, &IC_at28c256_t::ProcessUpdateA3);
    connect(addr1, &HW_Bus_16_t::SignalBit4Updated, lsb, &IC_at28c256_t::ProcessUpdateA4);
    connect(addr1, &HW_Bus_16_t::SignalBit5Updated, lsb, &IC_at28c256_t::ProcessUpdateA5);
    connect(addr1, &HW_Bus_16_t::SignalBit6Updated, lsb, &IC_at28c256_t::ProcessUpdateA6);
    connect(addr1, &HW_Bus_16_t::SignalBit7Updated, lsb, &IC_at28c256_t::ProcessUpdateA7);
    connect(addr1, &HW_Bus_16_t::SignalBit8Updated, lsb, &IC_at28c256_t::ProcessUpdateA8);
    connect(addr1, &HW_Bus_16_t::SignalBit9Updated, lsb, &IC_at28c256_t::ProcessUpdateA9);
    connect(addr1, &HW_Bus_16_t::SignalBitAUpdated, lsb, &IC_at28c256_t::ProcessUpdateA10);
    connect(addr1, &HW_Bus_16_t::SignalBitBUpdated, lsb, &IC_at28c256_t::ProcessUpdateA11);
    connect(addr1, &HW_Bus_16_t::SignalBitCUpdated, lsb, &IC_at28c256_t::ProcessUpdateA12);
    connect(addr1, &HW_Bus_16_t::SignalBitDUpdated, lsb, &IC_at28c256_t::ProcessUpdateA13);
    connect(addr1, &HW_Bus_16_t::SignalBitEUpdated, lsb, &IC_at28c256_t::ProcessUpdateA14);
    // -- OE handled in Process Fetch Suppress


    connect(nand1, &IC_74xx00_t::SignalY1Updated, msb, &IC_at28c256_t::ProcessUpdateCE);
    connect(addr1, &HW_Bus_16_t::SignalBit0Updated, msb, &IC_at28c256_t::ProcessUpdateA0);
    connect(addr1, &HW_Bus_16_t::SignalBit1Updated, msb, &IC_at28c256_t::ProcessUpdateA1);
    connect(addr1, &HW_Bus_16_t::SignalBit2Updated, msb, &IC_at28c256_t::ProcessUpdateA2);
    connect(addr1, &HW_Bus_16_t::SignalBit3Updated, msb, &IC_at28c256_t::ProcessUpdateA3);
    connect(addr1, &HW_Bus_16_t::SignalBit4Updated, msb, &IC_at28c256_t::ProcessUpdateA4);
    connect(addr1, &HW_Bus_16_t::SignalBit5Updated, msb, &IC_at28c256_t::ProcessUpdateA5);
    connect(addr1, &HW_Bus_16_t::SignalBit6Updated, msb, &IC_at28c256_t::ProcessUpdateA6);
    connect(addr1, &HW_Bus_16_t::SignalBit7Updated, msb, &IC_at28c256_t::ProcessUpdateA7);
    connect(addr1, &HW_Bus_16_t::SignalBit8Updated, msb, &IC_at28c256_t::ProcessUpdateA8);
    connect(addr1, &HW_Bus_16_t::SignalBit9Updated, msb, &IC_at28c256_t::ProcessUpdateA9);
    connect(addr1, &HW_Bus_16_t::SignalBitAUpdated, msb, &IC_at28c256_t::ProcessUpdateA10);
    connect(addr1, &HW_Bus_16_t::SignalBitBUpdated, msb, &IC_at28c256_t::ProcessUpdateA11);
    connect(addr1, &HW_Bus_16_t::SignalBitCUpdated, msb, &IC_at28c256_t::ProcessUpdateA12);
    connect(addr1, &HW_Bus_16_t::SignalBitDUpdated, msb, &IC_at28c256_t::ProcessUpdateA13);
    connect(addr1, &HW_Bus_16_t::SignalBitEUpdated, msb, &IC_at28c256_t::ProcessUpdateA14);
    // -- OE handled in Process Fetch Suppress


    // -- connect the outputs to the fetch bus
    HW_Bus_16_t *fetch = HW_Computer_t::GetFetchBus();
    connect(lsb, &IC_at28c256_t::SignalDq0Updated, fetch, &HW_Bus_16_t::ProcessUpdateBit0);
    connect(lsb, &IC_at28c256_t::SignalDq1Updated, fetch, &HW_Bus_16_t::ProcessUpdateBit1);
    connect(lsb, &IC_at28c256_t::SignalDq2Updated, fetch, &HW_Bus_16_t::ProcessUpdateBit2);
    connect(lsb, &IC_at28c256_t::SignalDq3Updated, fetch, &HW_Bus_16_t::ProcessUpdateBit3);
    connect(lsb, &IC_at28c256_t::SignalDq4Updated, fetch, &HW_Bus_16_t::ProcessUpdateBit4);
    connect(lsb, &IC_at28c256_t::SignalDq5Updated, fetch, &HW_Bus_16_t::ProcessUpdateBit5);
    connect(lsb, &IC_at28c256_t::SignalDq6Updated, fetch, &HW_Bus_16_t::ProcessUpdateBit6);
    connect(lsb, &IC_at28c256_t::SignalDq7Updated, fetch, &HW_Bus_16_t::ProcessUpdateBit7);

    connect(msb, &IC_at28c256_t::SignalDq0Updated, fetch, &HW_Bus_16_t::ProcessUpdateBit8);
    connect(msb, &IC_at28c256_t::SignalDq1Updated, fetch, &HW_Bus_16_t::ProcessUpdateBit9);
    connect(msb, &IC_at28c256_t::SignalDq2Updated, fetch, &HW_Bus_16_t::ProcessUpdateBitA);
    connect(msb, &IC_at28c256_t::SignalDq3Updated, fetch, &HW_Bus_16_t::ProcessUpdateBitB);
    connect(msb, &IC_at28c256_t::SignalDq4Updated, fetch, &HW_Bus_16_t::ProcessUpdateBitC);
    connect(msb, &IC_at28c256_t::SignalDq5Updated, fetch, &HW_Bus_16_t::ProcessUpdateBitD);
    connect(msb, &IC_at28c256_t::SignalDq6Updated, fetch, &HW_Bus_16_t::ProcessUpdateBitE);
    connect(msb, &IC_at28c256_t::SignalDq7Updated, fetch, &HW_Bus_16_t::ProcessUpdateBitF);


    // -- finally connect up the LEDs
    connect(lsb, &IC_at28c256_t::SignalDq0Updated, bit0, &GUI_Led_t::ProcessStateChange);
    connect(lsb, &IC_at28c256_t::SignalDq1Updated, bit1, &GUI_Led_t::ProcessStateChange);
    connect(lsb, &IC_at28c256_t::SignalDq2Updated, bit2, &GUI_Led_t::ProcessStateChange);
    connect(lsb, &IC_at28c256_t::SignalDq3Updated, bit3, &GUI_Led_t::ProcessStateChange);
    connect(lsb, &IC_at28c256_t::SignalDq4Updated, bit4, &GUI_Led_t::ProcessStateChange);
    connect(lsb, &IC_at28c256_t::SignalDq5Updated, bit5, &GUI_Led_t::ProcessStateChange);
    connect(lsb, &IC_at28c256_t::SignalDq6Updated, bit6, &GUI_Led_t::ProcessStateChange);
    connect(lsb, &IC_at28c256_t::SignalDq7Updated, bit7, &GUI_Led_t::ProcessStateChange);
    connect(msb, &IC_at28c256_t::SignalDq0Updated, bit8, &GUI_Led_t::ProcessStateChange);
    connect(msb, &IC_at28c256_t::SignalDq1Updated, bit9, &GUI_Led_t::ProcessStateChange);
    connect(msb, &IC_at28c256_t::SignalDq2Updated, bitA, &GUI_Led_t::ProcessStateChange);
    connect(msb, &IC_at28c256_t::SignalDq3Updated, bitB, &GUI_Led_t::ProcessStateChange);
    connect(msb, &IC_at28c256_t::SignalDq4Updated, bitC, &GUI_Led_t::ProcessStateChange);
    connect(msb, &IC_at28c256_t::SignalDq5Updated, bitD, &GUI_Led_t::ProcessStateChange);
    connect(msb, &IC_at28c256_t::SignalDq6Updated, bitE, &GUI_Led_t::ProcessStateChange);
    connect(msb, &IC_at28c256_t::SignalDq7Updated, bitF, &GUI_Led_t::ProcessStateChange);
}


