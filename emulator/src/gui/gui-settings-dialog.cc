//===================================================================================================================
//  gui-settings-dialog.cc -- The Settings Dialog box
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-14  Initial  v0.0.1   Initial version
//===================================================================================================================



#include "16bcfs.hh"
#include "../moc/gui-settings-dialog.moc.cc"



//
// -- clean up and destroy the dialog box
//    -----------------------------------
GUI_SettingsDialog_t::~GUI_SettingsDialog_t()
{
    if (result() == QDialog::Accepted) {
        QSettings *settings = HW_Computer_t::GetSettings();

        settings->setValue(key, dir->text());
        settings->sync();
    }
}



//
// -- Initialize the Settings Dialog Box
//    ----------------------------------
void GUI_SettingsDialog_t::Init(void)
{
    QGridLayout *layout = new QGridLayout;
    QSettings *settings = HW_Computer_t::GetSettings();


    QLabel *dirLabel = new QLabel("Control ROM Folder:");
    layout->addWidget(dirLabel, 0, 0, 1, 4);

    ctrlDir = settings->value(key).toString();
    dir = new QLineEdit(ctrlDir);
    dir->setMinimumWidth(300);
    layout->addWidget(dir, 1, 0, 1, 3);

    QPushButton *folder = new QPushButton("...");
    folder->setFixedWidth(15);
    layout->addWidget(folder, 1, 3, 1, 1);
    connect(folder, &QPushButton::pressed, this, &GUI_SettingsDialog_t::ProcessSelectDir);


    QPushButton *ok = new QPushButton("OK");
    ok->setDefault(true);
    ok->setFixedWidth(50);
    layout->addWidget(ok, 4, 2, 1, 1);
    connect(ok, &QPushButton::clicked, this, &QDialog::accept);


    QPushButton *cancel = new QPushButton("Cancel");
    cancel->setFixedWidth(50);
    layout->addWidget(cancel, 4, 3, 1, 1);
    connect(cancel, &QPushButton::clicked, this, &QDialog::reject);

    setLayout(layout);
    setMinimumWidth(400);
}



//
// -- Process the slot to select the directory
//    ----------------------------------------
void GUI_SettingsDialog_t::ProcessSelectDir(void)
{
    QString selected = QFileDialog::getExistingDirectory(this, tr("Select Control Logic Directory"),
            (ctrlDir == "" ? "." : ctrlDir), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (selected == "") return;

    ctrlDir = selected;
    dir->setText(selected);
}


