#include "gamesinfoeditor.h"
#include "tinyxml2.h"
#include <iostream>
#include <QString>
#include <QFileDialog>

GamesInfoEditor::GamesInfoEditor(QWidget *parent)
: QMainWindow(parent), last_sel_row(-1)
{
	ui.setupUi(this);
	connect(ui.listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(setItemEditable(QListWidgetItem *)));
	ui.tabWidget->setStyleSheet("QTabWidget:pane{ border: 1px solid grey; top: -1px; background - color:grey;}\
							QTabBar::tab{ height:22px; background - color:grey; margin - right: 2px; margin - bottom:-2px; }\
							QTabBar::tab:selected{ border:1px solid grey; border - bottom - color: none; }\
							QTabBar::tab:!selected{ border - bottom: 3px solid grey; }");
	ui.listWidget->setGenrals(&generals);
	ui.listWidget->setLastRow(&last_sel_row);
}

GamesInfoEditor::~GamesInfoEditor()
{
	
}


void GamesInfoEditor::on_actionLoad_XML_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Import XML"), ".", "xml files(*.xml)", 0);
	if (!filename.size())
		return;
	if (ui.tabWidget->currentIndex() == 0)
		importGeneralsXML(filename.toStdString());
}

void GamesInfoEditor::on_actionSave_XML_triggered()
{
	QString name = QFileDialog::getSaveFileName(this, tr("Export XML"), ".", "xml files(*.xml)", 0);
	if (!name.size())
		return;
	if (ui.tabWidget->currentIndex() == 0)
		exportGeneralsXML(name.toStdString());
}

void GamesInfoEditor::on_listWidget_currentRowChanged(int ind)
{
	if (last_sel_row >= 0)
	{
		General& cur_gen = generals[last_sel_row];
		cur_gen.wuli = atoi(ui.wuliEdit->text().toStdString().c_str());
		cur_gen.zhili = atoi(ui.zhiliEdit->text().toStdString().c_str());
		cur_gen.tongshuai = atoi(ui.tongshuaiEdit->text().toStdString().c_str());
		cur_gen.minjie = atoi(ui.minjieEdit->text().toStdString().c_str());
		cur_gen.yunqi = atoi(ui.yunqiEdit->text().toStdString().c_str());
		cur_gen.biography = ui.biographyEdit->toPlainText().toStdString();
		cur_gen.name = ui.listWidget->item(last_sel_row)->text().toStdString();
		cur_gen.critical = ui.criticalEdit->text().toStdString();
		cur_gen.retreate = ui.retreatEdit->text().toStdString();
		cur_gen.hpadd = atoi(ui.hpEdit->text().toStdString().c_str());
		cur_gen.mpadd = atoi(ui.mpEdit->text().toStdString().c_str());
	}
	last_sel_row = ind;
	if (ind < 0)
		return;
	const General & general = generals[ind];
	ui.wuliEdit->setText(QString::number(general.wuli));
	ui.zhiliEdit->setText(QString::number(general.zhili));
	ui.tongshuaiEdit->setText(QString::number(general.tongshuai));
	ui.yunqiEdit->setText(QString::number(general.yunqi));
	ui.minjieEdit->setText(QString::number(general.minjie));
	ui.biographyEdit->setText(general.biography.c_str());
	ui.criticalEdit->setText(general.critical.c_str());
	ui.retreatEdit->setText(general.retreate.c_str());
	ui.hpEdit->setText(QString::number(general.hpadd));
	ui.mpEdit->setText(QString::number(general.mpadd));
}

void GamesInfoEditor::setItemEditable(QListWidgetItem * item)
{
	item->setFlags(item->flags() | Qt::ItemIsEditable);
}

void GamesInfoEditor::exportGeneralsXML(const std::string& file_name)
{
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	XMLElement* parent = doc.NewElement("Generals");
	doc.InsertFirstChild(parent);
	for (int i = 0; i < (int)generals.size(); i++)
	{
		XMLElement* gen_node = doc.NewElement("General");
		gen_node->SetAttribute("Name", generals[i].name.c_str());
		gen_node->SetAttribute("ImgPath", generals[i].img_path.c_str());

		XMLElement* cap_node = doc.NewElement("Capacity");
		cap_node->SetAttribute("Wuli", QString::number(generals[i].wuli).toStdString().c_str());
		cap_node->SetAttribute("Zhili", QString::number(generals[i].zhili).toStdString().c_str());
		cap_node->SetAttribute("Tongshuai", QString::number(generals[i].tongshuai).toStdString().c_str());
		cap_node->SetAttribute("Minjie", QString::number(generals[i].minjie).toStdString().c_str());
		cap_node->SetAttribute("Yunqi", QString::number(generals[i].yunqi).toStdString().c_str());
		cap_node->SetAttribute("HpAdd", QString::number(generals[i].hpadd).toStdString().c_str());
		cap_node->SetAttribute("MpAdd", QString::number(generals[i].mpadd).toStdString().c_str());
		gen_node->InsertEndChild(cap_node);
		XMLElement* bio_node = doc.NewElement("Biography");
		bio_node->SetText(generals[i].biography.c_str());
		gen_node->InsertEndChild(bio_node);
		XMLElement* critical_node = doc.NewElement("Critical");
		critical_node->SetText(generals[i].critical.c_str());
		gen_node->InsertEndChild(critical_node);
		XMLElement* retreat_node = doc.NewElement("Retreat");
		retreat_node->SetText(generals[i].retreate.c_str());
		gen_node->InsertEndChild(retreat_node);

		parent->InsertEndChild(gen_node);
	}
	doc.SaveFile(file_name.c_str());
	std::cout << "Export " << file_name << " successfully!" << std::endl;
}

void GamesInfoEditor::importGeneralsXML(const std::string& file_name)
{
	ui.listWidget->setCurrentRow(-1);
	ui.listWidget->clear();
	generals.clear();
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	doc.LoadFile(file_name.c_str());
	XMLElement* root = doc.FirstChildElement("Generals");
	XMLElement* general_node = root->FirstChildElement();
	for (; general_node; general_node = general_node->NextSiblingElement())
	{
		General general;
		general.name = general_node->Attribute("Name");
		general.img_path = general_node->Attribute("ImgPath");
		XMLElement* cap_node = general_node->FirstChildElement();
		general.wuli = atoi(cap_node->Attribute("Wuli"));
		general.zhili = atoi(cap_node->Attribute("Zhili"));
		general.tongshuai = atoi(cap_node->Attribute("Tongshuai"));
		general.minjie = atoi(cap_node->Attribute("Minjie"));
		general.yunqi = atoi(cap_node->Attribute("Yunqi"));
		XMLElement* bio_node = cap_node->NextSiblingElement();
		if (bio_node->GetText())
			general.biography = bio_node->GetText();
		XMLElement* cri_node = bio_node->NextSiblingElement();
		if (cri_node->GetText())
			general.critical = cri_node->GetText();
		XMLElement* ret_node = cri_node->NextSiblingElement();
		if (ret_node->GetText())
			general.retreate = ret_node->GetText();
		generals.push_back(general);
	}
	ui.listWidget->updateListViewFromGenerals();
	std::cout << "Load " << file_name<< " successfully!" << std::endl;
}

