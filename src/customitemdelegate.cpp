/*!
	\file
	\brief ���� ���������� ������ �������� �������� ����������� ���������
*/

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
	QStyledItemDelegate::paint(painter, option, index);
}
