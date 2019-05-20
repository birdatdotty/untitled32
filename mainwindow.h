#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QPushButton>

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QEventLoop>


class MainWindow : public QWidget
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  QGridLayout mainLayout;
  QLabel *lFile;
  QLabel *lUrl;
  QLineEdit *eFile;
  QLineEdit *eUrl;
  QPushButton *button;
  QString filePath;
  QString url;

private slots:
  void btnClick();
  void getUrl();
  void replyFinished(QNetworkReply*);
//  QEventLoop _loop;

};

#endif // MAINWINDOW_H
