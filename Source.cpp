//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.          
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk. 
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat. 
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization függvényt kivéve, a lefoglalt adat korrekt felszabadítása nélkül 
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:  
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D, 
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi, 
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Boczan Tamas
// Neptun : A5X61F
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy 
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem. 
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a 
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb 
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem, 
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.  
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat 
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#if defined(__APPLE__)                                                                                                                                                                                                            
#include <OpenGL/gl.h>                                                                                                                                                                                                            
#include <OpenGL/glu.h>                                                                                                                                                                                                           
#include <GLUT/glut.h>                                                                                                                                                                                                            
#else                                                                                                                                                                                                                             
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)                                                                                                                                                                       
#include <windows.h>                                                                                                                                                                                                              
#endif                                                                                                                                                                                                                            
#include <GL/gl.h>                                                                                                                                                                                                                
#include <GL/glu.h>                                                                                                                                                                                                               
#include <GL/glut.h>                                                                                                                                                                                                              
#endif          


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

#define PI 3.14159265359f
#define NEAR_ZERO 0.001f

//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------
struct Vector {
    float x, y, z;

    Vector() {
        x = y = z = 0;
    }

    Vector(float x0, float y0, float z0 = 0) {
        x = x0;
        y = y0;
        z = z0;
    }

    Vector operator*(float a) const {
        return Vector(x * a, y * a, z * a);
    }

    Vector operator/(float a) const {
        return Vector(x / a, y / a, z / a);
    }

    Vector operator+(const Vector &v) const {
        return Vector(x + v.x, y + v.y, z + v.z);
    }

    Vector operator-(const Vector &v) const {
        return Vector(x - v.x, y - v.y, z - v.z);
    }

    float operator*(const Vector &v) const {    // dot product
        return (x * v.x + y * v.y + z * v.z);
    }

    Vector operator%(const Vector &v) const {    // cross product
        return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    float length() const {
        return sqrtf(x * x + y * y + z * z);
    }

    Vector normalized() const {
        return *this / length();
    }

    Vector operator*=(float a) {
        x *= a;
        y *= a;
        z *= a;
        return *this;
    }
};
 
//--------------------------------------------------------
// Spektrum illetve szin
//--------------------------------------------------------
struct Color {
   float r, g, b;

   Color( ) { 
	r = g = b = 0;
   }
   Color(float r0, float g0, float b0) { 
	r = r0; g = g0; b = b0;
   }
   Color operator*(float a) { 
	return Color(r * a, g * a, b * a); 
   }
   Color operator*(const Color& c) { 
	return Color(r * c.r, g * c.g, b * c.b); 
   }
   Color operator+(const Color& c) {
 	return Color(r + c.r, g + c.g, b + c.b); 
   }
};

const int screenWidth = 600;	// alkalmazás ablak felbontása
const int screenHeight = 600;
Color space[screenWidth*screenHeight];	// egy alkalmazás ablaknyi kép

struct ControlPoint {
    Vector originalP;
    Vector p;
    float t;
};

struct Material {
    Color diffuse, ambient, specular;
    float shine;

    Material(){};

    Material(Color const &diffuse, Color const &ambient, Color const &specular, float shine)
            : diffuse(diffuse), ambient(ambient), specular(specular), shine(shine) {
    }
};

const Material silver(Color(0.51, 0.51, 0.51), Color(0.19, 0.19, 0.19), Color(0.51, 0.51, 0.51), 51.2);
const Material water(Color(0.06, 0.06, 0.39), Color(0.06, 0.06, 0.39), Color(0.06*8.0, 0.06*8.0, 0.39*8.0), 80.0);
const Material sunColor(Color(0.93, 0.88, 0.14), Color(0.93, 0.88, 0.14), Color(0.93, 0.88, 0.14), 0.0);

const size_t maxControlPoints = 10;
ControlPoint cp[maxControlPoints];
size_t cpSize;

void glVertex(Vector const &v) {
    glVertex3f(v.x, v.y, v.z);
}

void glNormal(Vector const &v) {
    glNormal3f(v.x, v.y, v.z);
}

class Shape {
protected:
    static const size_t shapeResolution = 200;
    Vector shapePoints[shapeResolution + 1];
    size_t shapePointSize;
    Color color;

public:
    Shape() {
        shapePointSize = 0;
    }

    virtual void computeShape() {
    }

    virtual void drawShape() {
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < shapePointSize; i++)
            glVertex2f(shapePoints[i].x, shapePoints[i].y);
        glEnd();
    }
};

class CatmullRomSpline : public Shape {
    unsigned pointsBetweenControlPoints;
    Vector v[maxControlPoints];
    Vector startV;
    Vector endV;

    Vector a0(size_t prev) {
        return cp[prev].p;
    }

