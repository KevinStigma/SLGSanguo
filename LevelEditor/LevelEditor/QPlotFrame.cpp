#include "QPlotFrame.h"
#include <iostream>
#include <QFileDialog>

QPlotFrame::QPlotFrame(QWidget* parent, Qt::WindowFlags f) :QFrame(parent, f), bg_label(NULL)
{

}

QPlotFrame::~QPlotFrame()
{

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