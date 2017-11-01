#ifndef GENERALS_LISTWIDGET_H
#define GENERALS_LISTWIDGET_H
#include <QListWidget>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include "DataStructure.h"

class GeneralsListWidget:public QListWidget
{
	Q_OBJECT
public:
	GeneralsListWidget(QWidget* parent=0);
	void setGenrals(std::vector<General>*g){ generals = g; }
	void setLastRow(int* l){ last_sel_row = l; }
	void updateListViewFromGenerals();
protected:
	void contextMenuEvent(QContextMenuEvent *e);
public slots:
	void insertGeneral();
	void deleteGeneral();
private:
	QMenu pop_menu;
	QAction insertGen;
	QAction deleteGen;
	std::vector<General>* generals;
	int* last_sel_row;
};

#endif