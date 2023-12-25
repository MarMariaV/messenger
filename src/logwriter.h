#pragma once
#include <QObject>
#include <QFile>

class QTimer;

class LogWriter : public QObject
{
	Q_OBJECT
public:
	LogWriter(QObject* parent);
	~LogWriter();

private:
	QFile newFile;

public slots:
	Q_INVOKABLE void slotWrite(QString str);
};

