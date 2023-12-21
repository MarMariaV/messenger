/*!
	\file
	\brief Файл реализации класса TCP клиента
*/

#include "client.h"
#include <QTcpSocket>
#include <QTime>
#include <QDataStream>
#include <QTimer>


Client::Client(QObject *parent, quint16 fromID)
{
	m_HostIP = "localhost";
	m_port = 1111;
	m_blockSize = 0;
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
	in.setVersion(QDataStream::Qt_5_3);
	for (;;) {
		if (!m_blockSize) {
			if (m_socket->bytesAvailable() < sizeof(quint16)) {
				break;
			}
			in >> m_blockSize;
		}

		if (m_socket->bytesAvailable() < m_blockSize) {
			break;
		}

		QTime time;
		QString str;
		quint16 fromID, code;
		in >> time >> fromID >> code >> str;

		switch (code)
		{
		case eMessage: {
			QString strMessage = str;
			emit setMessage(strMessage, fromID);
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

		m_blockSize = 0;
	}
}

//void MainWindow::slotError(QAbstractSocket::SocketError err)
//{
//
//}

void Client::slotSendToServer(QString str, quint16 toID, quint16 code)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_2);
	out << quint16(0) << QTime::currentTime() << m_fromID << toID << code << str;
	out.device()->seek(0);
	out << quint16(arrBlock.size() - sizeof(quint16));
	m_socket->write(arrBlock);
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
