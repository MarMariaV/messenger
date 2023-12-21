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
	MainWindow(QWidget *parent = Q_NULLPTR);
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
	QTimer*	m_timerOut;

	QMap<int, QString> contactMap;
	QMap<int, QStandardItemModel*> modelMap;

	QButtonGroup* bgContacts;

	void readAddressBook();
	void createContacts();

private slots:
	void textFinished();
	void textChanged();
	void slotIsConnected(bool bState);
	void slotContactIsConnected(bool bState, quint16 id);
	void slotContactIsTyping(quint16 id);
	void slotTimeoutIn();
	void slotTimeoutOut();
	void slotSetMessage(QString str, quint16 fromID);
	void setChoseContact(int);
};
