/*!
	\file
	\brief Заголовочный файл класса TCP клиента
*/

#pragma once
#include <QObject>
#include <QMap>

/*!
	\brief Класс TCP клиента

	Client предоставляет возможность принять сообщения от сервера,
	отправить сообщения на сервер
*/

class QTcpSocket;
class QTimer;
class QFile;

class Client: public QObject
{
	Q_OBJECT

public:
	/*!
		Конструктор с параметром
		\param[in] parent Указатель на родителя
		\param[in] fromID ID адресанта
	*/
	Client(QObject *parent, quint16 fromID);
	~Client();

	enum eCodes {
		eNo,
		eMessage,
		eFile,
		eConnected,
		eDisconnected,
		eTyping,
	};

private:
	QTcpSocket*	m_socket;
	QString		m_HostIP;
	quint16		m_port;
	QTimer*		m_timer;
	quint16		m_fromID;
	bool		m_isTyping;
	quint16		m_typingId;
	QString		m_fileName;
	QFile*		m_sendFile;

public slots:
	/*!
		Слот отправки сообщения на сервер
		\param[in] str Строка сообщения
		\param[in] toID ID адресата
		\param[in] code Тип отправляемого сообщения
	*/
	Q_INVOKABLE void slotSendToServer(QString str, quint16 toID, quint16 code);
	/*!
		Слот сообщения о том, что пользователь печатает
		\param[in] toID ID адресата
		\param[in] isTexting флаг того, что пользователь печатает
	*/
	Q_INVOKABLE void slotIsTyping(quint16 toID, bool isTexting);
	//! Слот инициализации Client
	Q_INVOKABLE void start();
	Q_INVOKABLE void socketSendFile(quint16 toID, QString path);

private slots:
	//! Слот готовности чтения сообщения от сервера
	void slotReadyRead();
	//! Слот соединения с сервером
	void slotConnected();
	//! Слот рассоединения с сервером
	void slotDisconnected();
	//! Слот Timeout таймера для повторного подключения к серверу
	void slotTimeout();
//	void slotError(QAbstractSocket::SocketError);
	void sendPartOfFile();

signals:
	void isConnected(bool);
	void setMessage(QString, quint16);
	void contactIsConnected(bool, quint16);
	void contactIsTyping(quint16);
};

