/*!
	\file
	\brief ������������ ���� ������ ���������� TextEdit
*/
#pragma once
#include <QTextEdit>

class MyTextEdit: public QTextEdit
{
	Q_OBJECT

public:
	/*!
		����������� � ����������
		\param[in] parent ��������� �� ��������
	*/
	explicit MyTextEdit(QWidget* parent = nullptr);
	~MyTextEdit();
	//! ��������������� ������ ��������� ������� ������
	void keyPressEvent(QKeyEvent* e) override;

signals:
	void textFinished();
};

