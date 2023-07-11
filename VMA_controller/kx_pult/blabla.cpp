#include "AH127Cprotocol.h"
#include <stdint.h>
#include <QDataStream>

AH127Cprotocol::AH127Cprotocol(QString portName, int baudRate, QObject *parent)
{
    m_port.setBaudRate(baudRate);
    m_port.setPortName(portName);
    m_port.open(QIODevice::ReadWrite);

    char cmd_1[6]; //задание частоты выдачи данных, 2.17
    cmd_1[0] = 0x77;
    cmd_1[1] = 0x05;
    cmd_1[2] = 0x00;
    cmd_1[3] = 0x56;
    cmd_1[4] = 0x05;
    cmd_1[5] = 0x60;
    m_port.write(cmd_1, 6);
    m_port.waitForBytesWritten();

    char cmd_2[6]; //задание формата посылки, 2.16
    cmd_2[0] = 0x77;
    cmd_2[1] = 0x05;
    cmd_2[2] = 0x00;
    cmd_2[3] = 0x0C;
    cmd_2[4] = 0x05;
    cmd_2[5] = 0x16;
    m_port.write(cmd_2, 6);
    m_port.waitForBytesWritten();

    QTimer *timer = new QTimer(this);
    connect(&m_port, &QSerialPort::readyRead, this, &AH127Cprotocol::readData);
    connect(&m_port, &QSerialPort::readyRead, this, &AH127Cprotocol::readyReadForTimer);
    connect(timer, &QTimer::timeout, this, &AH127Cprotocol::timeoutSlot);
    timer->start(3000);
}

//unsigned short AH127Cprotocol::calculateCRC(unsigned char data[], unsigned int length) {
//     unsigned int i;
//     unsigned short crc = 0;
//     for(i=0; i<length; i++){
//         crc += data[i];
//     }
//    return crc;
//}

//bool AH127Cprotocol::correctChecksum (QByteArray const &ba) {
//    if (calculateCRC((uchar*)ba.data(), 55) == ba.data()[56]) {
//        return true;
//    }
//    return false;
//}

void AH127Cprotocol::readData() {
    m_buffer.append(m_port.readAll());
    parseBuffer();
}

void AH127Cprotocol::readyReadForTimer() {
    time.restart();

}

void AH127Cprotocol::timeoutSlot(){
 double deltaTMax = 3000;
    if (time.elapsed()>deltaTMax) {
        char cmd_1[6]; //задание частоты выдачи данных, 2.17
        cmd_1[0] = 0x77;
        cmd_1[1] = 0x05;
        cmd_1[2] = 0x00;
        cmd_1[3] = 0x56;
        cmd_1[4] = 0x05;
        cmd_1[5] = 0x60;
        m_port.write(cmd_1, 6);
        m_port.waitForBytesWritten();

        char cmd_2[6]; //задание формата посылки, 2.15
        cmd_2[0] = 0x77;
        cmd_2[1] = 0x05;
        cmd_2[2] = 0x00;
        cmd_2[3] = 0x0C;
        cmd_2[4] = 0x05;
        cmd_2[5] = 0x16;
        m_port.write(cmd_2, 6);
        m_port.waitForBytesWritten();
    }
}

void AH127Cprotocol::parseBuffer() {
    if (m_buffer.size() <= 4 ) {
        return;
    }
    //QByteArray header((char*) &(data.header), sizeof(uint8_t));
    QByteArray header((char*) &(data.header),sizeof(Header));
    qDebug() << "хедер" << header;
    int index = m_buffer.indexOf(header);
    if (index == -1) {
        // Не найдено сообщение
        qDebug() << "нет сообщения в буфере ";
        return;
    }
    if ( m_buffer.size() <= index + 55 ) {
        return;
    }
    if (1) {
        DataFromAH127C msg;
        auto tmp = m_buffer.mid(index, 57);
        QDataStream stream(&tmp, QIODevice::ReadOnly);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        stream >> msg.header.identif;
        stream >> msg.header.lenght;
        stream >> msg.header.adress;
        stream >> msg.header.command;
        stream >> msg.yaw.yaw1;
        stream >> msg.yaw.yaw2;
        stream >> msg.yaw.yaw3;
        float yaw =
        qDebug() << "yaw: " <<msg.Yaw;
        stream >>msg.pitch;
        qDebug() << "pitch: " <<msg.pitch;
        stream >> msg.roll;
        qDebug() << "roll: " <<msg.roll;
        stream >> msg.X_accel;
        qDebug() << "X_accel: " <<msg.X_accel;
        stream >>msg.Y_accel;
        qDebug() << "Y_accel: " <<msg.Y_accel;
        stream >> msg.Z_accel;
        qDebug() << "Z_accel: " <<msg.Z_accel;
        stream >> msg.X_rate;
        qDebug() << "X_rate: " <<msg.X_rate;
        stream >>msg.Y_rate;
        qDebug() << "Y_rate: " <<msg.Y_rate;
        stream >> msg.Z_rate;
        qDebug() << "Z_rate: " <<msg.Z_rate;
        stream >> msg.X_magn;
        qDebug() << "X_magn: " <<msg.X_magn;
        stream >>msg.Y_magn;
        qDebug() << "Y_magn: " <<msg.Y_magn;
        stream >> msg.Z_magn;
        qDebug() << "Z_magn: " <<msg.Z_magn;
        stream >> msg.first_qvat;
        qDebug() << "first_qvat: " <<msg.first_qvat;
        stream >>msg.second_qvat;
        qDebug() << "second_qvat: " <<msg.second_qvat;
        stream >> msg.third_qvat;
        qDebug() << "third_qvat: " <<msg.third_qvat;
        stream >> msg.four_qvat;
        qDebug() << "four_qvat: " <<msg.four_qvat;

        emit newMessageDetected(msg);
        m_buffer.remove(0, index+57);
    }
    else {
        m_buffer.remove(0, index+1);
    }

    return;
}
