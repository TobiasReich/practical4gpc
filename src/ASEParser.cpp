#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <ASEParser.h>

using namespace std;

// Constants for parsing the aes file
const string MESH_VERTEX_LIST   = "*MESH_VERTEX_LIST";
const string MESH_FACE_LIST     = "*MESH_FACE_LIST";
const string MESH_NORMALS       = "*MESH_NORMALS";

const string MESH_VERTEX        = "*MESH_VERTEX";
const string MESH_FACENORMAL    = "*MESH_FACENORMAL";
const string MESH_VERTEXNORMAL  = "*MESH_VERTEXNORMAL";
const string MESH_FACE          = "*MESH_FACE";
const string END_TOKEN          = "}";

/** Constructor **/
ASEParser::ASEParser(string file) {
    cout << "### Creating ASEParser! ### - loading " + file << endl;
    filename = file;
}

/** Destructor **/
ASEParser::~ASEParser() {
    cout << "... all done! DESTROYING ASEParser!" << endl;
}

void ASEParser::readObject(std::vector<ASEParser::Vertex> &verts,
                           std::vector<ASEParser::Normal> &norms,
                           std::vector<ASEParser::Face> &faces) {
    fstream fis;    // FileInputStream
    //int numVertices;
    //int numFaces;
    /*  We dont use 'malloc'! like
        vertices = (Point3D*) malloc(numVertices*sizeof(Point3D));

        Docuentation says:

        Don't use 'malloc' any more.
        Use new Constructors for those reasons:
        Constructors/destructors: unlike malloc(sizeof(Fred)), new Fred() calls Fred's constructor. Similarly, delete p calls *p's destructor.
        Type safety: malloc() returns a void* which isn't type safe. new Fred() returns a pointer of the right type (a Fred*).
        Overridability: new is an operator that can be overridden by a class, while malloc() is not overridable on a per-class basis.
        @see: http://www.parashift.com/c++-faq/new-vs-malloc.html
    */
    string readToken;   // The token which is reading from the document
    fis.open(filename, ios::in);

    // Check if the file could have been opened. NOTIFY IF NOT!
    if (fis.good()) {
        cout<<"Open AES file ... Success!"<<endl;
    } else{
        cout << "COULD NOT OPEN AES OBJECT FILE... ";
        return;
    }

    while (!fis.eof()) {
        fis >> readToken;
        /*
        // These parts are not needed since we rely on the value amount not on the declaration.
        // This even gives a correct result if the number is wrong.
        if (readToken == "*MESH_NUMVERTEX") {
            numVertices = readNumVertices(fis);
            cout << "# Vertex: " << numVertices << endl;
        } else if (readToken == "*MESH_NUMFACES") {
            numFaces = readNumFaces(fis);
            cout << "# Faces: " << numFaces  << endl;
        } else
        */
        if (readToken == MESH_VERTEX_LIST) {
            parseVertices(fis, vertices);
        } else if (readToken == MESH_FACE_LIST) {
            parseFaces(fis, faces);
        } else if (readToken == MESH_NORMALS) {
            parseNormals(fis, normals);
        }
    }
    fis.close();

    // Set the given references of vertices and surfaces so the main program knows the object data
    verts = vertices;
    faces = faces;
    norms = normals;
}

/** Returns the amount of Vertices -> Not necessary since we rely on the document values not on the numbers.
That way even a wrong entry here is possible to show **/
int ASEParser::readNumVertices(fstream &stream) {
    int numVert;
    stream >> numVert;
    return numVert;
}

/** Returns the amount of Faces -> Not necessary since we rely on the document values not on the numbers.
That way even a wrong entry here is possible to show **/
int ASEParser::readNumFaces(fstream &stream) {
    int numFace;
    stream >> numFace;
    return numFace;
}

/** Parses the *MESH_VERTEX part into Vertex Objects:
    example: *MESH_VERTEX 9 19.56 -8.32 33.9 => Vertex(19.56, -8.32, 33.9) **/
void ASEParser::parseVertices(fstream &stream, std::vector<ASEParser::Vertex> &verts) {   // parses the Vertices and puts the values
    cout << "Parsing Vertices... ";
    string readToken;
    int counter = 0;
    while (readToken != END_TOKEN) {
        stream >> readToken;
        if (readToken == MESH_VERTEX) {
            float x,y,z;
            stream >> counter;
            stream >> x;
            stream >> y;
            stream >> z;
            Vertex vertex = { x, y, z};
            //cout << "# reading Vertex : " << counter << ": " << vertex.x << " " << vertex.y << " " << vertex.z << endl;
            verts.push_back(vertex);
        }
    }
    cout << "done!" << endl;
}

/** Parses the *MESH_FACENORMAL part into Normal objects
    example: MESH_FACENORMAL 1021 0.1808 0.0357 0.9829 => Normal(0.1808, 0.0357, 0.9829) **/
void ASEParser::parseNormals(fstream &stream, std::vector<ASEParser::Normal> &normals) {   // parses the Vertices and puts the values
    cout << "Parsing Normals...";
    // Initialize with "empty normals" so we may just assign.
    Normal emptyNormal = { 0, 0, 0};
    normals.assign(vertices.size(), emptyNormal);

    string readToken;
    int face = 0;
    while (readToken != END_TOKEN) { // Some files just end, others have }
        stream >> readToken;
        if (readToken == MESH_VERTEXNORMAL) {
            float x,y,z;
            stream >> face;
            stream >> x;
            stream >> y;
            stream >> z;
            Normal normal = { x, y, z};
            // std::cout<< "# reading Face Normal : " << face << ": " << normal.x << " " << normal.y << " " << normal.z << endl;
            normals[face] = normal;
        }
    }
    //cout << "normals size after filling: " << normals.size() << " " << normals.capacity() << endl;
    cout << "done!" << endl;
}

/** Parses the *MESH_FACE part to a vector of faces
    example: *MESH_FACE 8: A: 3 B: 2 C: 6 => Face(3,2,6) **/
void ASEParser::parseFaces(fstream &stream, std::vector<ASEParser::Face> &faces) {   // parses the Vertices and puts the values
    cout << "Parsing Faces...";
    string readToken;
    int faceNum = 0;
    while (readToken != END_TOKEN) { // Some files just end, others have }
        stream >> readToken;
        if (readToken == MESH_FACE) {
            int a,b,c;
            stream >> faceNum;
            stream >> readToken;
            stream >> readToken;
            stream >> a;
            stream >> readToken;
            stream >> b;
            stream >> readToken;
            stream >> c;
            Face face = {a,b,c};
            //cout << "# reading Face: " << faceNum << ": " << face.a << " " << face.b << " " << face.c << endl;
            faces.push_back(face);
        }
    }
    cout << "done!" << endl;
}