    Vector a1(size_t prev) {
        return v[prev];
    }

    Vector a2(size_t prev) {
        size_t i = prev;
        Vector p0 = cp[i].p;
        Vector p1 = cp[i + 1].p;
        float t0 = cp[i].t;
        float t1 = cp[i + 1].t;
        Vector tag1 = (p1 - p0) * 3
                / pow(t1 - t0, 2);
        Vector tag2 = (v[i + 1] + v[i] * 2)
                / (t1 - t0);

        return tag1 - tag2;
    }

    Vector a3(size_t prev) {
        size_t i = prev;
        Vector p0 = cp[i].p;
        Vector p1 = cp[i + 1].p;
        float t0 = cp[i].t;
        float t1 = cp[i + 1].t;
        Vector tag1 = (p0 - p1) * 2
                / pow(t1 - t0, 3);
        Vector tag2 = (v[i + 1] + v[i])
                / pow(t1 - t0, 2);

        return tag1 + tag2;
    }

public:
    CatmullRomSpline() : Shape() {
        startV = Vector(0.00001, 0.00001, 0.0);
        endV = Vector(0.00001, 0.00001, 0.0);
        pointsBetweenControlPoints = shapeResolution / maxControlPoints;
        color = Color(1.0f, 1.0f, 1.0f);;
    }

    void computeV() {
        v[0] = startV;
        v[cpSize - 1] = endV;
        for (size_t i = 1; i < cpSize - 1; i++) {
            Vector p0 = cp[i].p;
            Vector pp1 = cp[i + 1].p;
            Vector pm1 = cp[i - 1].p;
            float t0 = cp[i].t;
            float tp1 = cp[i + 1].t;
            float tm1 = cp[i - 1].t;

            Vector tag1 = (pp1 - p0) / (tp1 - t0);
            Vector tag2 = (p0 - pm1) / (t0 - tm1);
            v[i] = tag1 + tag2;
        }
    }

    Vector getPos(float t, size_t prevIndex) {
        size_t i = prevIndex;
        Vector ai0 = a0(i);
        Vector ai1 = a1(i);
        Vector ai2 = a2(i);
        Vector ai3 = a3(i);

        float t0 = cp[i].t;

        return ai3 * pow(t - t0, 3)
                + ai2 * pow(t - t0, 2)
                + ai1 * (t - t0)
                + ai0;
    }

    void computeShape() {
        unsigned points = pointsBetweenControlPoints;
        shapePointSize = 0;

        computeV();
        for (size_t i = 0; i < cpSize - 1; i++)
            for (size_t j = i * points; j < (i + 1) * points; j++) {
                float t = cp[i].t + (
                        ((cp[i + 1].t - cp[i].t) / (float) points) * (j - (i * (float) points))
                );
                shapePoints[j] = getPos(t, i);
                shapePointSize = j + 1;
            }
    }
};

class Ellipsoid {
    float a, b, c;
    Vector pos;
    Material material;

    Vector getFirstPartialDerivative(float u, float v) {
        return Vector(
                a * (-1.0f * sinf(u)) * cosf(v),
                b * (-1.0f * sinf(u)) * sinf(v),
                c * cosf(u)
        );
    }

    Vector getSecondPartialDerivative(float u, float v) {
        return Vector(
                a * cosf(u) * (-1.0f * sinf(v)),
                b * cosf(u) * cosf(v),
                0.0f
        );
    }

    Vector getSurfacePoint(float u, float v) {
        return Vector(
                a * cosf(u) * cosf(v),
                b * cosf(u) * sinf(v),
                c * sinf(u)
        );
    }

    Vector getNormal(float u, float v) {
        if (PI / (-2.0f) < u && u < PI / 2.0f )
            return (getSecondPartialDerivative(u, v) % getFirstPartialDerivative(u, v)).normalized();
        if (PI / (-2.0f) >= u)
            return Vector(0, 0, -1);
        return Vector(0, 0, 1);
    }

public:
    Ellipsoid(){}

    Ellipsoid(float a, float b, float c, Material const &material, Vector const &pos) : a(a), b(b), c(c), material(material), pos(pos) {
    }

    void draw() {
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);

        float ambient[]  = {material.ambient.r, material.ambient.g, material.ambient.b};
        float diffuse[]  = {material.diffuse.r, material.diffuse.g, material.diffuse.b};
        float specular[] = {material.specular.r, material.specular.g, material.specular.b};
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, material.shine);

        const unsigned resolution = 30;
        float du = PI / resolution;
        float dv = PI / resolution;
        glBegin(GL_QUADS);
        for (float u = PI / (-2.0f); u < PI / 2.0f; u += du) {
            for (float v = PI * (-1.0f); v < PI; v += dv) {
                glNormal(getNormal(u, v));
                glVertex(getSurfacePoint(u, v));
                glNormal(getNormal(u + du, v));
                glVertex(getSurfacePoint(u + du, v));
                glNormal(getNormal(u + du, v + dv));
                glVertex(getSurfacePoint(u + du, v + dv));
                glNormal(getNormal(u, v + dv));
                glVertex(getSurfacePoint(u, v + dv));
            }
        }
        glEnd();

        glPopMatrix();
    }
};

