#include "QPlotFrame.h"
#include <iostream>
#include <cassert>
#include <QFileDialog>
#include "GlobalSys.h"

QPlotFrame::QPlotFrame(QWidget* parent, Qt::WindowFlags f) :QFrame(parent, f), bg_label(NULL), plotFrames(NULL), plot_item_label(NULL),
plot_item_comboBox(NULL),plot_item_checkBox(NULL),cur_slider(0),insertImg("insertImg", this), deleteImg("deleteImg", this), 
cur_sel_label_id(-1)
{
	dialog_label.setPixmap(QPixmap::fromImage(QImage(std::string(g_pGlobalSys->assetPath+"Mark/47-1.png").c_str()).scaled(345,76)));
	dialog_label.setGeometry(0, 0, 345, 76);
	dialog_label.setParent(this);
	dialog_label.show();
	dialog_label.setVisible(false);
	dialog_label.raise();

	pop_menu.addAction(&insertImg);
	pop_menu.addAction(&deleteImg);

	connect(&insertImg, SIGNAL(triggered()), this, SLOT(insertImage()));
	connect(&deleteImg, SIGNAL(triggered()), this, SLOT(deleteImage()));
}

QPlotFrame::~QPlotFrame()
{
	clearLabels();
}

void QPlotFrame::insertImage()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load Image"),std::string(g_pGlobalSys->assetPath+"pmapobj").c_str(), "Image files(*.jpg *.png *.bmp)", 0);
	if (!filename.size())
		return;
	assert(plotFrames);
	std::string img_name = filename.toStdString();
	int slash_ind = img_name.find_last_of('/');
	ImageItem img_item;
	img_item.img_paths = "pmapobj/" + img_name.substr(slash_ind + 1, img_name.length() - slash_ind);
	img_item.type = 0;
	img_item.mirrored = false;
	(*plotFrames)[cur_slider].img_items.push_back(img_item);
	images.push_back(QImage(filename).copy(0, 0, 48, 64));
	QLabel* label = new QLabel;
	label->setPixmap(QPixmap::fromImage(images.back()));
	label->setGeometry(0, 0, images.back().width(), images.back().height());
	label->setParent(this);
	labels.push_back(label);
	for (int i = 0; i < labels.size(); i++)
		labels[i]->show();
	initPlotItemGroup();
}

void QPlotFrame::deleteImage()
{
	if (cur_sel_label_id == -1)
		return;
	assert(plotFrames);
	auto it = (*plotFrames)[cur_slider].img_items.begin() + cur_sel_label_id;
	auto& frame = (*plotFrames)[cur_slider].img_items.erase(it);

	auto it2 = images.begin() + cur_sel_label_id;
	images.erase(it2);

	auto it3 = labels.begin() + cur_sel_label_id;
	(*it3)->setVisible(false);
	SAFE_DELETE(*it3);
	labels.erase(it3);
	initPlotItemGroup();
}

void QPlotFrame::updateCurFrame(int val)
{
	cur_slider = val;
	cur_sel_label_id = -1;
	updateDialog(cur_slider);
	loadImages(cur_slider);
	updateLabels(cur_slider);
	initPlotItemGroup();
}

void QPlotFrame::clearLabels()
{
	for (int i = 0; i <labels.size(); i++)
		SAFE_DELETE(labels[i]);
	labels.clear();
}

void QPlotFrame::updateLabels(int cur_fra)
{
	assert((*plotFrames)[cur_fra].img_items.size() == images.size());
	clearLabels();
	auto& frame = (*plotFrames)[cur_fra];
	for (int i = 0; i < frame.img_items.size(); i++)
	{
		QLabel* label = new QLabel;
		label->setPixmap(QPixmap::fromImage(images[i]));
		label->setGeometry(frame.img_items[i].pos.x, frame.img_items[i].pos.y, images[i].width(), images[i].height());
		label->setParent(this);
		labels.push_back(label);
	}
	for (int i = 0; i < labels.size(); i++)
		labels[i]->show();
}

