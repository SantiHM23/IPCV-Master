#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QVector3D>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


QJsonArray vectorToJson(const QVector3D &vector) {
  QJsonArray result;
  for (int dim = 0; dim < 3; dim++)
    result.append(QJsonValue(vector[dim]));
  return result;
}

class Face {
public:
  std::vector<QVector3D> vertices;
  QVector3D normal;
  float c;

  Face(vector<QVector3D> ver, QVector3D nor, float col){
      c = col;
      normal = nor;
      for(int i=0; i<ver.size(); i++)
          vertices.push_back(ver[i]);
  }

  QJsonObject toJson() const {
    QJsonArray vertices_json;
    for (const QVector3D &v : vertices)
      vertices_json.append(vectorToJson(v));
    QJsonObject result;
    result["vertices"] = vertices_json;
    result["normal"] = vectorToJson(normal);
    result["color"] = c;
    return result;
  }
};

std::vector<Face> pgm3DToFaces(const std::string &path) {
  std::vector<Face> result;
  // TODO implement

  //  reading from input
  ifstream myfile (path);
  string line;
  if (myfile.is_open()){

    getline(myfile,line);

    // checking if header is PGM3D
    if(line!="PGM3D"){
        cout << "Wrong header! Should be PGM3D!" << endl;
        exit(1);
    }

    getline(myfile,line);

    // reading number of columns, rows and layers
    std::string delimiter = " ";

    size_t pos = 0;
    std::string token;
    int col_num, row_num, lay_num;
    int read = 0;
    while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);
        if(read == 0)
            col_num = stoi(token);
        else if(read == 1)
            row_num = stoi(token);
        line.erase(0, pos + delimiter.length());
        read++;
    }

    if(read!=2){
        cout << "Wrong number of dimensions! (Must be 3) " << endl;
        exit(2);
    }
    lay_num = stoi(token);
