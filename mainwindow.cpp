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

void libconfigObjToJsonObj(const libconfig::Setting &libconfigObj, rapidjson::PrettyWriter<rapidjson::StringBuffer> &jsonObj)
{
  using namespace libconfig;

  if(libconfigObj.isAggregate())
  {
    if(libconfigObj.isList() || libconfigObj.isArray())
    {
      jsonObj.StartArray();
      for(auto &item : libconfigObj)
      {
        jsonObj.Key(item.getName());
        libconfigObjToJsonObj(item, jsonObj);
      }
      jsonObj.EndArray();
    }
    else if(libconfigObj.isGroup())
    {
      jsonObj.StartObject();
      for(auto &item : libconfigObj)
      {
        jsonObj.Key(item.getName());
        libconfigObjToJsonObj(item, jsonObj);
      }
      jsonObj.EndObject();
    }
  }
  else if(libconfigObj.isScalar())
  {
    switch(libconfigObj.getType())
    {
     case Setting::Type::TypeInt:
        jsonObj.Int(libconfigObj);
        break;

     case Setting::Type::TypeInt64:
        jsonObj.Int64(libconfigObj);
        break;

     case Setting::Type::TypeFloat:
        jsonObj.Double(libconfigObj);
        break;

     case Setting::Type::TypeBoolean:
        jsonObj.Bool(libconfigObj);
        break;

     case Setting::Type::TypeString:
        jsonObj.String(libconfigObj);
        break;

     default:
        break;
    }
  }
}

QString MainWindow::libconfigToJson(QString libconfigStr)
{
  libconfig::Config libconfig;

  try
  {
    libconfig.readString(libconfigStr.toStdString());
  }
  catch(const libconfig::ParseException &ex)
  {
    return QString("Libconfig text parsing error!\n") + ex.what();
  }

  rapidjson::StringBuffer jsonBuffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> jsonWriter(jsonBuffer);

  try
  {
    const libconfig::Setting& libconfigRoot = libconfig.getRoot();
    libconfigObjToJsonObj(libconfigRoot, jsonWriter);
  }
  catch(const libconfig::ParseException &ex)
  {
    return QString("Libconfig text parsing error!\n") + ex.what();
  }

  return jsonBuffer.GetString();
}

void MainWindow::on_libconfigBrowser_textChanged()
{
  QString libconfigObj = ui->libconfigBrowser->toPlainText();
  QString jsonObj = libconfigToJson(libconfigObj);

  ui->jsonBrowser->setText(jsonObj);
}
