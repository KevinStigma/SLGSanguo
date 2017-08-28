#include "QPlotFrame.h"
#include <iostream>
#include <QFileDialog>

QPlotFrame::QPlotFrame(QWidget* parent, Qt::WindowFlags f) :QFrame(parent, f), bg_label(NULL), plotFrames(NULL), cur_slider(0)
{
	dialog_label.setPixmap(QPixmap::fromImage(QImage("../Data/Assets/Mark/47-1.bmp").scaled(345,76)));
	dialog_label.setGeometry(0, 0, 345, 76);
	dialog_label.setParent(this);
	dialog_label.show();
	dialog_label.setVisible(false);
}

QPlotFrame::~QPlotFrame()
{

}

void QPlotFrame::updateCurFrame(int val)
{
	cur_slider = val;
	updateDialog(cur_slider);
}

void QPlotFrame::updateDialog(int cur_frame)
{
	auto& frame = (*plotFrames)[cur_frame];
	if (!frame.dialog_box)
		dialog_label.setVisible(false);
	else
	{
		Position position;
		if (frame.dialog_box->pos == LEFT_UP)
			position = Position(15,15);
		else if (frame.dialog_box->pos == RIGHT_UP)
			position = Position(285,15);
		else if (frame.dialog_box->pos == LEFT_DOWN)
			position = Position(15,309);
		else
			position = Position(285,309);
		dialog_label.setGeometry(position.x,position.y,345,76);
		dialog_label.setVisible(true);
	}
}


void QPlotFrame::mouseDoubleClickEvent(QMouseEvent * e)
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load background"), "../Data/Assets/Maps", "Image files(*.jpg *.png *.bmp)", 0);
	if (!filename.size())
		return;
	bg_label->setPixmap(QPixmap::fromImage(QImage(filename.toStdString().c_str())));
}

void QPlotFrame::mousePressEvent(QMouseEvent* e)
{
}