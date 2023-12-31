//===================================================================================================================
//  client.cc -- this is the source file for the eeprom programmer client
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Dec-23  Initial  v0.0.1   Initial version
//===================================================================================================================



//
// -- Some facts for this program
//    ---------------------------
#define VER     QString("0.0.1")
#define SIZE    (32 * 1024)
// -- !!! This device name is hard-coded at the moment
#define DEV     QString("/dev/ttyUSB0")



#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <poll.h>



#include "QtWidgets/QApplication"
#include "QtWidgets/QMainWindow"
#include "QtWidgets/QTextEdit"
#include "QtWidgets/QGridLayout"
#include "QtWidgets/QWidget"
#include "QtWidgets/QLineEdit"
#include "QtWidgets/QLabel"
#include "QtWidgets/QStatusBar"
#include "QtWidgets/QMenuBar"
#include "QtWidgets/QFileDialog"
#include "QtCore/QTimer"



//
// -- I need my own version of a main window
//    --------------------------------------
class MainWindow : public QMainWindow {
    Q_OBJECT


// -- public members
public:
    static QApplication *app;
    static MainWindow *mWin;


// -- private members
private:
    QTextEdit *log;
    QLineEdit *line;

    QMenu *fileMenu;
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionExit;

    bool bufferDirty;
    QString filename;

    QByteArray *buffer;

    QTimer *timer;

    int fdDev;
    int fdMax;
    bool xmit;


public:
    MainWindow(QApplication *a);
    virtual ~MainWindow() {}

public:
    void MakeWindow(void);
    static void Cleanup(MainWindow *mWin);
    static void SignalHandler(int sig);


private:
    void CreateActions(void);
    void CreateMenus(void);


public slots:
    void SetTitle(void);
    void NewBinary(void);
    void OpenBinary(void);

    void OnIdle(void);

    void ProcessInput(void);
};



//
// -- This is the main window, which needs to be globally visible
//    -----------------------------------------------------------
MainWindow *MainWindow::mWin = nullptr;
QApplication *MainWindow::app = nullptr;



//
// --  Handle the Ctrl-C to clean up properly
//     --------------------------------------
void MainWindow::SignalHandler(int sig)
{
    exit(EXIT_SUCCESS);
}



//
// -- On normal exit, use this function to clean up
//    ---------------------------------------------
void Cleanup(void) { MainWindow::Cleanup(MainWindow::mWin); }
void MainWindow::Cleanup(MainWindow *win)
{
    if (win->fdDev != -1) ::close(win->fdDev);
    win->fdDev = -1;
}



