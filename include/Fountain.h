#ifndef FOUNTAIN_H
#define FOUNTAIN_H

#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <math.h>

#include "ASEParser.h"     // Importer for ASE Files

#include "GameObject.h"

#include <GL/freeglut.h>


using namespace std;

class Fountain : public GameObject
{
    public:
        // Imported Object (3Ds - ASE import)
        //const string filename = "Data/objects/fuente.ase";
        const string filename = "Data/objects/objeto.ase";

        std::vector<ASEParser::Vertex> vertices;
        std::vector<ASEParser::Normal> normals;
        std::vector<ASEParser::Face> faces;

        // Display-Lists for faster rendering
        GLuint fountainDL;              // The Display-List for the imported object

        Fountain();
        virtual ~Fountain();

        void create(void) override;
        void draw(void) override;
    protected:
    private:
};

#endif // FOUNTAIN_H
