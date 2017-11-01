#include "GeneralsListWidget.h"
GeneralsListWidget::GeneralsListWidget(QWidget* parent) :QListWidget(parent), insertGen("insert", this), deleteGen("delete", this), generals(NULL), last_sel_row(NULL)
{
	pop_menu.addAction(&insertGen);
	pop_menu.addAction(&deleteGen);
	connect(&insertGen, SIGNAL(triggered()), this, SLOT(insertGeneral()));
	connect(&deleteGen, SIGNAL(triggered()), this, SLOT(deleteGeneral()));
}

void GeneralsListWidget::insertGeneral()
{
	General general;
	general.name = "anonym";
	generals->push_back(general);
	addItem(QString("anonym"));
}

void GeneralsListWidget::deleteGeneral()
{
	int ind = currentRow();
	if (ind < 0)
		return;
	generals->erase((*generals).begin() + ind);
	*(last_sel_row) = -1;
	updateListViewFromGenerals();
}

void GeneralsListWidget::updateListViewFromGenerals()
{
	clear();
	for (int i = 0; i < generals->size(); i++)
		addItem(QString((*generals)[i].name.c_str()));
	setCurrentRow(-1);
}

void GeneralsListWidget::contextMenuEvent(QContextMenuEvent *e)
{
	QPoint pt = mapFromGlobal(QCursor::pos());
	insertGen.setEnabled(true);
	int ind = currentRow();
	if (ind < 0)
		deleteGen.setEnabled(false);
	else
		deleteGen.setEnabled(true);
	pop_menu.exec(QCursor::pos());
	e->accept();
}