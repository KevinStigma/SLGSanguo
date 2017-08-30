#ifndef QPLOT_FRAME
#define QPLOT_FRAME
#include<QFrame>
#include<QMenu>
#include<QAction>
#include<QImage>
#include<QLabel>
#include<QComboBox>
#include<QCheckBox>
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
	void setplotItemLabel(QLabel*label){ plot_item_label = label; }
	void setPlotComboBox(QComboBox* box){ plot_item_comboBox = box; }
	void setPlotCheckBox(QCheckBox* box){ plot_item_checkBox = box; }
	void updateCurFrame(int val);
	void updateDialog(int cur_fra);
	std::string getBackgroundPath(){ return bg_local_path; }
	void setBackgroundPath(std::string str);
public slots:
	void insertImage();
	void deleteImage();
	void changeCurItemImg();
	void changeCurItemType(int index);
	void mirrorCurItemImg(int state);
protected:
	void clearLabels();
	void updateLabels(int cur_fra);
	void loadImages(int cur_fra);
	void initPlotItemGroup();
	int getSeledImgId(QPoint pt);
	void contextMenuEvent(QContextMenuEvent *e);
	void mouseDoubleClickEvent(QMouseEvent * e);
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
private:
	QMenu pop_menu;
	QAction insertImg;
	QAction deleteImg;
	int cur_sel_label_id;
	int button_state;
	std::vector<PlotFrame> *plotFrames;
	std::vector<QImage> images;
	std::vector<QLabel*> labels;
	std::string bg_local_path;
	int cur_slider;
	QLabel* bg_label;
	QLabel* plot_item_label;
	QCheckBox* plot_item_checkBox;
	QComboBox* plot_item_comboBox;
	QLabel dialog_label;
};

#endif