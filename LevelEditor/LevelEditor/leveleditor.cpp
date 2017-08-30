#include "leveleditor.h"
#include "tinyxml2.h"
#include <iostream>
#include <cassert>
#include <QFileDialog>

LevelEditor::LevelEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.tabWidget->setStyleSheet("QTabWidget:pane{ border: 1px solid grey; top: -1px; background - color:grey;}\
	QTabBar::tab{ height:22px; background - color:grey; margin - right: 2px; margin - bottom:-2px; }\
	QTabBar::tab:selected{ border:1px solid grey; border - bottom - color: none; }\
	QTabBar::tab:!selected{ border - bottom: 3px solid grey; }");
	ui.world_map_label->setPixmap(QPixmap::fromImage(QImage("../Data/Assets/Maps/1- (114).JPG")));
	ui.worldframe_scrollBar->setMaximum(0);
	ui.cur_frame_lineEdit->setText("0");
	worldMapFrames.resize(1);
	plotFrames.resize(1);
	ui.frame->setWorldMapFrames(&worldMapFrames);
	ui.frame2->setBackgroundLabel(ui.plotbg_label);
	ui.frame2->setPlotFrames(&plotFrames);
	ui.frame2->setplotItemLabel(ui.plotItemlabel);
	ui.frame2->setPlotComboBox(ui.comboBox_2);
	ui.frame2->setPlotCheckBox(ui.mirror_checkBox);
	
	connect(ui.changeItemImg_pushButton, SIGNAL(clicked()), ui.frame2, SLOT(changeCurItemImg()));
	connect(ui.comboBox_2, SIGNAL(currentIndexChanged(int)), ui.frame2, SLOT(changeCurItemType(int)));
	connect(ui.mirror_checkBox, SIGNAL(stateChanged(int)), ui.frame2, SLOT(mirrorCurItemImg(int)));
}

LevelEditor::~LevelEditor()
{

}

void LevelEditor::on_addTextButton_clicked()
{
	QString text=ui.voiceover_textEdit->toPlainText();
	if (!text.size())
		return;
	int ind=ui.voiceover_listWidget->currentRow();
	if (ind == -1||ind==ui.voiceover_listWidget->count()-1)
		ui.voiceover_listWidget->addItem(text);
	else
		ui.voiceover_listWidget->insertItem(ind + 1, text);
	ui.voiceover_textEdit->setText("");
	ui.voiceover_listWidget->setCurrentRow(-1);
}

void LevelEditor::on_editTextButton_clicked()
{
	if (ui.voiceover_listWidget->currentRow() == -1)
		return;
	auto item = ui.voiceover_listWidget->item(ui.voiceover_listWidget->currentRow());
	QString txt = ui.voiceover_textEdit->toPlainText();
	if (!txt.size())
		return;
	item->setText(txt);
}

void LevelEditor::on_loadImgButton_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load background"), "../Data/Assets/Maps", "Image files(*.jpg *.png *.bmp)", 0);
	if (!filename.size())
		return;
	std::string img_name = filename.toStdString();
	int slash_ind=img_name.find_last_of('/');
	std::string local_name = "Maps/"+img_name.substr(slash_ind+1,img_name.length()-slash_ind);
	
	QImage image = QImage(filename).scaled(ui.bg_label->width(),ui.bg_label->height());
	ui.bg_label->setPixmap(QPixmap::fromImage(image));
	ui.local_img_label->setText(QString(local_name.c_str()));
}

void LevelEditor::on_deleteTextButton_clicked()
{
	if (ui.voiceover_listWidget->currentRow() != -1)
	{
		int row = ui.voiceover_listWidget->currentRow();
		if (row!=-1)
			ui.voiceover_listWidget->takeItem(row);
		ui.voiceover_listWidget->setCurrentRow(-1);
	}
}

void LevelEditor::on_newframe_pushButton_clicked()
{
	int num = ui.worldframe_scrollBar->maximum();
	ui.worldframe_scrollBar->setMaximum(num + 1);
	ui.cur_frame_lineEdit->setText(QString::number(ui.worldframe_scrollBar->sliderPosition()));
	ui.frame_num_label->setText(QString("frame num:")+QString::number(num+2));
	worldMapFrames.push_back(worldMapFrames.back());
}

