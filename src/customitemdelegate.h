#pragma once
#include <QStyledItemDelegate>

class CustomItemDelegate : public QStyledItemDelegate
{
public:
	CustomItemDelegate(QObject* parent);
	~CustomItemDelegate();

protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

