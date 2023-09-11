#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString processedText READ processedText NOTIFY processedTextChanged)

public:
    explicit Backend(QObject *parent = nullptr);

    QString processedText() const;

public slots:
    void processText(const QString &text);

signals:
    void processedTextChanged();

private:
    QString m_processedText;
};

#endif // BACKEND_H