void LevelEditor::on_newplotframe_pushButton_clicked()
{
	if (!ui.plotbg_label->pixmap())
		return;
	int num = ui.plot_scrollBar->maximum();
	ui.plot_scrollBar->setMaximum(num + 1);
	ui.cur_plotframe_lineEdit->setText(QString::number(ui.plot_scrollBar->sliderPosition()));
	ui.plotframe_num_label->setText(QString("frame num:") + QString::number(num + 2));
	plotFrames.push_back(plotFrames.back());
}

void LevelEditor::on_defineTxt_pushButton_clicked()
{
	int cur_id = ui.worldframe_scrollBar->sliderPosition();
	assert(cur_id <worldMapFrames.size());
	worldMapFrames[cur_id].txt = ui.worldMap_textEdit->toPlainText().toStdString();
}

void LevelEditor::on_loadCharacterImg_pushButton_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load character image"), "../Data/Assets/Face", "Image files(*.jpg *.png *.bmp)", 0);
	if (!filename.size())
		return;
	std::string img_name = filename.toStdString();
	int slash_ind = img_name.find_last_of('/');
	std::string local_name = "Face/" + img_name.substr(slash_ind + 1, img_name.length() - slash_ind);
	character_local_name = local_name;
	ui.characterImg_label->setPixmap(QPixmap::fromImage(QImage(filename)));
}

void LevelEditor::on_insertPlotDialog_pushButton_clicked()
{
	if (!ui.plotbg_label->pixmap())
		return;
	int cur_id = ui.plot_scrollBar->sliderPosition();
	assert(cur_id <plotFrames.size());
	if (!ui.characterImg_label->pixmap()||!ui.charactername_lineEdit->text().size())
	{
		SAFE_DELETE(plotFrames[cur_id].dialog_box);
	}
	else
	{
		if (!plotFrames[cur_id].dialog_box)
		{
			DialogBox*dialog = new DialogBox;
			dialog->character_name = ui.charactername_lineEdit->text().toStdString();
			dialog->character_path = character_local_name;
			dialog->pos = DialogPos(ui.comboBox->currentIndex());
			dialog->txt = ui.plot_textEdit->toPlainText().toStdString();
			plotFrames[cur_id].dialog_box = dialog;
		}
		else
		{
			DialogBox* dialog = plotFrames[cur_id].dialog_box;
			dialog->character_name = ui.charactername_lineEdit->text().toStdString();
			dialog->character_path = character_local_name;
			dialog->pos = DialogPos(ui.comboBox->currentIndex());
			dialog->txt = ui.plot_textEdit->toPlainText().toStdString();
		}
	}
	ui.frame2->updateDialog(cur_id);
}

void LevelEditor::on_plotname_lineEdit_editingFinished()
{
	ui.plotname_lineEdit->clearFocus();
}

void LevelEditor::on_scenename_lineEdit_editingFinished()
{
	ui.scenename_lineEdit->clearFocus();
}

void LevelEditor::on_charactername_lineEdit_editingFinished()
{
	ui.charactername_lineEdit->clearFocus();
}

void LevelEditor::on_voiceover_listWidget_currentRowChanged(int id)
{
	if (id < 0)
	{
		ui.voiceover_textEdit->setText("");
		return;
	}
	auto item=ui.voiceover_listWidget->item(id);
	ui.voiceover_textEdit->setText(item->text());
}

void LevelEditor::on_worldframe_scrollBar_valueChanged(int val)
{
	assert(val <worldMapFrames.size());
	ui.worldMap_textEdit->setText(worldMapFrames[val].txt.c_str());
	ui.cur_frame_lineEdit->setText(QString::number(val));
	ui.frame->updateCurFrame(val);
}

void LevelEditor::on_plot_scrollBar_valueChanged(int val)
{
	assert(val <plotFrames.size());
	if (plotFrames[val].dialog_box)
	{
		ui.plot_textEdit->setText(plotFrames[val].dialog_box->txt.c_str());
		ui.charactername_lineEdit->setText(plotFrames[val].dialog_box->character_name.c_str());
		ui.characterImg_label->setPixmap(QPixmap::fromImage(QImage(std::string("../Data/Assets/"+plotFrames[val].dialog_box->character_path).c_str())));
	}
	else
	{
		ui.plot_textEdit->setText("");
		ui.charactername_lineEdit->setText("");
		ui.characterImg_label->setPixmap(QPixmap::fromImage(QImage()));
	}
	ui.cur_plotframe_lineEdit->setText(QString::number(val));
	ui.frame2->updateCurFrame(val);
}