class Light0 {
    Vector pos;
    Material lightColor;

public:
    Light0(Vector const &pos, Material const &lightColor)
    : pos(pos), lightColor(lightColor) {
        // pozíció, utolsó koordináta 0, ha irányfényforrás
        GLfloat lightPos[] = {pos.x, pos.y, pos.z, 0};
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        GLfloat diffuse[] = {lightColor.diffuse.r, lightColor.diffuse.g, lightColor.diffuse.b, 1.0f};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        GLfloat ambient[] = {lightColor.ambient.r, lightColor.ambient.g, lightColor.ambient.b, 1.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        GLfloat specular[] = {lightColor.specular.r, lightColor.specular.g, lightColor.specular.b, 1.0f};
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
        float shine = lightColor.shine;
        glLightf(GL_LIGHT0, GL_SHININESS, lightColor.shine);
    }

    Light0() {
    }

    void enable() {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }

    void disable() {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }
};

void createSpace(){
    for(int Y = 0; Y < screenHeight; Y++)
        for(int X = 0; X < screenWidth; X++) {
            int current = Y * screenWidth + X;
            if ((current % 184) == (Y * X % 211))
                space[current] = Color(1.0f, 1.0f, 1.0f);
            else
                space[current] = Color(0.0f, 0.0f, 0.0f);
        }
}

void setPerspective() {
    // képernyő méret
    glViewport(0, 0, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // perspektíva és vágás
    glEnable(GL_DEPTH_TEST);
    float zNear = 0.1;
    float zFar = 10;
    gluPerspective(60, 1, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

void setCamera() {
    Vector eye(0.0f, 0.0f, -6.0f);
    Vector lookat(0.0f, 0.0f, 0.0f);
    Vector up(0, 1, 0);
    gluLookAt(eye.x, eye.y, eye.z, lookat.x, lookat.y, lookat.z, up.x, up.y, up.z);
}

Ellipsoid sun;
Ellipsoid earth;
Light0 light;

void build(){
    createSpace();
    Vector center = Vector(5.0f, 0.0f, 3.0f);
    earth = Ellipsoid(1.0f*5.0f, 0.85f*8.0f, 1.0f*5.0f, water, center);

    Material sunLight;
    sunLight.shine = 5;
    sunLight.diffuse = Color(1.0f, 1.0f, 1.0f);
    sunLight.ambient = Color(1.0f, 1.0f, 1.0f);
    sunLight.specular = Color(1.0f, 1.0f, 1.0f);

    Vector pos = Vector(-2.0f, 3.0f, 3.0f);
    sun = Ellipsoid(1.0f, 1.0f, 1.0f, sunColor, pos);
    light = Light0(Vector(2.0f, 3.0f, 3.0f), sunLight);
}

void enableThrowBack() {
    // hátsólap-eldobás
    glFrontFace(GL_CW); // Az normál irányából nézve CCW a körüljárási irány
    glCullFace(GL_BACK); // A hátsó oldalt akarjuk eldobni
    glEnable(GL_CULL_FACE); // És engedélyezzük a lapeldobást.
}
void drawSpace() {
    glDisable(GL_DEPTH_TEST);
    glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_FLOAT, space);
    glEnable(GL_DEPTH_TEST);
}

void debug() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
    build();
    setPerspective();
    setCamera();
    glShadeModel(GL_SMOOTH);
    light.enable();
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.0, 0.0, 0.0, 1.0);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles
    //debug();

    drawSpace();
    enableThrowBack();
    light.disable();

    glColor3f(sunColor.ambient.r, sunColor.ambient.g, sunColor.ambient.b);
    glEnable(GL_COLOR_MATERIAL);
    sun.draw();
    glDisable(GL_COLOR_MATERIAL);
    light.enable();
    earth.draw();

    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet

}

// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y) {

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)   // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
		glutPostRedisplay( ); 						 // Ilyenkor rajzold ujra a kepet
}

// Eger mozgast lekezelo fuggveny
void onMouseMotion(int x, int y)
{

}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
     long time = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido

}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv); 				// GLUT inicializalasa
    glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel 
    glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

    glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

    glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();

    onInitialization();					// Az altalad irt inicializalast lefuttatjuk

    glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
    glutMouseFunc(onMouse); 
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutMotionFunc(onMouseMotion);

    glutMainLoop();					// Esemenykezelo hurok
    
    return 0;
}

