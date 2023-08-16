#include "dbusmanager.h"
#include <QDebug>
#include <QTimer>

DBusManager::DBusManager(QObject* parent) : QObject(parent),
    m_bus(QDBusConnection::sessionBus()),
    m_rpm(0)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DBusManager::fetchData);
    timer->start(100);  // increase speed every 0.1 second

    // valueSource Object finding

    m_interface = new QDBusInterface("com.example.dbusService", "/com/example"
                                                                "/dbusService", "com.example.dbusService");

    if(!m_interface->isValid()){
        qDebug() << "Interface not valid: " << qPrintable(m_interface->lastError().message());
        exit(1);
    }
}


void DBusManager::fetchData() {

    QDBusMessage rpm = m_interface->call("get_rpm");
    QDBusMessage distance = m_interface->call("get_distance");
    QDBusMessage battery = m_interface->call("energy_report");

    // check if the call was successful
    if(rpm.type() == QDBusMessage::ErrorMessage || distance.type() == QDBusMessage::ErrorMessage || battery.type() == QDBusMessage::ErrorMessage ) {
        qDebug() << "Error: " << qPrintable(rpm.errorMessage());
        exit(1);
    }

    m_rpm = rpm.arguments().at(0).toInt();
    m_speed = m_rpm * 2.5 * 3.14 / 60; //cm
    m_rpm = m_rpm * 2.5 / 6.8; //car wheel rpm (2.5cm & 6.8cm for each wheel jirum)
    m_distance = distance.arguments().at(0).toInt();
    m_battery = battery.arguments().at(0).toInt();

    emit speedChanged(m_rpm);
    emit distanceChanged(m_distance);
    emit batteryChanged(m_battery);
}

qreal DBusManager::getSpeed() {
    qDebug() << "get speed: " << m_speed;
    return m_speed;
}

qreal DBusManager::getDistance() {
    qDebug() << "get distance: " << m_distance;
    return m_distance;
}

qreal DBusManager::getBattery() {
    qDebug() << "get battery: " << m_battery;
    return m_battery;
}

qreal DBusManager::getRPM() {
    qDebug() << "get rpm: " << m_rpm;
    return m_rpm;
}
