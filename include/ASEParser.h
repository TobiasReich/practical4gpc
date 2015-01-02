#ifndef AESPARSER_H
#define AESPARSER_H

#include <vector>

class ASEParser{
    public:
        struct Vertex{
            float x;
            float y;
            float z;
        };

        struct Face{
            int a;
            int b;
            int c;
        };

        struct Normal{
            float x;
            float y;
            float z;
        };

        std::vector<Vertex> vertices;
        std::vector<Normal> normals;
        std::vector<Face> faces;

        /** Since we use per vertex normals the Surface3D struct is useless! Using faces & normals instead**/

        ASEParser(std::string file);
        virtual ~ASEParser();
        void readObject(std::vector<Vertex> &verts, std::vector<Normal> &norms, std::vector<Face> &faces);
    protected:
    private:
        std::string filename;                       // File to open

        // Not used but for general purpose (or idle curiosity)
        int readNumVertices(std::fstream &stream);  // Amount of Vertices of the object -> *MESH_NUMVERTEX
        int readNumFaces(std::fstream &stream);     // Amount of Surfaces of the object -> *MESH_NUMFACES

        void parseVertices(std::fstream &stream, std::vector<Vertex> &verts);
        void parseNormals(std::fstream &stream, std::vector<Normal> &normals);
        void parseFaces(std::fstream &stream, std::vector<Face> &faces);
        void createSurfaces();
};

#endif // ASEParser_H