//
// -- MainWindow class constructor
//    ----------------------------
MainWindow::MainWindow(QApplication *a) : QMainWindow(), fdDev(-1), fdMax(0)
{
    app = a;
    xmit = false;

    buffer = new QByteArray(SIZE, 0); bufferDirty = false;

    // -- Open the device, read/write, not the controlling tty, and non-blocking I/O
    fdDev = open(DEV.toStdString().c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (fdDev == -1) exit(EXIT_FAILURE);

    // -- must be a tty
    if (!isatty(fdDev)) exit(EXIT_FAILURE);


    // -- Set fdDev non-blocking
    if (fcntl(fdDev, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl()");
        ::close(fdDev);
        exit(EXIT_FAILURE);
    }

    fdMax = (fdDev>STDIN_FILENO?fdDev+1:STDIN_FILENO+1);

    // -- Get the attributes
    struct termios termios;     // -- The termios structure, to be configured for serial interface
    if(tcgetattr(fdDev, &termios) == -1) {
        perror("Failed to get attributes of device");
        exit(EXIT_FAILURE);
    }

    // -- use polling
    termios.c_cc[VTIME] = 0;
    termios.c_cc[VMIN] = 0;

    // -- 8N1 mode, no input/output/line processing masks
    termios.c_iflag = 0;
    termios.c_oflag = 0;
    termios.c_cflag = CS8 | CREAD | CLOCAL;
    termios.c_lflag = 0;

    // -- Set the baud rate
    if((cfsetispeed(&termios, B115200) < 0) || (cfsetospeed(&termios, B115200) < 0)) {
        perror("Failed to set baud-rate");
        exit(EXIT_FAILURE);
    }

    // -- Write the attributes
    if (tcsetattr(fdDev, TCSAFLUSH, &termios) == -1) {
        perror("tcsetattr()");
        exit(EXIT_FAILURE);
    }

    timer = new QTimer;
    timer->setInterval(0);
    timer->start();
    connect(timer, &QTimer::timeout, this, &MainWindow::OnIdle);
}


//
// -- Construct all the widgets in the main window
//    --------------------------------------------
void MainWindow::MakeWindow(void)
{
    setMinimumSize(800, 600);

    QFont *textFont = new QFont;
    textFont->setFamily("monospace [Consolas]");
    textFont->setFixedPitch(true);
    textFont->setStyleHint(QFont::TypeWriter);

    log = new QTextEdit;
    log->setFont(*textFont);
    log->setReadOnly(true);
    log->insertPlainText("Welcome to the SPI EEPROM Programmer Client\n");
    log->insertPlainText("  Please log issues at https://github.com/eryjus/16bcfs\n\n");

    line = new QLineEdit;

    QWidget *central = new QWidget;

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel("Log: "), 0, 0, 1, 2);
    layout->addWidget(log, 1, 0, 1, 2);
    layout->addWidget(new QLabel("Cmd: "), 2, 0);
    layout->addWidget(line, 2, 1);

    central->setLayout(layout);
    setCentralWidget(central);

    CreateActions();
    CreateMenus();
    statusBar()->showMessage("Welcome!  Please log issues at https://github.com/eryjus/16bcfs");

    connect(line, &QLineEdit::returnPressed, this, &MainWindow::ProcessInput);

    line->setFocus();
}



//
// -- Create the Actions needed for the menu
//    --------------------------------------
void MainWindow::CreateActions(void)
{
    // -- new action
    actionNew = new QAction("&New", this);
    actionNew->setShortcuts(QKeySequence::New);
    actionNew->setStatusTip("Create a new Binary");
    connect(actionNew, &QAction::triggered, this, &MainWindow::NewBinary);

    // -- open action
    actionOpen = new QAction("&Open", this);
    actionOpen->setShortcuts(QKeySequence::Open);
    actionOpen->setStatusTip("Open a Binary File from Disk");
    connect(actionOpen, &QAction::triggered, this, &MainWindow::OpenBinary);

    // -- exit action
    actionExit = new QAction("&Quit", this);
    actionExit->setShortcuts(QKeySequence::Quit);
    actionExit->setStatusTip("Quit the application");
    connect(actionExit, &QAction::triggered, app, &QApplication::quit);
}



//
// -- Create the Menu structure for the application
//    ---------------------------------------------
void MainWindow::CreateMenus(void)
{
    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(actionNew);
    fileMenu->addAction(actionOpen);
    fileMenu->addSeparator();
    fileMenu->addAction(actionExit);
}



//
// -- Create a new binary for interacting with the EEPROM programmer
//    --------------------------------------------------------------
void MainWindow::NewBinary(void)
{
// -- TODO: check to save any existing binary which has been changed
    delete buffer;
    buffer = new QByteArray(SIZE, 0);
    bufferDirty = false;
    filename = "";
    SetTitle();
}


//
// -- Open an existing Binary from disk and load its contents
//    -------------------------------------------------------
void MainWindow::OpenBinary(void)
{
// -- TODO: check to save any existing binary which has been changed
    delete buffer;
    buffer = new QByteArray(SIZE, 0);

    QString name = QFileDialog::getOpenFileName(this, "Open EEPROM Binary");
    if (name.isNull()) return;

    filename = name;
    bufferDirty = false;

    QFile file(filename);
    file.open(QIODeviceBase::ReadOnly | QIODeviceBase::ExistingOnly);
    *buffer = file.readAll();
    file.close();

    SetTitle();
}



//
// -- Maintain the current window title
//    ---------------------------------
inline void MainWindow::SetTitle(void)
{
    QString title = QString(bufferDirty?"*":"");
    title += (filename.isEmpty()?QString("Untitled"):filename);
    title += QString("  -  SPI EEPROM Programmer Client (Ver: ");
    title += VER;
    title += QString(")");
    setWindowTitle(title);
}


//
// -- When we are idle, poll the tty for new input
//    --------------------------------------------
void MainWindow::OnIdle(void)
{
    if (xmit) return;

    struct pollfd fds = {
        .fd = fdDev,
        .events = POLL_IN,
        .revents = 0
    };

    // -- set up to use poll to see if we have any input to process
    if (!poll(&fds, 1, 0)) return;

    if (fds.revents & POLL_IN) {
        const int BUF_SIZE = 1024;      // -- create a modest buffer for TTY data
        char buf[BUF_SIZE];

        ssize_t len = read(fdDev, buf, BUF_SIZE);        // read as much as we can

        if (len < 0) return;
        buf[len] = 0;
        log->insertPlainText(buf);
        log->moveCursor(QTextCursor::End);
    }
}



//
// -- Process input from the UI
//    -------------------------
void MainWindow::ProcessInput(void)
{
    QString entry = line->text();
    if (entry.length() == 0) return;

    const char newline[] = { '\n', 0};

    if (write(fdDev, entry.toStdString().c_str(), entry.length()) < 0) {
        perror("Writing to tty");
    }

    if (write(fdDev, newline, 1) < 0) {
        perror("Writing to tty");
    }

    line->clear();
}



//
// -- The main entry point for the application
//    ----------------------------------------
int main(int argc, char *argv[])
{
    // -- set up for a clean exit; from this point on will reset terminal settings
    atexit(Cleanup);
    signal(SIGINT, &MainWindow::SignalHandler);

    MainWindow::app = new QApplication(argc, argv);
    MainWindow::mWin = new MainWindow(MainWindow::app);

    MainWindow::mWin->MakeWindow();

    MainWindow::mWin->SetTitle();
    MainWindow::mWin->show();

    return MainWindow::app->exec();
}



//
// -- finally, include the Qt6-generated code
//    ---------------------------------------
#include "client.moc"

