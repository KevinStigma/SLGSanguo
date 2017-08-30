#ifndef QWORLDMAP_FRAME
#define QWORLDMAP_FRAME
#include<QFrame>
#include<QMenu>
#include<QAction>
#include<QImage>
#include<QLabel>
#include<QContextMenuEvent>
#include "DataStructure.h"
class QWorldMapFrame:public QFrame
{
	Q_OBJECT
public:
	QWorldMapFrame(QWidget* parent = 0, Qt::WindowFlags f = 0);
	~QWorldMapFrame();
	void setWorldMapFrames(std::vector<WorldMapFrame>* p){ worldMapFrames = p; }
	void updateCurFrame(int val);
protected:
	void contextMenuEvent(QContextMenuEvent *e);
	void loadImages(int cur_val);
	void updateLabels(int cur_val);
	void clearLabels();
	int getSeledImgId(QPoint pt);
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);

public slots:
	void insertImage();
	void deleteImage();
	void changeImage();
private:
	QMenu pop_menu;
	QAction insertImg;
	QAction deleteImg;
	QAction changeImg;
	std::vector<WorldMapFrame>* worldMapFrames;
	std::vector<QImage> images;
	std::vector<QLabel*> labels;
	int cur_slider,cur_sel_label_id;
	int button_state;
	QPoint init_pt;
};
#endif