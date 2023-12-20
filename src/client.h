#pragma once
#include <QObject>
#include <QMap>

class QTcpSocket;
class QTimer;

class Client: public QObject
{
	Q_OBJECT

public:
	Client(QObject *parent, quint16 fromID);
	~Client();

	enum eCodes {
		eMessage,
		eConnected,
		eDisconnected,
		eTyping,
	};



private:
	QTcpSocket*	m_socket;
	quint16		m_blockSize;
	QString		m_HostIP;
	int			m_port;
	QTimer*		m_timer;
	quint16		m_fromID;

public slots:
	Q_INVOKABLE void slotSendToServer(QString str, quint16 toID, quint16 code);
	Q_INVOKABLE void start();

private slots:
	void slotReadyRead();
	void slotConnected();
	void slotDisconnected();
	void slotTimeout();
//	void slotError(QAbstractSocket::SocketError);

private:


signals:
	void isConnected(bool);
	void setMessage(QString, quint16);
	void contactIsConnected(bool, quint16);
};

