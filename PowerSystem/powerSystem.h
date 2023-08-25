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


class Power_System: public QObject {
    Q_OBJECT

public:
    Power_System(const QString& portName,
                  const qint32 baud,
                  const QSerialPort::DataBits data = QSerialPort::Data8,
                  const QSerialPort::StopBits stop = QSerialPort::OneStop,
                  const QSerialPort::Parity parity = QSerialPort::NoParity,
                  const QSerialPort::FlowControl flow = QSerialPort::NoFlowControl,
                  QObject* parent = nullptr);
    void settings(QObject *parent);
    virtual ~Power_System();

public slots:
    virtual void start();
    virtual void stop();
//    void setValues(const float Upl,const float Upp,const float Ucl,const float Ucp, const float Uzl, const float Uzp);
signals:
    void started();
    void finished();

private:
    QSerialPort* powerSerialPort = new QSerialPort(this);
    QString pPortName;
    qint32 pBaud;
    QSerialPort::DataBits pDataBits;
    QSerialPort::StopBits pStopBits;
    QSerialPort::Parity pParity;
    QSerialPort::FlowControl pFlow;
    qint16 powerVector[1];
    QTimer pTimer;
    bool isConnected = false;
    mutable QMutex mGuard;
    static constexpr int REQUEST_TIME = 100;
    static constexpr int PACKET_SIZE = 18;
    bool sendData();
    quint16 calculate_crc(QByteArray array);

private slots:
    void tick_power();
};
