/*!
	\file
	\brief Файл реализации класса TCP клиента
*/

#include "client.h"
#include <QTcpSocket>
#include <QTime>
#include <QDataStream>
#include <QTimer>
#include <QFile>
#include <QFileInfo>


Client::Client(QObject *parent, quint16 fromID)
{
	m_HostIP = "localhost";
	m_port = 1111;
	m_fromID = fromID;
	m_isTyping = false;
}


void Client::start()
{
	m_socket = new QTcpSocket(this);
	m_timer = new QTimer(this);

	connect(m_socket, &QTcpSocket::connected, this, &Client::slotConnected);
	connect(m_socket, &QTcpSocket::disconnected, this, &Client::slotDisconnected);
	connect(m_socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
	//connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));

	connect(m_timer, &QTimer::timeout, this, &Client::slotTimeout);
	m_timer->start(1000);
}


Client::~Client()
{
	m_socket->disconnectFromHost();
}

void Client::slotTimeout()
{
	if (m_socket->state() == QTcpSocket::UnconnectedState) {
		m_socket->connectToHost(m_HostIP, m_port);
	}
	else if (m_socket->state() == QTcpSocket::ConnectedState) {
		if (m_isTyping) {
			slotSendToServer("", m_typingId, eTyping);
		}
	}
}

void Client::slotIsTyping(quint16 toID, bool isTexting)
{
	m_isTyping = isTexting;
	m_typingId = toID;
}

void Client::slotReadyRead()
{
	QDataStream in(m_socket);
	in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
	while (!in.atEnd())
	{

		QTime time;
		quint16 fromID, code;
		in >> time >> fromID >> code;

		switch (code)
		{
		case eMessage: {
			QString strMessage;
			in >> strMessage;
			emit setMessage(strMessage, fromID);
			break;
		}
		case eFile: {

			break;
		}
		case eConnected:
			emit contactIsConnected(true, fromID);
			break;
		case eDisconnected:
			emit contactIsConnected(false, fromID);
			break;
		case eTyping:
			emit contactIsTyping(fromID);
			break;
		default:
			break;
		}
	}
}

//void MainWindow::slotError(QAbstractSocket::SocketError err)
//{
//
//}

void Client::slotSendToServer(QString str, quint16 toID, quint16 code)
{
	QDataStream stream(m_socket);
	stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);
	stream << QTime::currentTime() << m_fromID << toID << code << str;
	m_socket->waitForBytesWritten();
}

void Client::socketSendFile(quint16 toID, QString path)
{
	QDataStream stream(m_socket);
	stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);

	QFile file(path);
	auto strList = path.split("/");
	m_fileName = strList.last();

	m_sendFile = new QFile(path);
	if (m_sendFile->open(QFile::ReadOnly)) {
		connect(m_socket, &QTcpSocket::bytesWritten, this, &Client::sendPartOfFile);
		sendPartOfFile();
	}
}

void Client::sendPartOfFile()
{
	QDataStream stream(m_socket);
	stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);

	if (!m_sendFile->atEnd()) {
		QByteArray data = m_sendFile->read(1024 * 250);
		stream << QTime::currentTime() << m_fromID << m_fromID << quint16(eFile) << m_fileName << quint16(data.size()) << data;
		m_socket->waitForBytesWritten();
	}
	else {
		m_sendFile->close();
		disconnect(m_socket, &QTcpSocket::bytesWritten, this, &Client::sendPartOfFile);
	}
}


void Client::slotConnected()
{
	slotSendToServer("", 0, eMessage);
	emit isConnected(true);
}

void Client::slotDisconnected()
{
	emit isConnected(false);
}