void QPlotFrame::loadImages(int cur_fra)
{
	images.clear();
	auto& img_items = (*plotFrames)[cur_fra].img_items;
	for (int i = 0; i < (int)img_items.size(); i++)
	{
		int type = (*plotFrames)[cur_fra].img_items[i].type;
		bool mirrored = (*plotFrames)[cur_fra].img_items[i].mirrored;
		QImage image=QImage(std::string(g_pGlobalSys->assetPath + img_items[i].img_paths).c_str()).copy(0,type*64,48,64);
		if (mirrored)
			image = image.mirrored(true, false);
		images.push_back(image);
	}
}

void QPlotFrame::setBackgroundPath(std::string str)
{ 
	bg_local_path = str;
	bg_label->setPixmap(QPixmap::fromImage(QImage(std::string(g_pGlobalSys->assetPath + str).c_str()))); 
	bg_label->lower();
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
		dialog_label.show();
	}
}

void QPlotFrame::initPlotItemGroup()
{
	cur_sel_label_id = -1;
	plot_item_label->setPixmap(QPixmap::fromImage(QImage()));
	plot_item_comboBox->setCurrentIndex(0);
	plot_item_checkBox->setChecked(false);
}

int QPlotFrame::getSeledImgId(QPoint pt)
{
	auto &frame = (*plotFrames)[cur_slider];
	for (int i = frame.img_items.size() - 1; i >= 0; i--)
	{
		Position pos = frame.img_items[i].pos;
		if (pt.x() >= pos.x&&pt.x() < pos.x + images[i].width() && pt.y() >= pos.y&&pt.y() < pos.y + images[i].height())
			return i;
	}
	return -1;
}

void QPlotFrame::contextMenuEvent(QContextMenuEvent *e)
{ 
	if (!bg_label->pixmap())
		return;
	QPoint pt = mapFromGlobal(QCursor::pos());
	cur_sel_label_id = getSeledImgId(pt);
	if (cur_sel_label_id == -1)
		deleteImg.setEnabled(false);
	else
		deleteImg.setEnabled(true);
	pop_menu.exec(QCursor::pos());
	e->accept();
}

void QPlotFrame::mouseDoubleClickEvent(QMouseEvent * e)
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load background"),std::string(g_pGlobalSys->assetPath+"Maps").c_str(), "Image files(*.jpg *.png *.bmp)", 0);
	if (!filename.size())
		return;

	std::string img_name = filename.toStdString();
	int slash_ind = img_name.find_last_of('/');
	bg_local_path = "Maps/" + img_name.substr(slash_ind + 1, img_name.length() - slash_ind);
	bg_label->setPixmap(QPixmap::fromImage(QImage(filename.toStdString().c_str())));
	bg_label->lower();
}

void QPlotFrame::mousePressEvent(QMouseEvent* mouse_event)
{
	if (!bg_label->pixmap())
		return;
	if (mouse_event->button() == Qt::LeftButton)
	{
		QPoint pt = mapFromGlobal(QCursor::pos());
		int id = getSeledImgId(pt);
		if (id == -1)
		{
			cur_sel_label_id = -1;
			initPlotItemGroup();
		}
		else if (id != cur_sel_label_id)
		{
			cur_sel_label_id = id;
			int type = (*plotFrames)[cur_slider].img_items[cur_sel_label_id].type;
			bool mirrored = (*plotFrames)[cur_slider].img_items[cur_sel_label_id].mirrored;
			std::string&str = (*plotFrames)[cur_slider].img_items[cur_sel_label_id].img_paths;
			if (!mirrored)
				plot_item_label->setPixmap(QPixmap::fromImage(QImage(std::string(g_pGlobalSys->assetPath+str).c_str()).copy(0,type*64,48,64)));
			else
				plot_item_label->setPixmap(QPixmap::fromImage(QImage(std::string(g_pGlobalSys->assetPath+str).c_str()).copy(0, type * 64, 48, 64).mirrored(true,false)));
			plot_item_comboBox->setCurrentIndex(type);
			plot_item_checkBox->setChecked(mirrored);
		}
		button_state = 1;
	}
}

