#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include <QtWidgets/QMainWindow>
#include <QKeyEvent>
#include "ui_leveleditor.h"
#include "DataStructure.h"

class LevelEditor : public QMainWindow
{
	Q_OBJECT

public:
	LevelEditor(QWidget *parent = 0);
	~LevelEditor();
public slots:
	void on_addTextButton_clicked();
	void on_editTextButton_clicked();
	void on_loadImgButton_clicked();
	void on_deleteTextButton_clicked();
	void on_newframe_pushButton_clicked();
	void on_newplotframe_pushButton_clicked();
	void on_defineTxt_pushButton_clicked();
	void on_loadCharacterImg_pushButton_clicked();
	void on_insertPlotDialog_pushButton_clicked();

	void on_plotname_lineEdit_editingFinished();
	void on_scenename_lineEdit_editingFinished();
	void on_charactername_lineEdit_editingFinished();

	void on_voiceover_listWidget_currentRowChanged(int);
	void on_worldframe_scrollBar_valueChanged(int);
	void on_plot_scrollBar_valueChanged(int);

	void on_cur_frame_lineEdit_editingFinished();
	void on_cur_plotframe_lineEdit_editingFinished();
	void on_actionSave_Xml_triggered();
	void on_actionLoad_Xml_triggered();
	void on_actionConvertImage_triggered();

protected:
	void exportVoiceoverXml(std::string file_name);
	void exportWorldMapXml(std::string file_name);
	void exportPlotXml(std::string file_name);
	void loadVoiceoverXml(std::string file_name);
	void loadWorldMapXml(std::string file_name);
	void loadPlotXml(std::string file_name);
	void initVoiceover();
private:
	Ui::LevelEditorClass ui;
	std::vector<WorldMapFrame> worldMapFrames;
	std::vector<PlotFrame> plotFrames;
	std::string character_local_name;
};

#endif // LEVELEDITOR_H
