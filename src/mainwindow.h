/*!
	\file
	\brief ������������ ���� ������ �������� ����
*/
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include <QMap>
#include <QString>

class QStandardItemModel;
class MyTextEdit;
class LogWriter;
class Client;
class QTimer;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/*!
		����������� � ����������
		\param[in] parent ��������� �� ��������
	*/
	explicit MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

private:
	QStandardItemModel* m_currentModel;
	MyTextEdit* m_textEdit;
	QThread* m_netThread;
	QThread* m_logThread;
	Client* m_client;
	LogWriter* m_logWriter;

	Ui::MainWindowClass ui;

	quint16 m_fromID;
	quint16 m_toID;
	QString m_name;
	QTimer*	m_timerIn;

	QMap<int, QString> contactMap;
	QMap<int, QStandardItemModel*> modelMap;

	QButtonGroup* bgContacts;

	//! ������ ������ ��������� �� xml
	void readAddressBook();
	//! �������� ���������� ��������� ��������� � ��������������� ��������� ����������
	void createContacts();

private slots:
	//! ���� ��������� ����� ��������� ��� �������� �� ������
	void textFinished();
	//! ���� ��������� ������ ��� ���������: �������
	void textChanged();
	/*!
		���� ����������� ��������� ���������� � ��������
		\param[in] bState ��������� ����������
	*/
	void slotIsConnected(bool bState);	
	/*!
		���� ����������� ��������� ���������� � ���������
		\param[in] bState ��������� ����������
		\param[in] toID ID ��������
	*/
	void slotContactIsConnected(bool bState, quint16 id);
	/*!
		���� ����������� ���������, ��� ������������ ��������
		\param[in] toID ID ��������
	*/
	void slotContactIsTyping(quint16 id);
	//! ���� Timeout ������� ��� ����������� ���������, ��� ������� ��������
	void slotTimeoutIn();

	/*!
		���� ���������� ��������� ��� �������� �� ������
		\param[in] str ������ ���������
		\param[in] toID ID ��������
	*/
	void slotSetMessage(QString str, quint16 fromID);	
	/*!
		���� ������ ��������
		\param[in] toID ID ��������
	*/
	void setChoseContact(quint16);

	void sendFile();
};
