#include "logwriter.h"
#include <QTimer>
#include <QString>
#include <QDateTime>
#include <QByteArray>

LogWriter::LogWriter(QObject* parent)
{
	QDate date = QDate::currentDate();
	QTime time = QTime::currentTime();
	QString str = date.toString("yyyy-MM-dd") + "_" + time.toString("hh-mm-ss") + ".txt";
	newFile.setFileName(str);

	newFile.open(QIODevice::WriteOnly | QFile::Append);
}

LogWriter::~LogWriter()
{
	newFile.close();
}

void LogWriter::slotWrite(const QString str)
{
	qint64 toFile = newFile.write(str.toUtf8() + "\n");
}
