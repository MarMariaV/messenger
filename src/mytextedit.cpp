#include "mytextedit.h"
#include <QKeyEvent>


MyTextEdit::MyTextEdit(QWidget* parent)
{
}


MyTextEdit::~MyTextEdit()
{
}

void MyTextEdit::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Return)
	{
		emit textFinished();
		return;
	}

	QTextEdit::keyPressEvent(e);
}