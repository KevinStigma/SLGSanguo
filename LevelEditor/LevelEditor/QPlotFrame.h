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
	void setBackgroundLabel(QLabel*label) { bg_label = label; }
	void setPlotFrames(std::vector<PlotFrame> *pf) { plotFrames = pf; }
	void updateCurFrame(int val);
	void updateDialog(int cur_fra);
protected:
	void mouseDoubleClickEvent(QMouseEvent * e);
	void mousePressEvent(QMouseEvent*);
private:
	std::vector<PlotFrame> *plotFrames;
	int cur_slider;
	QLabel* bg_label;
	QLabel dialog_label;
};

#endif