void LevelEditor::on_cur_frame_lineEdit_editingFinished()
{
	QString text=ui.cur_frame_lineEdit->text();
	if (!text.size())
	{
		int id = ui.worldframe_scrollBar->sliderPosition();
		ui.cur_frame_lineEdit->setText(QString::number(id));
	}
	else
	{
		int id=text.toInt();
		ui.worldframe_scrollBar->setValue(id);
	}
	ui.cur_frame_lineEdit->clearFocus();
}

void LevelEditor::on_cur_plotframe_lineEdit_editingFinished()
{
	QString text = ui.cur_plotframe_lineEdit->text();
	if (!text.size())
	{
		int id = ui.plot_scrollBar->sliderPosition();
		ui.cur_plotframe_lineEdit->setText(QString::number(id));
	}
	else
	{
		int id = text.toInt();
		ui.plot_scrollBar->setValue(id);
	}
	ui.cur_plotframe_lineEdit->clearFocus();
}

void LevelEditor::on_actionSave_Xml_triggered()
{
	QString name = QFileDialog::getSaveFileName(this, tr("Save XML"), ".", "xml files(*.xml)", 0);
	if (!name.size())
		return;
	if (ui.tabWidget->currentIndex() == 0 && ui.voiceover_listWidget->count() && ui.local_img_label->text().size())
		exportVoiceoverXml(name.toStdString());
	else if (ui.tabWidget->currentIndex() == 1 && !(worldMapFrames.size() == 1 && worldMapFrames[0].img_items.size() == 0 && worldMapFrames[0].txt.size() == 0))
		exportWorldMapXml(name.toStdString());
	else if (ui.tabWidget->currentIndex() == 2 && !(plotFrames.size() == 1 && plotFrames[0].img_items.size() == 0))
		exportPlotXml(name.toStdString());
}

void LevelEditor::exportVoiceoverXml(std::string file_name)
{
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	XMLElement* parent = doc.NewElement("Voiceover");
	doc.InsertFirstChild(parent);
	XMLElement* node = doc.NewElement("Background");
	node->SetAttribute("ImagePath",ui.local_img_label->text().toStdString().c_str());
	parent->InsertEndChild(node);
	
	node = doc.NewElement("Texts");

	for (int i = 0; i < (int)ui.voiceover_listWidget->count(); i++)
	{
		XMLElement* child_node = doc.NewElement("Text");
		auto item = ui.voiceover_listWidget->item(i);
		child_node->SetText(item->text().toStdString().c_str());
		node->InsertEndChild(child_node);
	}
	parent->InsertEndChild(node);
	doc.SaveFile(file_name.c_str());
	std::cout << "Export " << file_name << " successfully!" << std::endl;
}

void LevelEditor::exportWorldMapXml(std::string file_name)
{
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	XMLElement* parent = doc.NewElement("WorldMap");
	doc.InsertFirstChild(parent);
	XMLElement* node = doc.NewElement("Images");
	std::set<std::string> imgs;
	for (int i = 0; i < (int)worldMapFrames.size(); i++)
	{
		const auto & frame = worldMapFrames[i];
		for (int j = 0; j < (int)frame.img_items.size(); j++)
			imgs.insert(frame.img_items[j].img_paths);
	}
	for (auto it = imgs.begin(); it != imgs.end(); it++)
	{
		XMLElement* child_node = doc.NewElement("Path");
		child_node->SetText(it->c_str());
		node->InsertEndChild(child_node);
	}
	parent->InsertEndChild(node);

	node = doc.NewElement("Frames");
	for (int i = 0; i <(int)worldMapFrames.size() ; i++)
	{
		const auto & frame = worldMapFrames[i];
		XMLElement* child_node = doc.NewElement("Frame");
		
		XMLElement* cc_node = doc.NewElement("Txt");
		cc_node->SetText(frame.txt.c_str());
		child_node->InsertEndChild(cc_node);

		cc_node = doc.NewElement("Items");
		for (int j = 0; j < (int)frame.img_items.size(); j++)
		{
			XMLElement* item_node = doc.NewElement("Item");
			item_node->SetAttribute("ImagePath", frame.img_items[j].img_paths.c_str());
			item_node->SetAttribute("PosX", frame.img_items[j].pos.x);
			item_node->SetAttribute("PosY", frame.img_items[j].pos.y);

			cc_node->InsertEndChild(item_node);
		}
		child_node->InsertEndChild(cc_node);
		node->InsertEndChild(child_node);
	}
	parent->InsertEndChild(node);
	doc.SaveFile(file_name.c_str());
	std::cout << "Export " << file_name << " successfully!" << std::endl;
}

