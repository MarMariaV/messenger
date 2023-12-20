#include "customitemdelegate.h"
#include <QPainter>


CustomItemDelegate::CustomItemDelegate(QObject* parent)
{
}


CustomItemDelegate::~CustomItemDelegate()
{
}

void CustomItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	//QFont var = QFont(index.data(Qt::FontRole));
	//if (var == "test")
	//	painter->fillRect(option.rect, Qt::yellow);
	//else 
	//	painter->fillRect(option.rect, Qt::green);
	QStyledItemDelegate::paint(painter, option, index);
}
