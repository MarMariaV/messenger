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
	quint16		m_port;
	QTimer*		m_timer;
	quint16		m_fromID;
	bool		m_isTyping;
	quint16		m_typingId;

public slots:
	Q_INVOKABLE void slotSendToServer(QString str, quint16 toID, quint16 code);
	Q_INVOKABLE void slotIsTyping(quint16 toID, bool isTexting);
	Q_INVOKABLE void start();

private slots:
	void slotReadyRead();
	void slotConnected();
	void slotDisconnected();
	void slotTimeout();
//	void slotError(QAbstractSocket::SocketError);

signals:
	void isConnected(bool);
	void setMessage(QString, quint16);
	void contactIsConnected(bool, quint16);
	void contactIsTyping(quint16);
};

