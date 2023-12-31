/*!
	\file
	\brief ������������ ���� ������ �������� �������� ����������� ���������
*/
#pragma once
#include <QStyledItemDelegate>

class CustomItemDelegate : public QStyledItemDelegate
{
public:	
	/*!
		����������� � ����������
		\param[in] parent ��������� �� ��������
	*/
	explicit CustomItemDelegate(QObject* parent);
	~CustomItemDelegate();

protected:
	//! ��������������� ������ ��������� ��������
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

