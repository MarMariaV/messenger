/*!
	\file
	\brief Заголовочный файл класса Кастомного TextEdit
*/
#pragma once
#include <QTextEdit>

class MyTextEdit: public QTextEdit
{
	Q_OBJECT

public:
	/*!
		Конструктор с параметром
		\param[in] parent Указатель на родителя
	*/
	explicit MyTextEdit(QWidget* parent = nullptr);
	~MyTextEdit();
	//! Переопределения метода обработки нажатия клавиш
	void keyPressEvent(QKeyEvent* e) override;

signals:
	void textFinished();
};

