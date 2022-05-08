#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QDebug>
#include <QIterator>
#include <QFile>
#include <QMessageBox>
#include <QIntValidator>
#include <QLineEdit>
#include <QMetaEnum>
#include <QString>
#include <QSignalMapper>
#include <QFont>
#include <QDir>
#include <QToolBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QSaveFile>
#include <QSplitter>
#include "setpointwidget.h"
#include "settingsdialog.h"
#include "kinematics.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) , ui(new Ui::MainWindow), port(nullptr)
{
    ui->setupUi(this);

    //mainwidget
    QSplitter* splitter = new QSplitter(Qt::Orientation::Horizontal, this);
    editor = new CodeEditor;
    setPointWidget = new SetPointWidget;
    splitter->addWidget(editor);
    splitter->addWidget(setPointWidget);
    this->setCentralWidget(splitter);
    //
    connect(this->editor->document(), &QTextDocument::contentsChanged, this, &MainWindow::documentWasModified);
    connect(this->setPointWidget, &SetPointWidget::newPosGcode, this->editor, &CodeEditor::appendPlainText);
    connect(this->setPointWidget, &SetPointWidget::newGripperGcode, this->editor, &CodeEditor::appendPlainText);
    connect(this->setPointWidget->sliderGripper1, &QSlider::valueChanged, this, &MainWindow::sendAngleGripper1);
    connect(this->setPointWidget->sliderVertical2, &QSlider::valueChanged, this, &MainWindow::sendAngleVertical2);
    //connect(this->setPointWidget->sliderHorizontal3, &QSlider::valueChanged, [this,this->setPointWidget->angleVertical2->value(), this->setPointWidget->angleHorizontal3->value()] {sendAngleHorizontal3(angleVertical2->value(),angleHorizontal3->value());});
    connect(this->setPointWidget->sliderHorizontal3, &QSlider::valueChanged, [=] () {sendAngleHorizontal3(this->setPointWidget->sliderVertical2->value(), this->setPointWidget->sliderHorizontal3->value());});
    connect(this->setPointWidget->sliderRotation4, &QSlider::valueChanged, this, &MainWindow::sendAngleRotation4);


    //Fenster anpassen
    QMainWindow::showMaximized();
    QMainWindow::setWindowTitle("Robot Control");
    //verbindungsmenü
    connectionMenu = menuBar()->addMenu("Connection");
    connectionMenu->setToolTipsVisible(true);
    QAction* set = new QAction("Settings", this);
    set->setIcon(QIcon("://settings.ico"));
    set->setIconVisibleInMenu(true);
    connect(set, SIGNAL(triggered()), this, SLOT(openSettings()));
    connectionMenu->addAction(set);
    connect(connectionMenu, SIGNAL(aboutToShow()), this, SLOT(search()));
    connectionMenu->addSeparator();

    //File menü
    //neue datei
    QMenu *fileMenu = menuBar()->addMenu("File");
    QToolBar *fileToolBar = addToolBar(tr("File"));
    QAction *newAct = new QAction(QIcon("://new.png"), "New", this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip("Create a new file");
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);
    //datei öffnen
    QAction *openAct = new QAction(QIcon("://open.png"), "Open...", this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip("Open an existing file");
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);
    //datei speichern
    QAction *saveAct = new QAction(QIcon("://save.png"), "Save", this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip("Save the document to disk");
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);
    //datei speichern als
    QAction *saveAsAct = new QAction("Save As...", this);
    fileMenu->addAction(saveAsAct);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveAs);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip("Save the document under a new name");

    //vor, zurück, kopieren, ausschneiden, einfügen
    QToolBar *tb = addToolBar("Edit Actions");
    QMenu *menu = menuBar()->addMenu("Edit");
    //zurück
    QAction* actionUndo = new QAction(QIcon("://editundo.png"), "Undo", this);
    actionUndo->setShortcut(QKeySequence::Undo);
    connect(actionUndo, &QAction::triggered, this->editor, &CodeEditor::undo);
    tb->addAction(actionUndo);
    menu->addAction(actionUndo);
    //vor
    QAction* actionRedo = new QAction(QIcon("://editredo.png"), "Redo", this);
    actionRedo->setPriority(QAction::LowPriority);
    actionRedo->setShortcut(QKeySequence::Redo);
    connect(actionRedo, &QAction::triggered, this->editor, &CodeEditor::redo);
    tb->addAction(actionRedo);
    menu->addAction(actionRedo);
    menu->addSeparator();
    //ausschneiden
    QAction* actionCut = new QAction(QIcon("://editcut.png"), "cut", this);
    actionCut->setPriority(QAction::LowPriority);
    actionCut->setShortcut(QKeySequence::Cut);
    connect(actionCut, &QAction::triggered, this->editor, &CodeEditor::cut);
    tb->addAction(actionCut);
    menu->addAction(actionCut);
    //kopieren
    QAction* actionCopy = new QAction(QIcon("://editcopy.png"), "copy", this);
    actionCopy->setPriority(QAction::LowPriority);
    actionCopy->setShortcut(QKeySequence::Copy);
    connect(actionCopy, &QAction::triggered, this->editor, &CodeEditor::copy);
    tb->addAction(actionCopy);
    menu->addAction(actionCopy);
    //einfügen
    QAction* actionPaste = new QAction(QIcon("://editpaste.png"), "paste", this);
    actionPaste->setPriority(QAction::LowPriority);
    actionPaste->setShortcut(QKeySequence::Paste);
    connect(actionPaste, &QAction::triggered, this->editor, &CodeEditor::paste);
    tb->addAction(actionPaste);
    menu->addAction(actionPaste);

    //robot run mode
    toolBaarMode = addToolBar("Robot-Run-Mode");
    menuMode = menuBar()->addMenu("Robot-Run-Mode");
    //einmal
    actionOnce = new QAction(QIcon("://once.png"), "Run Once", this);
    //actionUndo->setShortcut(QKeySequence::);
    //connect(actionUndo, &QAction::triggered, this->editor, &CodeEditor::undo);
    toolBaarMode->addAction(actionOnce);
    menuMode->addAction(actionOnce);
    //unendlich
    actionInfinity = new QAction(QIcon("://infinity.png"), "Run Infinity", this);
    //actionUndo->setShortcut(QKeySequence::);
    //connect(actionUndo, &QAction::triggered, this->editor, &CodeEditor::undo);
    toolBaarMode->addAction(actionInfinity);
    menuMode->addAction(actionInfinity);
    //stop
    actionStop = new QAction(QIcon("://stop.png"), "Stop Run", this);
    //actionUndo->setShortcut(QKeySequence::);
    //connect(actionUndo, &QAction::triggered, this->editor, &CodeEditor::undo);
    toolBaarMode->addAction(actionStop);
    menuMode->addAction(actionStop);

    //wenn erlaubt, direkt verinden
    QFile file("config.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,"error",file.errorString() + "\n\nno auto conneciton");
    }
    else{
        QString buffer;
        buffer = file.readLine();
        if (static_cast<Qt::CheckState>(QMetaEnum::fromType<Qt::CheckState>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", ""))) == Qt::Checked) {
            const auto infos = QSerialPortInfo::availablePorts();
            if (infos.size() == 1){
                 //port = new QSerialPort(infos.first().portName());
                 QAction* closeConnection = new QAction("disconnect", this);
                 closeConnection->setIcon(QIcon("://exit.ico"));
                 closeConnection->setIconVisibleInMenu(true);
                 connectionMenu->addAction(closeConnection);
                 connectionList.push_back(closeConnection);
                 connect(closeConnection, SIGNAL(triggered()), this, SLOT(disconnectPort()));
                 connectToPort(infos.first().portName());
            }
            else{
                actionOnce->setDisabled(true);
                actionInfinity->setDisabled(true);
                actionStop->setDisabled(true);
                setPointWidget->setDisabled(true);
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::search(){
    //alle actions löschen
    for (const auto& i : connectionList){
        connectionMenu->removeAction(i);
    }
    qDeleteAll(connectionList);
    connectionList.clear();

    //alle Ports auslesen
    const auto infos = QSerialPortInfo::availablePorts();
    if (infos.empty()){
        QAction* nothing = new QAction("no dvice available", this);
        nothing->setEnabled(false);
        connectionMenu->addAction(nothing);
        connectionList.push_back(nothing);
    }
    else{
        for (const QSerialPortInfo &info : infos) {
            QString deviceInfo = (info.description().isEmpty() ? "" :   QObject::tr("Description: ") + info.description() + "\n")
                               + (info.manufacturer().isEmpty() ? "" :  QObject::tr("Manufacturer: ") + info.manufacturer() + "\n")
                               + (info.serialNumber().isEmpty() ? "" :  QObject::tr("Serial number: ") + info.serialNumber() + "\n")
                               + (info.hasVendorIdentifier() ? "" :     QObject::tr("Vendor Identifier: ") + QString::number(info.vendorIdentifier(), 16) + "\n")
                               + (info.hasProductIdentifier() ? "" :    QObject::tr("Product Identifier: ") + QString::number(info.productIdentifier(), 16) + "\n");

            //gefundene Verbidnung als auswahl eintragen
            QString name = info.portName();
            QAction* detectedDevice = new QAction(name, this);
            detectedDevice->setToolTip(deviceInfo);
            if (port != nullptr) {
                if (port->portName() == name){
                    detectedDevice->setIconVisibleInMenu(true);
                     detectedDevice->setIcon(QIcon("://connection.ico"));
                }
            }
            connectionMenu->addAction(detectedDevice);
            connectionList.push_back(detectedDevice);
            QString text = detectedDevice->text();
            connect(detectedDevice, &QAction::triggered, [this, text] {connectToPort(text);});
        }

        if (port != nullptr){
            QAction* closeConnection = new QAction("disconnect", this);
            closeConnection->setIcon(QIcon("://exit.ico"));
            closeConnection->setIconVisibleInMenu(true);
            connectionMenu->addAction(closeConnection);
            connectionList.push_back(closeConnection);
            connect(closeConnection, SIGNAL(triggered()), this, SLOT(disconnectPort()));
        }
    }
}

void MainWindow::connectToPort(QString portName){
    actionOnce->setDisabled(false);
    actionInfinity->setDisabled(false);
    actionStop->setDisabled(false);
    setPointWidget->setDisabled(false);
    if (port == nullptr){
        port = new QSerialPort(portName, this);
        loadPortSettings();
        if (!port->open(QIODevice::WriteOnly)) {
            QMessageBox::information(0,"error", "Can't open Port");

        }
    }
    else {
        port->setPortName(portName);
    }
}

void MainWindow::loadPortSettings(){
    //serial config lesen
    QFile file("config.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,"error",file.errorString() + "\n\ncant load settings, going on with default");
    }
    else{
        QString buffer;
        buffer = file.readLine();
        buffer = file.readLine();
        port->setBaudRate(static_cast<QSerialPort::BaudRate>(QMetaEnum::fromType<QSerialPort::BaudRate>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", ""))));
        buffer = file.readLine();
        port->setDataBits(static_cast<QSerialPort::DataBits>(QMetaEnum::fromType<QSerialPort::DataBits>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", ""))));
        buffer = file.readLine();
        port->setParity(static_cast<QSerialPort::Parity>(QMetaEnum::fromType<QSerialPort::Parity>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", ""))));
        buffer = file.readLine();
        port->setStopBits(static_cast<QSerialPort::StopBits>(QMetaEnum::fromType<QSerialPort::StopBits>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", ""))));
        buffer = file.readLine();
        port->setFlowControl(static_cast<QSerialPort::FlowControl>(QMetaEnum::fromType<QSerialPort::FlowControl>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", ""))));
        file.close();
    }
}

void MainWindow::disconnectPort(){
    actionOnce->setDisabled(true);
    actionInfinity->setDisabled(true);
    actionStop->setDisabled(true);
    setPointWidget->setDisabled(true);
    port->flush();
    port->close();
    if (port != nullptr){
        delete port;
        port = nullptr;
    }
}

void MainWindow::openSettings(){
    settings = new SettingsDialog();
    settings->deleteLater();
    //connect(settings, QCloseEvent::Q, settings, QDialog::close);
    settings->exec();
}

bool MainWindow::saveFile(const QString &fileName){
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << this->editor->toPlainText();
        if (!file.commit()) {
            errorMessage = tr("Cannot write file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    } else {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                       .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("Application"), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::open(){
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::save(){
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs(){
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}

void MainWindow::documentWasModified(){
    setWindowModified(this->editor->document()->isModified());
}

bool MainWindow::maybeSave(){
    if (!this->editor->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName){
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    this->editor->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::setCurrentFile(const QString &fileName){
    curFile = fileName;
    this->editor->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

QString MainWindow::strippedName(const QString &fullFileName){
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::newFile(){
    if (maybeSave()) {
        this->editor->clear();
        setCurrentFile(QString());
    }
}

void MainWindow::sendAngleGripper1(quint16 angleGripper){
    if (port != nullptr){
        port->write("1" + QByteArray().setNum(angleGripper));
    }
}

void MainWindow::sendAngleVertical2(double angleVertical){
    if (port != nullptr){
        port->write("2" + QByteArray().setNum(q2ToServo(angleVertical)));
    }
}

void MainWindow::sendAngleHorizontal3(double angleVertical, double angleHorizontal){
    if (port != nullptr){
        port->write("3" + QByteArray().setNum(q3ToServo(angleVertical, angleHorizontal)));
    }
}

void MainWindow::sendAngleRotation4(double angleRotation){
    if (port != nullptr){
        port->write("4" + QByteArray().setNum(q1ToServo(angleRotation)));
    }
}

