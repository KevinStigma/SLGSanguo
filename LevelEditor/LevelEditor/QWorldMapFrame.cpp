#include "QWorldMapFrame.h"
#include <cassert>
#include <iostream>
#include <QFileDialog>

QWorldMapFrame::QWorldMapFrame(QWidget* parent, Qt::WindowFlags f) :QFrame(parent, f), worldMapFrames(NULL), insertImg("insertImg",this),
deleteImg("deleteImg", this), changeImg("changeImg", this), cur_slider(0), cur_sel_label_id(-1), button_state(0)
{
	pop_menu.addAction(&insertImg);
	pop_menu.addAction(&changeImg);
	pop_menu.addAction(&deleteImg);

	connect(&insertImg, SIGNAL(triggered()), this, SLOT(insertImage()));
	connect(&changeImg, SIGNAL(triggered()), this, SLOT(changeImage()));
	connect(&deleteImg, SIGNAL(triggered()), this, SLOT(deleteImage()));
}

QWorldMapFrame::~QWorldMapFrame()
{
	clearLabels();
}

void QWorldMapFrame::contextMenuEvent(QContextMenuEvent *e)
{
	QPoint pt = mapFromGlobal(QCursor::pos());
	cur_sel_label_id = getSeledImgId(pt);
	if (cur_sel_label_id == -1)
	{
		changeImg.setEnabled(false);
		deleteImg.setEnabled(false);
	}
	else
	{
		changeImg.setEnabled(true);
		deleteImg.setEnabled(true);
	}
	pop_menu.exec(QCursor::pos());
	e->accept();
}

void QWorldMapFrame::loadImages(int cur_val)
{
	images.clear();
	auto& img_items = (*worldMapFrames)[cur_val].img_items;
	for (int i = 0; i <(int)img_items.size(); i++)
		images.push_back(QImage(std::string("../Data/Assets/"+img_items[i].img_paths).c_str()));
}

void QWorldMapFrame::updateLabels(int cur_val)
{
	assert((*worldMapFrames)[cur_val].img_items.size()==images.size());
	clearLabels();
	auto& frame = (*worldMapFrames)[cur_val];
	for (int i = 0; i < frame.img_items.size(); i++)
	{
		QLabel* label=new QLabel;
		label->setPixmap(QPixmap::fromImage(images[i]));
		label->setGeometry(frame.img_items[i].pos.x, frame.img_items[i].pos.y, images[i].width(), images[i].height());
		label->setParent(this);
		labels.push_back(label);
	}
	for (int i = 0; i < labels.size(); i++)
		labels[i]->show();		
}

void QWorldMapFrame::clearLabels()
{
	for (int i = 0; i <labels.size() ; i++)
		SAFE_DELETE(labels[i]);
	labels.clear();
}

int QWorldMapFrame::getSeledImgId(QPoint pt)
{
	auto &frame = (*worldMapFrames)[cur_slider];
	for (int i = frame.img_items.size() - 1; i >= 0; i--)
	{
		Position pos = frame.img_items[i].pos;
		if (pt.x() >= pos.x&&pt.x() < pos.x + images[i].width() && pt.y() >= pos.y&&pt.y() < pos.y + images[i].height())
			return i;
	}
	return -1;
}

void QWorldMapFrame::mousePressEvent(QMouseEvent* mouse_event)
{
	if (mouse_event->button() == Qt::LeftButton)
	{
		QPoint pt = mapFromGlobal(QCursor::pos());
		cur_sel_label_id = getSeledImgId(pt);
		button_state = 1;
		//init_pt = pt;
	}
}

void QWorldMapFrame::mouseReleaseEvent(QMouseEvent * mouse_event)
{
	cur_sel_label_id = -1;
	button_state = 0;
}

void QWorldMapFrame::mouseMoveEvent(QMouseEvent * mouse_event)
{
	if (button_state==1&&cur_sel_label_id!=-1)
	{
		int id = cur_sel_label_id;
		QPoint pt = mapFromGlobal(QCursor::pos());
		QLabel* label = labels[id];
		labels[id]->setGeometry(pt.x(),pt.y(),label->width(), label->height());
		(*worldMapFrames)[cur_slider].img_items[id].pos.x = pt.x();
		(*worldMapFrames)[cur_slider].img_items[id].pos.y = pt.y();
		//init_pt = pt;
	}
}

void QWorldMapFrame::updateCurFrame(int val)
{
	cur_slider = val;
	loadImages(val);
	updateLabels(val);
}

void QWorldMapFrame::insertImage()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load Image"), "../Data/Assets/Face", "Image files(*.jpg *.png *.bmp)", 0);
	if (!filename.size())
		return;
	assert(worldMapFrames);
	std::string img_name = filename.toStdString();
	int slash_ind = img_name.find_last_of('/');
	ImageItem img_item;
	img_item.img_paths = "Face/" + img_name.substr(slash_ind + 1, img_name.length() - slash_ind);
	(*worldMapFrames)[cur_slider].img_items.push_back(img_item);
	images.push_back(QImage(filename));
	QLabel* label = new QLabel;
	label->setPixmap(QPixmap::fromImage(images.back()));
	label->setGeometry(0, 0, images.back().width(), images.back().height());
	label->setParent(this);
	labels.push_back(label);
	for (int i = 0; i < labels.size(); i++)
		labels[i]->show();
}

void QWorldMapFrame::deleteImage()
{
	if (cur_sel_label_id == -1)
		return;
	assert(worldMapFrames);
	auto it = (*worldMapFrames)[cur_slider].img_items.begin()+cur_sel_label_id;
	auto&frame = (*worldMapFrames)[cur_slider].img_items.erase(it);

	auto it2 = images.begin() + cur_sel_label_id;
	images.erase(it2);

	auto it3 = labels.begin() + cur_sel_label_id;
	(*it3)->setVisible(false);
	SAFE_DELETE(*it3);
	labels.erase(it3);
}

void QWorldMapFrame::changeImage()
{
	if (cur_sel_label_id == -1)
		return;
	QString filename = QFileDialog::getOpenFileName(this, tr("Load Image"), "../Data/Assets/Face", "Image files(*.jpg *.png *.bmp)", 0);
	if (!filename.size())
		return;
	assert(worldMapFrames);
	std::string img_name = filename.toStdString();
	int slash_ind = img_name.find_last_of('/');
	(*worldMapFrames)[cur_slider].img_items[cur_sel_label_id].img_paths = "Face/" + img_name.substr(slash_ind + 1, img_name.length() - slash_ind);
	images[cur_sel_label_id] = QImage(img_name.c_str());
	labels[cur_sel_label_id]->setPixmap(QPixmap::fromImage(images[cur_sel_label_id]));
}

