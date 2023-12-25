/*!
	\file
	\brief Заголовочный файл класса Главного окна
*/
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include <QMap>
#include <QString>

class QStandardItemModel;
class MyTextEdit;
class Client;
class QTimer;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/*!
		Конструктор с параметром
		\param[in] parent Указатель на родителя
	*/
	explicit MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

private:
	QStandardItemModel* m_currentModel;
	MyTextEdit* m_textEdit;
	QThread* m_netThread;
	Client* m_client;

	Ui::MainWindowClass ui;

	quint16 m_fromID;
	quint16 m_toID;
	QString m_name;
	QTimer*	m_timerIn;

	QMap<int, QString> contactMap;
	QMap<int, QStandardItemModel*> modelMap;

	QButtonGroup* bgContacts;

	//! Чтение списка контактов из xml
	void readAddressBook();
	//! Создание локального хранилища контактов и соответствующих элементов интерфейса
	void createContacts();

private slots:
	//! Слот окончания ввода сообщения для отправки на сервер
	void textFinished();
	//! Слот изменения текста для сообщения: печатаю
	void textChanged();
	/*!
		Слот отображения состояния соединения с сервером
		\param[in] bState состояние соединения
	*/
	void slotIsConnected(bool bState);	
	/*!
		Слот отображения состояния соединения с контактом
		\param[in] bState состояние соединения
		\param[in] toID ID контакта
	*/
	void slotContactIsConnected(bool bState, quint16 id);
	/*!
		Слот отображения сообщения, что пользователь печатает
		\param[in] toID ID контакта
	*/
	void slotContactIsTyping(quint16 id);
	//! Слот Timeout таймера для отображения сообщения, что контакт печатает
	void slotTimeoutIn();

	/*!
		Слот подготовки сообщения для отправки на сервер
		\param[in] str Строка сообщения
		\param[in] toID ID адресата
	*/
	void slotSetMessage(QString str, quint16 fromID);	
	/*!
		Слот выбора контакта
		\param[in] toID ID адресата
	*/
	void setChoseContact(quint16);

	void sendFile();
};
