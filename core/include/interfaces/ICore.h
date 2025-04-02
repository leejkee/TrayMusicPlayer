//
// Created by cww on 25-4-2.
//
#pragma once
#include <QObject>

namespace Core {
class ICore : public QObject {
  Q_OBJECT
  public:
    ICore(QObject *parent) : QObject(parent){}



};
}