//   cout << "Column number: " << col_num << endl;
//   cout << "Row number: " << row_num << endl;
//   cout << "Layer number: " << lay_num << endl;

    // reading maximum value in 3DMatrix
    int max_val;
    getline(myfile,line);
    max_val = stoi(line);
    if(max_val<=0){
        cout << "Negative maximum value (or 0) selected" << endl;
        exit(5);
    }

    // Allocating space for 3DMatrix
    int*** mat3d = new int**[lay_num+2];
    for(int i=0; i<lay_num+2; i++)
    {
        mat3d[i] = new int*[row_num+2];
        for(int j=0; j<row_num+2; j++){
            mat3d[i][j] = new int[col_num+2];
            for(int k=0; k<col_num+2; k++)
                mat3d[i][j][k] = 0;
        }
    }

    // Filling 3DMatrix with values
    int i=1,j=1,k=1;
    int rows = 3;
    while(getline(myfile,line)){
        if(i==col_num+1){
            cout << "More data provided than size of matrix specified!" << endl;
            exit(3);
        }
        rows++;
        if(stoi(line)>max_val){
            cout << "Value higher than maximum value found!" << endl;
            exit(4);
        }
        mat3d[i][j][k] = stoi(line);
        k++;
        if(j==row_num && k == col_num+1){
            j = 1;
            k = 1;
            i++;
        }

        if(k==col_num+1)
        {
            k = 1;
            j++;
        }
    }
    if(i != lay_num+1 || j!= 1 || k != 1){
        cout << "Less data provided than size of matrix specified!" << endl;
        exit(3);
    }

    // finding faces inside matrix
    // iterating through each of dimensions and checking if some of 6-neighbors has different value than current pixel. If so, we calculate coordinates of vertices and add the face into a vector of faces.

    for(int k=1; k<col_num+1; k++){
        for(int j=1; j<row_num+1; j++){
            for(int i=1; i<lay_num+1; i++){

                if(mat3d[i][j][k]!=mat3d[i][j][k-1]){
                    QVector3D normal(-1,0,0);
                    float c = float(mat3d[i][j][k])/float(max_val);
                    QVector3D ver1(k-1-col_num/2.,j-1-row_num/2.,i-1-lay_num/2.);
                    QVector3D ver2(k-1-col_num/2.,j-1-row_num/2.,i-lay_num/2.);
                    QVector3D ver3(k-1-col_num/2.,j-row_num/2.,i-lay_num/2.);
                    QVector3D ver4(k-1-col_num/2.,j-row_num/2.,i-1-lay_num/2.);
                    vector<QVector3D> vertices;
                    vertices.push_back(ver1);
                    vertices.push_back(ver2);
                    vertices.push_back(ver3);
                    vertices.push_back(ver4);
                    Face new_face(vertices,normal,c);
                    result.push_back(new_face);
                }
                if(mat3d[i][j][k]!=mat3d[i][j][k+1]){
                    QVector3D normal(1,0,0);
                    float c = float(mat3d[i][j][k])/float(max_val);
                    QVector3D ver1(k-col_num/2.,j-1-row_num/2.,i-1-lay_num/2.);
                    QVector3D ver2(k-col_num/2.,j-1-row_num/2.,i-lay_num/2.);
                    QVector3D ver3(k-col_num/2.,j-row_num/2.,i-lay_num/2.);
                    QVector3D ver4(k-col_num/2.,j-row_num/2.,i-1-lay_num/2.);
                    vector<QVector3D> vertices;
                    vertices.push_back(ver1);
                    vertices.push_back(ver2);
                    vertices.push_back(ver3);
                    vertices.push_back(ver4);
                    Face new_face(vertices,normal,c);
                    result.push_back(new_face);
                }
                if(mat3d[i][j][k]!=mat3d[i][j-1][k]){
                    QVector3D normal(0,-1,0);
                    float c = float(mat3d[i][j][k])/float(max_val);
                    QVector3D ver1(k-1-col_num/2.,j-1-row_num/2.,i-1-lay_num/2.);
                    QVector3D ver2(k-col_num/2.,j-1-row_num/2.,i-1-lay_num/2.);
                    QVector3D ver3(k-col_num/2.,j-1-row_num/2.,i-lay_num/2.);
                    QVector3D ver4(k-1-col_num/2.,j-1-row_num/2.,i-lay_num/2.);
                    vector<QVector3D> vertices;
                    vertices.push_back(ver1);
                    vertices.push_back(ver2);
                    vertices.push_back(ver3);
                    vertices.push_back(ver4);
                    Face new_face(vertices,normal,c);
                    result.push_back(new_face);
                }
                if(mat3d[i][j][k]!=mat3d[i][j+1][k]){
                    QVector3D normal(0,1,0);
                    float c = float(mat3d[i][j][k])/float(max_val);
                    QVector3D ver1(k-1-col_num/2.,j-row_num/2.,i-1-lay_num/2.);
                    QVector3D ver2(k-col_num/2.,j-row_num/2.,i-1-lay_num/2.);
                    QVector3D ver3(k-col_num/2.,j-row_num/2.,i-lay_num/2.);
                    QVector3D ver4(k-1-col_num/2.,j-row_num/2.,i-lay_num/2.);
                    vector<QVector3D> vertices;
                    vertices.push_back(ver1);
                    vertices.push_back(ver2);
                    vertices.push_back(ver3);
                    vertices.push_back(ver4);
                    Face new_face(vertices,normal,c);
                    result.push_back(new_face);
                }
                if(mat3d[i][j][k]!=mat3d[i-1][j][k]){
                    QVector3D normal(0,0,-1);
                    float c = float(mat3d[i][j][k])/float(max_val);
                    QVector3D ver1(k-1-col_num/2.,j-1-row_num/2.,i-1-lay_num/2.);
                    QVector3D ver2(k-1-col_num/2.,j-row_num/2.,i-1-lay_num/2.);
                    QVector3D ver3(k-col_num/2.,j-row_num/2.,i-1-lay_num/2.);
                    QVector3D ver4(k-col_num/2.,j-1-row_num/2.,i-1-lay_num/2.);
                    vector<QVector3D> vertices;
                    vertices.push_back(ver1);
                    vertices.push_back(ver2);
                    vertices.push_back(ver3);
                    vertices.push_back(ver4);
                    Face new_face(vertices,normal,c);
                    result.push_back(new_face);
                }
                if(mat3d[i][j][k]!=mat3d[i+1][j][k]){
                    QVector3D normal(0,0,1);
                    float c = float(mat3d[i][j][k])/float(max_val);
                    QVector3D ver1(k-1-col_num/2.,j-1-row_num/2.,i-lay_num/2.);
                    QVector3D ver2(k-1-col_num/2.,j-row_num/2.,i-lay_num/2.);
                    QVector3D ver3(k-col_num/2.,j-row_num/2.,i-lay_num/2.);
                    QVector3D ver4(k-col_num/2.,j-1-row_num/2.,i-lay_num/2.);
                    vector<QVector3D> vertices;
                    vertices.push_back(ver1);
                    vertices.push_back(ver2);
                    vertices.push_back(ver3);
                    vertices.push_back(ver4);
                    Face new_face(vertices,normal,c);
                    result.push_back(new_face);
                }

            }
        }
            }
  }
  else{
      cout << "Wrong path specified!" << endl;
      exit(6);
  }
  return result;
}

void usage(int argc, char **argv) {
  (void)argc;
  std::cerr << "Usage: " << argv[0] << " <input.pgm3d>" << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    usage(argc, argv);
  }
  std::vector<Face> faces = pgm3DToFaces(argv[1]);
  QJsonArray faces_json;
  for (const Face &t : faces) {
    faces_json.append(t.toJson());
  }
  QString json_str(QJsonDocument(faces_json).toJson());
  ofstream output("our_output.json");
  output << json_str.toStdString() << std::endl;
  output.close();
  std::cout << json_str.toStdString() << std::endl;
}