void LevelEditor::exportPlotXml(std::string file_name)
{
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	XMLElement* parent = doc.NewElement("Plot");
	parent->SetAttribute("PlotName",ui.plotname_lineEdit->text().toStdString().c_str());
	parent->SetAttribute("SceneName",ui.scenename_lineEdit->text().toStdString().c_str());
	parent->SetAttribute("Background",ui.frame2->getBackgroundPath().c_str());
	doc.InsertFirstChild(parent);
	XMLElement*node = doc.NewElement("Images");
	std::set<std::string> imgs;
	for (int i = 0; i < (int)plotFrames.size(); i++)
	{
		auto& frame = plotFrames[i];
		for (int j = 0; j <(int)frame.img_items.size(); j++)
			imgs.insert(frame.img_items[j].img_paths);
	}
	for (auto it = imgs.begin(); it != imgs.end(); it++)
	{
		XMLElement* child_node = doc.NewElement("Image");
		child_node->SetText((*it).c_str());
		node->InsertEndChild(child_node);
	}
	parent->InsertEndChild(node);

	node = doc.NewElement("Frames");
	for (int i = 0; i < (int)plotFrames.size(); i++)
	{
		auto& frame = plotFrames[i];
		XMLElement* frame_node = doc.NewElement("Frame");
		XMLElement* dialog_node = doc.NewElement("Dialog");
		if (!frame.dialog_box)
		{
			dialog_node->SetAttribute("Name", "");
			dialog_node->SetAttribute("ImgPath","");
			dialog_node->SetAttribute("Pos", 0);
			XMLElement*child_node = doc.NewElement("Txt");
			child_node->SetText("");
			dialog_node->InsertEndChild(child_node);
		}
		else
		{
			dialog_node->SetAttribute("Name", frame.dialog_box->character_name.c_str());
			dialog_node->SetAttribute("ImgPath", frame.dialog_box->character_path.c_str());
			dialog_node->SetAttribute("Pos", frame.dialog_box->pos);
			XMLElement*child_node = doc.NewElement("Txt");
			child_node->SetText(frame.dialog_box->txt.c_str());
			dialog_node->InsertEndChild(child_node);
		}
		frame_node->InsertEndChild(dialog_node);
		XMLElement* item_node = doc.NewElement("Items");
		for (int j = 0; j < frame.img_items.size(); j++)
		{
			XMLElement* child_node = doc.NewElement("Item");
			child_node->SetAttribute("ImagePath", frame.img_items[j].img_paths.c_str());
			child_node->SetAttribute("PosX", frame.img_items[j].pos.x);
			child_node->SetAttribute("PosY", frame.img_items[j].pos.y);
			child_node->SetAttribute("type",frame.img_items[j].type);
			child_node->SetAttribute("mirrored",frame.img_items[j].mirrored);
			item_node->InsertEndChild(child_node);
		}
		frame_node->InsertEndChild(item_node);
		node->InsertEndChild(frame_node);
	}
	parent->InsertEndChild(node);
	doc.SaveFile(file_name.c_str());
	std::cout << "Export " << file_name << " successfully!" << std::endl;
}

void LevelEditor::loadVoiceoverXml(std::string file_name)
{
	initVoiceover();
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	doc.LoadFile(file_name.c_str());
	XMLElement* parent = doc.FirstChildElement("Voiceover");
	XMLElement* node = parent->FirstChildElement("Background");
	std::string local_path(node->Attribute("ImagePath"));
	ui.local_img_label->setText(local_path.c_str());
	QImage image = QImage(std::string("../Data/Assets/"+local_path).c_str()).scaled(ui.bg_label->width(), ui.bg_label->height());
	ui.bg_label->setPixmap(QPixmap::fromImage(image));

	node = parent->FirstChildElement("Texts");
	XMLElement* item = node->FirstChildElement();
	for (; item; item = item->NextSiblingElement())
		ui.voiceover_listWidget->addItem(QString(item->GetText()));

	std::cout << "Load " << file_name << " successfully!" << std::endl;
}

