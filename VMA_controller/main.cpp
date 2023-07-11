#include <QCoreApplication>
#include <QSettings>
#include <QThread>
#include "vma_controller.h"
#include "kx_pult/kx_protocol.h"
#include "kx_pult/qkx_coeffs.h"

const QString ConfigFile = "protocols.conf";
const QString XI = "xi";
const QString KI = "ki";

double X[2000][2];

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    Qkx_coeffs* kProtocol = new Qkx_coeffs(ConfigFile, KI);
    //передача X
    x_protocol* xProtocol = new x_protocol(ConfigFile, XI, X);

    QSettings settings("settings/settings.ini", QSettings::IniFormat);
    settings.beginGroup("Port");
    QString port = settings.value("portname", "/dev/tty.usbserial-AK06UI59").toString();
    qint32 baudrate = settings.value("baudrate", 38400).toInt();
//    VMA_controller* mSerialPort = new VMA_controller(port, baudrate);


    settings.endGroup();

    VMA_controller* mSerialPort = new VMA_controller(port, baudrate);
    QThread vmaThread;
    mSerialPort->moveToThread(&vmaThread);
    QObject::connect(&vmaThread, &QThread::started, mSerialPort, &VMA_controller::start);
    vmaThread.start();
    X[70][0]=10;
    return a.exec();
}
