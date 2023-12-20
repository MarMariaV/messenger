#include "mainwindow.h"
#include "mytextedit.h"
#include "customitemdelegate.h"
#include "client.h"

#include <QStandardItemModel>
#include <QListView>
#include <QStyledItemDelegate>
#include <QTime>
#include <QThread>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QButtonGroup>
#include <QSpacerItem>
#include <QMetaObject>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), m_toID(0), m_fromID(0)
{
	ui.setupUi(this);

	bgContacts = new QButtonGroup(this);

	readAddressBook();
	createContacts();

	m_netThread = new QThread(this);
	m_client = new Client(this, m_fromID);
	m_client->moveToThread(m_netThread);
	QMetaObject::invokeMethod(m_client, "start");
	m_netThread->start();
	connect(m_client, &Client::isConnected, this, &MainWindow::slotIsConnected);
	connect(m_client, &Client::setMessage, this, &MainWindow::slotSetMessage);
	connect(m_client, &Client::contactIsConnected, this, &MainWindow::slotContactIsConnected);

	m_textEdit = new MyTextEdit(this);
	ui.layoutWrite->addWidget(m_textEdit, 0, 0);
	connect(m_textEdit, &MyTextEdit::textFinished, this, &MainWindow::textFinished);

	ui.listView->setItemDelegate(new CustomItemDelegate(ui.listView));
	ui.layoutRead->addWidget(ui.listView);
}

MainWindow::~MainWindow()
{
	// ��������, ��� ����� ������ (m_client �� ���������)
	m_netThread->quit();
	m_netThread->wait();
}

void MainWindow::createContacts()
{
	for (const auto& contact : contactMap) {
		QPushButton* pb = new QPushButton(contact, this);
		pb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
		pb->setCheckable(true);
		bgContacts->addButton(pb, contactMap.key(contact));
		ui.scrollAreaWidgetContents->layout()->addWidget(pb);
		modelMap[contactMap.key(contact)] = new QStandardItemModel(this);
	}
	ui.scrollAreaWidgetContents->layout()->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
	connect(bgContacts, &QButtonGroup::idClicked, this, &MainWindow::setChoseContact);

	bgContacts->buttons().at(0)->setChecked(true);
	setChoseContact(bgContacts->checkedId());

	ui.labelName->setText(m_name);
}

void MainWindow::setChoseContact(int id)
{
	m_toID = id;
	m_currentModel = modelMap[m_toID];
	ui.listView->setModel(m_currentModel);
}

void MainWindow::readAddressBook()
{
	QFile file("contact.xml");

	if (file.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		QXmlStreamReader xml(&file);
		while (!xml.atEnd() && !xml.hasError())
		{
			xml.readNext();
			QXmlStreamReader::TokenType token = xml.readNext();
			if (token == QXmlStreamReader::StartDocument)
				continue;
			if (token == QXmlStreamReader::StartElement)
			{
				if (xml.name() == "client") {
					QXmlStreamAttributes attr = xml.attributes();
					if (attr.hasAttribute("id")) {
						m_fromID = attr.value("id").toInt();
					}
					if (m_fromID > 0) {
						m_name = xml.readElementText();
					}
				}
				if (xml.name() == "contact") {
					quint16 newId = 0;
					QXmlStreamAttributes attr = xml.attributes();
					if (attr.hasAttribute("id")) {
						newId = attr.value("id").toInt();	
					}
					if (newId > 0) { 
						contactMap[newId] = xml.readElementText();
					}
				}
			}
		}
	}

	file.close();
}

void MainWindow::textFinished()
{
	m_currentModel->insertRow(m_currentModel->rowCount(), new QStandardItem());
	QModelIndex index = m_currentModel->index(m_currentModel->rowCount() - 1, 0);
	m_currentModel->setData(index, m_textEdit->toPlainText(), Qt::EditRole);
	m_currentModel->setData(index, QColor(166, 221, 202), Qt::BackgroundRole);
	m_currentModel->setData(index, Qt::AlignLeft, Qt::TextAlignmentRole);
	QMetaObject::invokeMethod(m_client, "slotSendToServer", Qt::AutoConnection,
		Q_ARG(QString, m_textEdit->toPlainText()),
		Q_ARG(quint16, m_toID),
		Q_ARG(quint16, 0));
	m_textEdit->clear();
}

void MainWindow::slotContactIsConnected(bool bState, quint16 id)
{
	if (bgContacts->button(id)) {
		if (bState)
			bgContacts->button(id)->setStyleSheet("QPushButton {background-color : green }");
		else
			bgContacts->button(id)->setStyleSheet("QPushButton {background-color : grey }");
	}
}

void MainWindow::slotIsConnected(bool bState)
{
	if (bState)
		ui.pbServer->setStyleSheet("QPushButton {background-color : green }");
	else
		ui.pbServer->setStyleSheet("QPushButton {background-color : white }");
}

void MainWindow::slotSetMessage(QString str, int fromID)
{
	if (modelMap[fromID] != nullptr) {
		modelMap[fromID]->insertRow(modelMap[fromID]->rowCount(), new QStandardItem());
		QModelIndex index = modelMap[fromID]->index(modelMap[fromID]->rowCount() - 1, 0);
		modelMap[fromID]->setData(index, str, Qt::EditRole);
		m_currentModel->setData(index, QColor(199, 183, 217), Qt::BackgroundRole);
		m_currentModel->setData(index, Qt::AlignRight, Qt::TextAlignmentRole);
	}
}