#ifndef GAMESINFOEDITOR_H
#define GAMESINFOEDITOR_H

#include <QtWidgets/QMainWindow>
#include <vector>
#include <string>
#include "ui_gamesinfoeditor.h"
#include "DataStructure.h"

class GamesInfoEditor : public QMainWindow
{
	Q_OBJECT

public:
	GamesInfoEditor(QWidget *parent = 0);
	~GamesInfoEditor();
public slots:
	void on_actionLoad_XML_triggered();
	void on_actionSave_XML_triggered();
	void on_listWidget_currentRowChanged(int);
	void setItemEditable(QListWidgetItem * item);
protected:
	void exportGeneralsXML(const std::string& file_name);
	void importGeneralsXML(const std::string& file_name);
private:
	Ui::GamesInfoEditorClass ui;
	std::vector<General> generals;
	int last_sel_row;
};

#endif // GAMESINFOEDITOR_H
