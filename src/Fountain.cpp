#include "Fountain.h"
#include "World.h"

Fountain::Fountain()
{}

Fountain::~Fountain()
{}

/** Creates the Display-List for the tower (so we can reuse it several times)**/
void Fountain::create(void){
    // Create Parser Object and parse the "filename"
    ASEParser parser(filename);
    parser.readObject(vertices, normals, faces);

    fountainDL = glGenLists(1);
    glNewList(fountainDL, GL_COMPILE);

        glDisable(GL_BLEND);

        // These objects appear far to bright. This reduces the diffuse color greatly
        glColorMaterial(GL_FRONT, GL_DIFFUSE);
        glColor3f(0.02f, 0.02, 0.02f);

        glPushMatrix();
            glScalef(0.002, 0.002, 0.002);    // These objetcs are huge!

            // Rotate since axis are different
            // X / Y like screen cordinates for better understanding
            // since 2D is always X/Y, Z is depth
            // This leads to: X = left-right, Y = up-down, Z = back-forth
            glRotatef(-90, 1, 0, 0);

            // For every surfaces we have -> Draw it with corresponding normal
            // std::size_t instead of int in order to not compare (un)signed integers.
            // Used since size() is expressed in std::size_t (like sizeof...)
            for (std::size_t i=0; i<faces.size(); ++i){
                    ASEParser::Face face = faces[i];

                    // Vertex points
                    ASEParser::Vertex vA = vertices[face.a];
                    ASEParser::Vertex vB = vertices[face.b];
                    ASEParser::Vertex vC = vertices[face.c];

                    ASEParser::Normal nA = normals[face.a];
                    ASEParser::Normal nB = normals[face.b];
                    ASEParser::Normal nC = normals[face.c];

                    // Draw the object
                    // Using per Vertex Normals for smoother interpolation
                glBegin(GL_TRIANGLES);
                    glNormal3f(nA.x, nA.y, nA.z); glVertex3i(vA.x, vA.y, vA.z);
                    glNormal3f(nB.x, nB.y, nB.z); glVertex3i(vB.x, vB.y, vB.z);
                    glNormal3f(nC.x, nC.y, nC.z); glVertex3i(vC.x, vC.y, vC.z);
                glEnd();
            }
        glPopMatrix();

        // Reset the original diffuse material value
        World::resetGLColor();
    glEndList();
}

void Fountain::draw() {
     glCallList(fountainDL);
}
