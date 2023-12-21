/*!
	\file
	\brief Заголовочный файл класса Делегата элемента отображения сообщения
*/
#pragma once
#include <QStyledItemDelegate>

class CustomItemDelegate : public QStyledItemDelegate
{
public:	
	/*!
		Конструктор с параметром
		\param[in] parent Указатель на родителя
	*/
	explicit CustomItemDelegate(QObject* parent);
	~CustomItemDelegate();

protected:
	//! Переопределения метода отрисовки делегата
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

