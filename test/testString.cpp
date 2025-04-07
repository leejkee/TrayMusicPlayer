//
// Created by cww on 25-4-7.
//
#include <string>
#include <QDebug>
#include <QString>
int main(){


  const QString qstr("C:/Users/cww/Music/music/MyMudic/ZhangJie/剑心 - 张杰.mp3");
  const std::wstring w_str = qstr.toStdWString();
  const std::string utf8_str = qstr.toStdString();
  const char *c_str_utf8 = utf8_str.c_str();
  const wchar_t *w_chat_str = w_str.c_str();
  // qDebug() << c_str_utf8;

}
