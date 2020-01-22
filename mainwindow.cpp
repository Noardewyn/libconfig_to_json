#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <libconfig.h++>
#include "./rapidjson/stringbuffer.h"
#include "./rapidjson/prettywriter.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_clicked()
{
  QString libconfigObj = ui->libconfigBrowser->toPlainText();
  QString jsonObj = libconfigToJson(libconfigObj);

  ui->jsonBrowser->setText(jsonObj);
}

void libconfigObjToJsonObj(const libconfig::Setting &libconfig, rapidjson::PrettyWriter<rapidjson::StringBuffer> &json)
{
  using namespace libconfig;

  if(libconfig.isAggregate())
  {
    if(libconfig.isList() || libconfig.isArray())
    {
      json.StartArray();
      for(auto &item : libconfig)
      {
        json.Key(item.getName());
        libconfigObjToJsonObj(item, json);
      }
      json.EndArray();
    }
    else if(libconfig.isGroup())
    {
      json.StartObject();
      for(auto &item : libconfig)
      {
        json.Key(item.getName());
        libconfigObjToJsonObj(item, json);
      }
      json.EndObject();
    }
  }
  else if(libconfig.isScalar())
  {
    switch( libconfig.getType() )
    {
     case Setting::Type::TypeInt:
        json.Int(libconfig);
        break;

     case Setting::Type::TypeInt64:
        json.Int64(libconfig);
        break;

     case Setting::Type::TypeFloat:
        json.Double(libconfig);
        break;

     case Setting::Type::TypeBoolean:
        json.Bool(libconfig);
        break;

     case Setting::Type::TypeString:
        json.String(libconfig);
        break;

     default:
        break;
    }
  }
}

QString MainWindow::libconfigToJson(QString libconfigStr)
{
  libconfig::Config cfg;
  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

  try
  {
    cfg.readString(libconfigStr.toStdString());
  }
  catch(const libconfig::ParseException &ex)
  {
    return QString("PARSING ERROR!\n") + ex.what();
  }

  const libconfig::Setting& libconfigRoot = cfg.getRoot();

  libconfigObjToJsonObj(libconfigRoot, writer);

  return buffer.GetString();
}