void QPlotFrame::mouseReleaseEvent(QMouseEvent * mouse_event)
{
	button_state = 0;
}

void QPlotFrame::mouseMoveEvent(QMouseEvent * mouse_event)
{
	if (button_state == 1 && cur_sel_label_id != -1)
	{
		int id = cur_sel_label_id;
		QPoint pt = mapFromGlobal(QCursor::pos());
		QLabel* label = labels[id];
		labels[id]->setGeometry(pt.x(), pt.y(), label->width(), label->height());
		(*plotFrames)[cur_slider].img_items[id].pos.x = pt.x();
		(*plotFrames)[cur_slider].img_items[id].pos.y = pt.y();
	}
}

void QPlotFrame::changeCurItemImg()
{
	if (cur_sel_label_id == -1)
		return;
	QString filename = QFileDialog::getOpenFileName(this, tr("Load plot item image"), std::string(g_pGlobalSys->assetPath+"pmapobj").c_str(), "Image files(*.jpg *.png *.bmp)", 0);
	if (!filename.size())
		return;
	
	std::string img_name = filename.toStdString();
	int slash_ind = img_name.find_last_of('/');
	std::string local_name ="pmapobj/"+img_name.substr(slash_ind + 1, img_name.length() - slash_ind);
	(*plotFrames)[cur_slider].img_items[cur_sel_label_id].img_paths = local_name;
	images[cur_sel_label_id] = QImage(filename).copy(0, 0, 48, 64);
	labels[cur_sel_label_id]->setPixmap(QPixmap::fromImage(images[cur_sel_label_id]));
	plot_item_label->setPixmap(*labels[cur_sel_label_id]->pixmap());
	(*plotFrames)[cur_slider].img_items[cur_sel_label_id].type = 0;
	plot_item_comboBox->setCurrentIndex(0);
	(*plotFrames)[cur_slider].img_items[cur_sel_label_id].mirrored = false;
	plot_item_checkBox->setChecked(false);
}

void QPlotFrame::changeCurItemType(int index)
{
	if (index < 0 || cur_sel_label_id == -1)
		return;
	std::string filename = g_pGlobalSys->assetPath + (*plotFrames)[cur_slider].img_items[cur_sel_label_id].img_paths;
	images[cur_sel_label_id] = QImage(filename.c_str()).copy(0, index*64, 48, 64);
	if ((*plotFrames)[cur_slider].img_items[cur_sel_label_id].mirrored)
		images[cur_sel_label_id] = images[cur_sel_label_id].mirrored(true, false);
	labels[cur_sel_label_id]->setPixmap(QPixmap::fromImage(images[cur_sel_label_id]));
	plot_item_label->setPixmap(*labels[cur_sel_label_id]->pixmap());
	(*plotFrames)[cur_slider].img_items[cur_sel_label_id].type = index;
}

void QPlotFrame::mirrorCurItemImg(int state)
{
	if (cur_sel_label_id == -1)
		return;
	std::string filename = g_pGlobalSys->assetPath + (*plotFrames)[cur_slider].img_items[cur_sel_label_id].img_paths;
	int type = (*plotFrames)[cur_slider].img_items[cur_sel_label_id].type;
	if (state == 0)
	{
		images[cur_sel_label_id] = QImage(filename.c_str()).copy(0, type * 64, 48, 64);
		(*plotFrames)[cur_slider].img_items[cur_sel_label_id].mirrored = false;
	}
	else
	{
		images[cur_sel_label_id] = QImage(filename.c_str()).copy(0, type * 64, 48, 64).mirrored(true,false);
		(*plotFrames)[cur_slider].img_items[cur_sel_label_id].mirrored = true;
	}
	labels[cur_sel_label_id]->setPixmap(QPixmap::fromImage(images[cur_sel_label_id]));
	plot_item_label->setPixmap(*labels[cur_sel_label_id]->pixmap());
}
