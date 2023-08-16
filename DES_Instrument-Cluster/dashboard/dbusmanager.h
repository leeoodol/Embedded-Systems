#ifndef DBUSMANAGER_H
#define DBUSMANAGER_H

#include <QObject>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>

class DBusManager : public QObject
{
    Q_OBJECT
public:
    explicit DBusManager(QObject* parent = nullptr);

    Q_INVOKABLE void fetchData(); // fetch Rpm from Dbus
    Q_INVOKABLE qreal getSpeed();
    Q_INVOKABLE qreal getDistance();
    Q_INVOKABLE qreal getBattery();
    Q_INVOKABLE qreal getRPM();

signals:
    void speedChanged(qreal speed);
    void distanceChanged(qreal distance);
    void batteryChanged(qreal battery);

private:
    QDBusConnection m_bus;
    QDBusInterface* m_interface;
    qreal m_rpm;
    qreal m_battery;
    qreal m_distance;
    qreal m_speed;
};
#endif // DBUSMANAGER_H
