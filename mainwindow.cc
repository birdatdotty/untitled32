#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTimer>

static
QJsonObject ObjectFromString(const QString& in)
{
    QJsonObject obj;

    QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());

    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            obj = doc.object();
        }
    }
    return obj;
}

MainWindow::MainWindow(QWidget *parent) :
  QWidget(parent)
{
  button = new QPushButton("Button");
  lFile = new QLabel("Файл:");
  lUrl = new QLabel("Url:");

  eFile = new QLineEdit("/tmp/123.csv");
  eUrl = new QLineEdit("https://chain.api.btc.com/v3/block/3");

  connect(button, &QPushButton::released,
          this, &MainWindow::btnClick);
  connect(eFile, &QLineEdit::textChanged,
          [=](const QString&newFile){filePath = newFile;} );
  connect(eUrl, &QLineEdit::textChanged,
          [=](const QString&newUrl){url = newUrl;} );
  filePath = eFile->text();
  url = eUrl->text();

  mainLayout.addWidget(lFile,1,0);
  mainLayout.addWidget(eFile,1,1);
  mainLayout.addWidget(lUrl,2,0);
  mainLayout.addWidget(eUrl,2,1);

  mainLayout.addWidget(button,3,0,1,2);

  setLayout(&mainLayout);
  resize(800,300);
}

MainWindow::~MainWindow()
{
  delete button;
  delete lFile;
  delete lUrl;
}

void MainWindow::btnClick()
{
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout,
          this, &MainWindow::getUrl);
  timer->start(3000);
}

void MainWindow::getUrl()
{
  qInfo() << "getUrl";
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  connect(manager, &QNetworkAccessManager::finished,
          this, &MainWindow::replyFinished);

  manager->get(QNetworkRequest(QUrl(url)));
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
  QString header;
  QString newLine;
  QJsonObject obj = ObjectFromString(reply->readAll());
  QJsonObject data = obj["data"].toObject();
  foreach (auto key, data.keys())
    {
      if (!data[key].isObject() )
        {
          header += key + "\t";

          QJsonValue obj = data[key];
          QJsonValue::Type t = obj.type();
          switch (t) {
            case QJsonValue::Bool:
                newLine += QString::number(obj.toBool()) + "\t";
                break;
            case QJsonValue::Double:
                newLine +=QString::number(obj.toDouble()) + "\t";
                break;
            case QJsonValue::String:
                newLine += obj.toString() + "\t";
                break;
          }
        }
    }


  QFile file(filePath);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
      return;
  bool emptyFile = false;
  if (file.pos() == 0)
    emptyFile = true;

  QTextStream out(&file);
  if (emptyFile)
    out << header << "\n";

  out << newLine << "\n";
  file.close();
}
