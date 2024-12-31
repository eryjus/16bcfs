//===================================================================================================================
//  gui-settings-dialog.hh -- The Settings Dialog box
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-14  Initial  v0.0.1   Initial version
//===================================================================================================================



#pragma once



//
// -- A dialog box which will handle updating the settings
//    ----------------------------------------------------
class GUI_SettingsDialog_t : public QDialog {
    Q_OBJECT


private:
    QString ctrlDir;
    QLineEdit *dir;


public:
    GUI_SettingsDialog_t(QWidget *parent = nullptr) : QDialog(parent) { Init(); }
    ~GUI_SettingsDialog_t();


private:
    void Init(void);


private slots:
    void ProcessSelectDir(void);
};


