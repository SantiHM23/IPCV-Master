#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QVector3D>

QJsonArray vectorToJson(const QVector3D &vector);
QVector3D vectorFromJson(const QJsonArray &array);

class Face {
public:
  std::vector<QVector3D> vertices;
//  double z;
  QVector3D normal;
  float c;
//  int idx; // index in face_collection array


  QJsonObject toJson() const;
  void fromJson(const QJsonObject &json);
};

class FaceCollection {
public:
  std::vector<Face> faces;

  void fromJson(const QJsonArray &json);
};

class Sorting_Object {
public:
    double z;
    int idx;
    Sorting_Object(double z1, int idx1){
        z = z1;
        idx = idx1;
    }
};
