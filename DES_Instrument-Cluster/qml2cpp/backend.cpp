#include "backend.h"
#include <QDebug>

Backend::Backend(QObject *parent) : QObject(parent)
{
}

QString Backend::processedText() const
{
    return m_processedText;
}

void Backend::processText(const QString &text)
{
    qDebug() << "Received text from QML:" << text;
    m_processedText = text.toUpper(); // Just as an example, convert the text to uppercase.
    emit processedTextChanged();
}
