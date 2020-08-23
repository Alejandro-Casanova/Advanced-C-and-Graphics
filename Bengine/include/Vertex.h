#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <GLEW/glew.h>

namespace Bengine{

    struct Position {
        Position() : x(0), y(0) {}
        float x;
        float y;
    };

    struct ColorRGBA8{

        ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
            r(R), g(G), b(B), a(A){}

        ColorRGBA8() : r(0), g(0), b(0), a(0){}

        GLubyte r;
        GLubyte g;
        GLubyte b;
        GLubyte a;
    };

    struct UV{
        UV() : u(1.0f), v(1.0f) {}
        float u;
        float v;
    };

    struct Vertex{

        Position position;
        ColorRGBA8 color;
        //UV texture coordinates
        UV uv;

        void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a){
            color.r = r;
            color.g = g;
            color.b = b;
            color.a = a;
        }

        void setUV(float u, float v){
            uv.u = u;
            uv.v = v;
        }

        void setPosition(float x, float y){
            position.x = x;
            position.y = y;
        }
    };

}

#endif // VERTEX_H_INCLUDED
