#pragma once
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <QObject>
#include <QMutex>
#include <QTimer>
#include <QThread>
#include <QSettings>
#include "math.h"

//для использования kx-pult
extern double X[2000][2];
extern QVector<double> K;


class VMA_controller: public QObject {
    Q_OBJECT

public:
    VMA_controller(const QString& portName,
                  const qint32 baud,
                  const QSerialPort::DataBits data = QSerialPort::Data8,
                  const QSerialPort::StopBits stop = QSerialPort::OneStop,
                  const QSerialPort::Parity parity = QSerialPort::NoParity,
                  const QSerialPort::FlowControl flow = QSerialPort::NoFlowControl,
                  QObject* parent = nullptr);
    void settings(QObject *parent);
    virtual ~VMA_controller();

public slots:
    virtual void start();
    virtual void stop();
    void setValues(const float Upl,const float Upp,const float Ucl,const float Ucp, const float Uzl, const float Uzp);
signals:
    void started();
    void finished();

private:
    QSerialPort* mSerialPort = new QSerialPort(this);
    QString mPortName;
    qint32 mBaud;
    QSerialPort::DataBits mDataBits;
    QSerialPort::StopBits mStopBits;
    QSerialPort::Parity mParity;
    QSerialPort::FlowControl mFlow;
    qint16 vmaVector[7];
    QTimer mTimer;
    bool isConnected = false;
    mutable QMutex mGuard;
    static constexpr int REQUEST_TIME = 100;
    static constexpr int PACKET_SIZE = 18;
    bool sendData();
    quint16 calculate_crc(QByteArray array);
//    VMA_controller* vmaProtocol = nullptr;

//protected:

//    VMA_controller* vmaProtocol = nullptr;
//    QThread vmaThread;

private slots:
    void tick();
};
