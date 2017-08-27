#ifndef QPLOT_FRAME
#define QPLOT_FRAME
#include<QFrame>
#include<QMenu>
#include<QAction>
#include<QImage>
#include<QLabel>
#include<QContextMenuEvent>
#include "DataStructure.h"
class QPlotFrame :public QFrame
{
	Q_OBJECT
public:
	QPlotFrame(QWidget* parent = 0, Qt::WindowFlags f = 0);
	~QPlotFrame();
	void setBackgroundLabel(QLabel*label){ bg_label = label; }
protected:
	void mouseDoubleClickEvent(QMouseEvent * e);
	void mousePressEvent(QMouseEvent*);
private:
	QLabel* bg_label;
};

#endif