void LevelEditor::loadWorldMapXml(std::string file_name)
{
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	doc.LoadFile(file_name.c_str());
	XMLElement* parent = doc.FirstChildElement("WorldMap");
	XMLElement* node = parent->FirstChildElement("Frames");

	worldMapFrames.clear();
	XMLElement* frame_item = node->FirstChildElement();
	for (; frame_item; frame_item = frame_item->NextSiblingElement())
	{
		XMLElement* txt_node = frame_item->FirstChildElement("Txt");
		WorldMapFrame frame;
		frame.txt = txt_node->GetText();
		XMLElement *items_node = frame_item->FirstChildElement("Items");
		XMLElement *item_node = items_node->FirstChildElement();
		for (; item_node; item_node = item_node->NextSiblingElement())
		{
			ImageItem image_item;
			image_item.img_paths=QString(item_node->Attribute("ImagePath")).toStdString();
			image_item.pos.x=QString(item_node->Attribute("PosX")).toInt();
			image_item.pos.y=QString(item_node->Attribute("PosY")).toInt();
			frame.img_items.push_back(image_item);
		}
		worldMapFrames.push_back(frame);
	}
	ui.worldframe_scrollBar->setRange(0,worldMapFrames.size()-1);
	ui.worldframe_scrollBar->setSliderPosition(0);
	ui.frame->updateCurFrame(0);
	std::cout << "Load " << file_name << " successfully!" << std::endl;
}

void LevelEditor::loadPlotXml(std::string file_name)
{
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	doc.LoadFile(file_name.c_str());
	XMLElement* parent = doc.FirstChildElement("Plot");
	ui.plotname_lineEdit->setText(QString(parent->Attribute("PlotName")));
	ui.scenename_lineEdit->setText(QString(parent->Attribute("SceneName")));
	ui.frame2->setBackgroundPath(QString(parent->Attribute("Background")).toStdString());

	XMLElement* node = parent->FirstChildElement("Frames");
	plotFrames.clear();
	XMLElement* frame_item = node->FirstChildElement();
	for (; frame_item; frame_item = frame_item->NextSiblingElement())
	{
		PlotFrame frame;
		XMLElement* dialog_node = frame_item->FirstChildElement("Dialog");
		std::string name = dialog_node->Attribute("Name");
		if (!name.size())
			frame.dialog_box = NULL;
		else
		{
			frame.dialog_box = new DialogBox;
			frame.dialog_box->character_name = name;
			frame.dialog_box->character_path = dialog_node->Attribute("ImgPath");
			frame.dialog_box->pos = DialogPos(QString(dialog_node->Attribute("pos")).toInt());
			XMLElement*txt_node = dialog_node->FirstChildElement("Txt");
			frame.dialog_box->txt = txt_node->GetText();
			ui.charactername_lineEdit->setText(name.c_str());
			ui.characterImg_label->setPixmap(QPixmap::fromImage(QImage(std::string("../Data/Assets/"+frame.dialog_box->character_path).c_str())));
			ui.plot_textEdit->setText(frame.dialog_box->txt.c_str());
			ui.comboBox->setCurrentIndex(frame.dialog_box->pos);
		}
		XMLElement *items_node = frame_item->FirstChildElement("Items");
		XMLElement *item_node = items_node->FirstChildElement();

		for (; item_node; item_node = item_node->NextSiblingElement())
		{
			ImageItem image_item;
			image_item.img_paths = QString(item_node->Attribute("ImagePath")).toStdString();
			image_item.pos.x = QString(item_node->Attribute("PosX")).toInt();
			image_item.pos.y = QString(item_node->Attribute("PosY")).toInt();
			image_item.mirrored = (QString(items_node->Attribute("mirrored")) == QString("true"));
			image_item.type = QString(items_node->Attribute("type")).toInt();
			frame.img_items.push_back(image_item);
		}
		plotFrames.push_back(frame);
	}
	ui.plot_scrollBar->setRange(0, plotFrames.size() - 1);
	ui.plot_scrollBar->setSliderPosition(0);
	ui.frame2->updateCurFrame(0);
	std::cout << "Load " << file_name << " successfully!" << std::endl;
}

void LevelEditor::initVoiceover()
{
	ui.bg_label->clear();
	ui.bg_label->setText("No Image");
	ui.voiceover_listWidget->setCurrentRow(-1);
	ui.voiceover_listWidget->clear();
	ui.local_img_label->setText("");
}

void LevelEditor::on_actionLoad_Xml_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load XML"), ".", "xml files(*.xml)", 0);
	if (!filename.size())
		return;
	if (ui.tabWidget->currentIndex() == 0)
		loadVoiceoverXml(filename.toStdString());
	else if (ui.tabWidget->currentIndex() == 1)
		loadWorldMapXml(filename.toStdString());
	else
		loadPlotXml(filename.toStdString());
}
