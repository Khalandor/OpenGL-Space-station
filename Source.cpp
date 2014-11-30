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

    Color() {
        r = g = b = 0;
    }

    Color(float r0, float g0, float b0) {
        r = r0;
        g = g0;
        b = b0;
    }

    Color operator*(float a) const {
        return Color(r * a, g * a, b * a);
    }

    Color operator/(float a) const {
        return Color(r / a, g / a, b / a);
    }

    Color operator*(const Color &c) const {
        return Color(r * c.r, g * c.g, b * c.b);
    }

    Color operator/(const Color &c) const {
        return Color(r / c.r, g / c.g, b / c.b);
    }

    Color operator+(const Color &c) const {
        return Color(r + c.r, g + c.g, b + c.b);
    }

    Color operator-(const Color &c) const {
        return Color(r - c.r, g - c.g, b - c.b);
    }

    Color operator+=(const Color &c) {
        r += c.r;
        g += c.g;
        b += c.b;
        return *this;
    }

    Color operator*=(const Color &c) {
        r *= c.r;
        g *= c.g;
        b *= c.b;
        return *this;
    }
};

const int screenWidth = 600;	// alkalmazás ablak felbontása
const int screenHeight = 600;
Color space[screenWidth*screenHeight];	// egy alkalmazás ablaknyi kép
GLuint  tex;

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

    Material operator* (float  a) const {
        Material m;
        m.ambient = ambient * a;
        m.diffuse = diffuse * a;
        m.shine = shine * a;
        m.specular = specular * a;
        return m;
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
    bool textured;

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

    Ellipsoid(float a, float b, float c, Material const &material, Vector const &pos, bool textured)
            : a(a), b(b), c(c), material(material), pos(pos), textured(textured) {
    }

    void draw() {
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);

        if (textured){
            glRotatef(90.0f, 1, 0, 0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tex);
        }

        float ambient[]  = {material.ambient.r, material.ambient.g, material.ambient.b};
        float diffuse[]  = {material.diffuse.r, material.diffuse.g, material.diffuse.b};
        float specular[] = {material.specular.r, material.specular.g, material.specular.b};
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, material.shine);

        const unsigned resolution = 40;
        float startU = PI / (-2.0f);
        float startV = PI * (-1.0f);
        float endU = PI / 2.0f;
        float endV = PI;
        float du = (endU - startU) / resolution;
        float dv = (endV - startV) / resolution;
        glBegin(GL_QUADS);
        for (float u = startU; u < endU; u += du) {
            for (float v = startV; v < endV; v += dv) {
                float uX = (u - startU) / (endU - startU);
                float uX1 = (u - startU + du) / (endU - startU);
                float vX = (v - startV) / (endV - startV);
                float vX1 = (v - startV + dv) / (endV - startV);

                if (textured)
                    glTexCoord2f(uX, vX);
                glNormal(getNormal(u, v));
                glVertex(getSurfacePoint(u, v));

                if (textured)
                    glTexCoord2f(uX1, vX);
                glNormal(getNormal(u + du, v));
                glVertex(getSurfacePoint(u + du, v));

                if (textured)
                    glTexCoord2f(uX1, vX1);
                glNormal(getNormal(u + du, v + dv));
                glVertex(getSurfacePoint(u + du, v + dv));

                if (textured)
                    glTexCoord2f(uX, vX1);
                glNormal(getNormal(u, v + dv));
                glVertex(getSurfacePoint(u, v + dv));
            }
        }
        glEnd();

        if (textured) {
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_TEXTURE_2D);
        }
        glPopMatrix();
    }
};

class Cone {
    float r, height;
    Vector center, rotate;
    Material material;

public:
    Cone() {
    }

    Cone(float r, float height, Vector const &center, Vector const &rotate, Material const &material)
            : r(r), height(height), center(center), rotate(rotate), material(material) {
    }

    void draw() {
        glPushMatrix();
        glTranslatef(center.x, center.y, center.z);
        glRotatef(rotate.x, 1, 0, 0);
        glRotatef(rotate.y, 0, 1, 0);
        glRotatef(rotate.z, 0, 0, 1);
        glScalef(r, height, r);

        float ambient[]  = {material.ambient.r, material.ambient.g, material.ambient.b};
        float diffuse[]  = {material.diffuse.r, material.diffuse.g, material.diffuse.b};
        float specular[] = {material.specular.r, material.specular.g, material.specular.b};
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, material.shine);


        int triangles = 30;
        float delta = 2 * PI / triangles;
        glBegin(GL_TRIANGLE_FAN);
        Vector top = Vector(0, 1, 0);
        glNormal(top);
        glVertex(top);
        for (int i = 0; i <= triangles; i++) {
            Vector pointOnCircle = Vector(
                    (r * (float) cos(i * delta)),
                    0.0f,
                    0.0f + (r * (float) sin(i * delta))
            );
            glNormal(pointOnCircle);
            glVertex(pointOnCircle);
        }

        /*
        const unsigned resolution = 40;
        float startU = 0;
        float startV = 0;
        float endU = PI * 2.0f;
        float endV = height;
        float du = (endU - startU) / resolution;
        float dv = (endV - startV) / resolution;
        glBegin(GL_TRIANGLES);
        for (float u = startU; u < endU; u += du) {
            for (float v = startV; v < endV; v += dv) {
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
        */
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

float noise(int x, int y) {
    int n = x + y * 57;
    n = (n << 13) ^ n;
    int rand1 = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
    return (1.0f - ((float)rand1/ 1073741824.0f));
}

float smoothedNoise(float x, float y) {
    float corners = (noise(x - 1, y - 1) + noise(x + 1, y - 1) + noise(x - 1, y + 1) + noise(x + 1, y + 1)) / 16;
    float sides = (noise(x - 1, y) + noise(x + 1, y) + noise(x, y - 1) + noise(x, y + 1)) / 8;
    float center = noise(x, y) / 4;
    return corners + sides + center;
}

float interpolate(float a, float b, float x) {
    float ft = x * PI;
    float f = (1.0f - cosf(ft)) * 0.5f;
    return a * (1 - f) + b * f;
}

float interpolatedNoise(float x, float y) {
    int integer_X = int(x);
    float fractional_X = x - integer_X;

    int integer_Y = int(y);
    float fractional_Y = y - integer_Y;

    float v1 = smoothedNoise(integer_X, integer_Y);
    float v2 = smoothedNoise(integer_X + 1, integer_Y);
    float v3 = smoothedNoise(integer_X, integer_Y + 1);
    float v4 = smoothedNoise(integer_X + 1, integer_Y + 1);

    float i1 = interpolate(v1, v2, fractional_X);
    float i2 = interpolate(v3, v4, fractional_X);

    return interpolate(i1, i2, fractional_Y);
}

// Perlin noise elméleti hátterének és pszeudo-kódjának forrása: http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
float perlin(float x, float y, int octaves, float persistance) {
    float total = 0;
    float p = persistance;
    int n = octaves - 1;

    for (int i = 0; i < n; i++) {
        float frequency = 2 * i;
        float amplitude = p * i;
        total = total + interpolatedNoise(x * frequency, y * frequency) * amplitude;
    }
    return total;
}

void createTexture() {
    glGenTextures(1, &tex);

    int textureWidth = 32;
    int textureHeight = 32;
    GLubyte texture_data[textureWidth * textureHeight][3];
    for(int Y = 0; Y < textureHeight; Y++)
        for(int X = 0; X < textureWidth; X++) {
            float noise = perlin(X, Y, 4, 1.0f / powf(2, 5)) + 0.5f;
            texture_data[Y * textureWidth + X][0] = (GLubyte) (255 - (unsigned)(noise * 128 * 1.5));
            texture_data[Y * textureWidth + X][1] = (GLubyte) (255 - (unsigned)(noise * 10 * 2));
            texture_data[Y * textureWidth + X][2] = (GLubyte) (255 - (unsigned)(noise * 255 * 2));
        }

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data[0]);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void createTextureContinents() {
    glGenTextures(1, &tex);
    // http://www.text-image.com
    const char* ascii_textures = {
                    "                                                                                                                                                                                                                                                                "
                    "                                                                                                                                                                                                                                                                "
                    "                                                                                                                                                                                                                                                                "
                    "                                                                                                                                                                                                                                                                "
                    "                                                              ``  ``````````````````````..........`                                                                                                                                                             "
                    "                                                                 `.``   ```......................`                                                              `                                                                                               "
                    "                                         ```               ``````            ..................`                                                          `````````````                                                                                         "
                    "                                       ````...```   ```   ```..```            ..............``                                                   ``   `````````````````````````````     ``````                                                                  "
                    "            ````..........``````````````  ```````  ` `.`  ``    `...`        ``.........`````                       `..------..```      ````````````` ````````````````````````````````````````````````````````````                                              "
                    "        ````...............................`......`...`````    ```.```      `......```        ````                `.----.-----.`````.-.-------.```````````````````````````````````````````````````````````````````````` ``                                      "
                    "       ```........................................```  ``      `.``         .....`            `.``             ``.---.. `------...------------.````````````````````````````````````````````````````````````````````````                                         "
                    "     `....````````.............................``         `..``              ```                              -------`  .....-----------------.`````````````````````````````````````````````````````````````  ` ``````                                          "
                    "    `````          ``.........................`          .....```.                                     `      `.``---`  ``-.------------------.``````````````````````````````````````````````````````           ``                                              "
                    "                    `..........................```      `.........`                                  ` .`       . `.`   ----------------------.`````````````````````````````````````````````````````            ````                                            "
                    "                    `..............................` `............```                              `.. `-.`   ``-......-----------------------.````````````````````````````````````````````````````````          ```                                            "
                    "                    `..............................``........```````                                `  `..``.---------------------------------.``````````````````````````````````````````````````````````          `                                            "
                    "                   ```.....................````...........```     ```                                  ``..-----------------------------------.```````````````````````````````````````````````````````````                                                      "
                    "                   `......................``` ```..........``                                           `----------------------`.-.`.------.``````````````````````````````````````````````````````````````                                                      "
                    "                 ``........................```` ```....``` `                                       ``````---...``-.``.--------   `  `..----`  ````````````````````````````````````````````````````````````    ``                                                "
                    "                `..........................`..``....```                                            .------.`    ``..```.---.-.   ``    .....`   ```````````````````````````````````````````````````````       ``                                                "
                    "               `..................................``                                               ------`     ``   `.  ..`  ````````````````   ````````````````````````````````````````````````  ` ```        `                                                "
                    "                .................................                                                  `....`  ``````  `     ``   `````````````````  `````````````````````````````````````````````````    ```      ``                                               "
                    "                `..............................``                                                   `.``.........`         `        ``````````````````````````````````````````````````````````````     `    ````                                                "
                    "                  ..........................``                                                    `................``   `.``       ````````````````````````````````````````````````````````````````                                                             "
                    "                  ` `............`````````.`                                                     `.....................`..........```````````````````````````````````````````````````````````````````                                                           "
                    "                  `` `.........`          `.                                                   `..................................` ``````````  `````````````````````````````````````````````````````                                                           "
                    "                   `  `.......`            `                                                  .....................................` ```````````   ` ```````````````````````````````````````````````                                                            "
                    "                       `......            ``                                                `.......................................`  ```````````````        `````````````````````````````````````  `                                                          "
                    "                        ......     ```      ```                                             .........................................` ```````````````         ` ```````````    ```````````` `                                                                  "
                    "                        ``.....````..            ``                                         `.........................................`  ````````````            `````````        `````````  `                                                                  "
                    "                            `````...```                                                     ...........................................`  ````````                ```````          ``````````         ``                                                        "
                    "                                  ```..`                                                    .............................................` ````                    ````              `````````         `                                                        "
                    "                                     `.`       `` `                                         `..............................................    ``                   ```               `  `````                                                                  "
                    "                                      ````   `...`.....```                                    `.................................................`                    ``               `    `         `                                                          "
                    "                                            ..............`                                    `...............................................`                        `              `                  ``                                                    "
                    "                                            `..................`                                 ```````     ```..............................`                                     ``  ``        ````                                                          "
                    "                                           `.....................                                               `...........................``                                        `` `      ``````                                                          "
                    "                                          ......................``                                              ..........................`                                            ```     ``````  `                                                        "
                    "                                         `.......................``..````                                       `........................`                                              ```    `````  ```        `  ```                                         "
                    "                                         `................................``                                      .....................`                                                  ``          `            ```..```                                     "
                    "                                          `..................................`                                     ....................`                                                    ``  `                    `...````                                   "
                    "                                           `................................`                                      `....................                                                                               ``   ``                                  "
                    "                                            `..............................`                                       `....................`                                                                                                                       "
                    "                                             `............................`                                       `.....................`     `.                                                              ``..`    ``                                       "
                    "                                               ``.........................`                                       ......................   ``..`                                                         ``.``....``   `..                                      "
                    "                                                  `.......................`                                       `..................`     ....                                                        ``...........```...                                      "
                    "                                                   ......................`                                         `................       ...`                                                    ````...................``                                    "
                    "                                                   ..................````                                           ................`     `...                                                  ``..........................`                                   "
                    "                                                   `................`                                               `.............``       ``                                                   `............................`                                  "
                    "                                                   `................                                                 .............                                                              .............................`                                  "
                    "                                                   `...............`                                                  ..........`                                                               `...........................`                                   "
                    "                                                   `..............`                                                    .......`                                                                 `...``````` ``.`..........``                                    "
                    "                                                    ..........```                                                      ``````                                                                  ```            ` `.......``                                      "
                    "                                                    ...........`                                                                                                                                                `....```                 `                      "
                    "                                                    ........``                                                                                                                                                                          ```                     "
                    "                                                     `....``                                                                                                                                                      ``                 `                          "
                    "                                                      .....`                                                                                                                                                                     ````                           "
                    "                                                      ....`                                                                                                                                                                    ```                              "
                    "                                                       ....`                                                                                                                                                                                                    "
                    "                                                        `..                                                                                                                                                                                                     "
                    "                                                           ```                                                                                                                                                                                                  "
                    "                                                                                                                                                                                                                                                                "
                    "                                                                                                                                                                                                                                                                "
                    "                                                                                                                                                                                                                                                                "
    };

    int textureWidth = 256;
    int textureHeight = 64;
    GLubyte texture_data[textureWidth * textureHeight][3];
    for(int i = 0; i < textureWidth * textureHeight; i++) {
        switch(ascii_textures[i]) {
            case ' ':
                for(int j = 0; j < 3; j++) {
                    texture_data[i][j] = 255;
                }
                break;
            default:
                texture_data[i][0] = 128;
                texture_data[i][1] = 255;
                texture_data[i][2] = 0;
                break;
        }
    }

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data[0]);
    glBindTexture(GL_TEXTURE_2D, 0);
}



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


class Satellite {
    Vector pos;
    float size;

    Ellipsoid satelliteBody;
    Cone jet1, jet2, jet3, jet4, jet5, jet6;

public:
    Satellite() {
    }


    Satellite(Vector const &pos, float size) : pos(pos), size(size) {
        satelliteBody = Ellipsoid(size, size, size, silver, pos, false);
        jet1 = Cone(size/1.5f, size, pos + Vector(-size * 2.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, -90.0f), silver);
        jet2 = Cone(size/1.5f, size, pos + Vector(size * 2.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 90.0f), silver);
        jet3 = Cone(size/1.5f, size, pos + Vector(0.0f, 0.0f, -size * 2.0f), Vector(90.0f, 0.0f, 0.0f), silver);
        jet4 = Cone(size/1.5f, size, pos + Vector(0.0f, 0.0f, size * 2.0f), Vector(-90.0f, 0.0f, 0.0f), silver);
        jet5 = Cone(size/1.5f, size, pos + Vector(0.0f, -size * 2.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), silver);
        jet6 = Cone(size/1.5f, size, pos + Vector(0.0f, size * 2.0f, 0.0f), Vector(180.0f, 0.0f, 0.0f), silver);
    }

    void draw(){
        glPushMatrix();
        satelliteBody.draw();
        jet1.draw();
        jet2.draw();
        jet3.draw();
        jet4.draw();
        jet5.draw();
        jet6.draw();
        glPopMatrix();
    }
} satellite;

void build(){
    Vector earthCenter = Vector(4.0f, 0.0f, 8.0f);
    Vector sunCenter = Vector(-2.0f, 3.0f, 3.0f);

    createSpace();

    //earth = Ellipsoid(1.0f*5.0f, 0.85f*8.0f, 1.0f*5.0f, water, earthCenter, true);
    earth = Ellipsoid(5.5f, 5.0f, 5.0f, water, earthCenter, true);
    satellite = Satellite(Vector(-1.5, -0.3, 1), 0.6f);

    Material sunLight;
    sunLight.shine = 5;
    sunLight.diffuse = Color(1.0f, 1.0f, 1.0f);
    sunLight.ambient = Color(1.0f, 1.0f, 1.0f);
    sunLight.specular = Color(1.0f, 1.0f, 1.0f);

    sun = Ellipsoid(1.0f, 1.0f, 1.0f, sunColor, sunCenter, false);
    light = Light0(Vector(2.0f, 3.0f, 3.0f), sunLight * 2.0f);

}

void enableThrowBack() {
    // hátsólap-eldobás
    glFrontFace(GL_CW); // Az normál irányából nézve CCW a körüljárási irány
    glCullFace(GL_BACK); // A hátsó oldalt akarjuk eldobni
    glEnable(GL_CULL_FACE); // És engedélyezzük a lapeldobást.
}

void disableThrowBack() {
    glDisable(GL_CULL_FACE);
}

void drawSpace() {
    glDisable(GL_DEPTH_TEST);
    glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_FLOAT, space);
    glEnable(GL_DEPTH_TEST);
}

void debug() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void drawCircle(Vector center, float radius) {
    int triangles = 30;
    float delta = 2 * PI / triangles;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(center.x, center.y);
    for (int i = 0; i <= triangles; i++)
        glVertex2f(
                center.x + (radius * (float) cos(i * delta)),
                center.y + (radius * (float) sin(i * delta))
        );
    glEnd();
}

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
    createTexture();
    build();
    setPerspective();
    setCamera();
    glShadeModel(GL_SMOOTH);
    light.enable();
}

/*
void glQuad(const Vector& a, const Vector& b, const Vector& c, const Vector& d) {
    Vector normal = ((b-a) % (c-a)).normalized();
    glNormal3f(normal.x, normal.y, normal.z);

    int res = 8; // resolution
    for(int i = 0; i < res; i++) {
        for(int j = 0; j < res; j++) {
            glTexCoord2f(float(i)/res, float(j)/res);
            glVertex(a + (b-a)*i/res + (d-a)*j/res);

            glTexCoord2f(float(i+1)/res, float(j)/res);
            glVertex(a + (b-a)*(i+1)/res + (d-a)*j/res);

            glTexCoord2f(float(i+1)/res, float(j+1)/res);
            glVertex(a + (b-a)*(i+1)/res + (d-a)*(j+1)/res);

            glTexCoord2f(float(i)/res, float(j+1)/res);
            glVertex(a + (b-a)*i/res + (d-a)*(j+1)/res);
        }
    }
}

void drawCube(const Vector& size) {
    Vector s = size / 2;

    Vector A(+s.x, +s.y, -s.z), B(+s.x, +s.y, +s.z), C(+s.x, -s.y, +s.z), D(+s.x, -s.y, -s.z),
            E(-s.x, +s.y, -s.z), F(-s.x, +s.y, +s.z), G(-s.x, -s.y, +s.z), H(-s.x, -s.y, -s.z);

    Vector vertices[6][4] = {
            {A, B, C, D}, {G, F, E, H}, {A, E, F, B},
            {D, C, G, H}, {B, F, G, C}, {E, A, D, H}
    };
    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);

    for(int i = 0; i < 6; i++) {
        glBindTexture(GL_TEXTURE_2D, tex); // Ezt semmiképpen se rakd a glBegin - glEnd blokk közé

        glBegin(GL_QUADS); {
            glQuad(vertices[i][0], vertices[i][1], vertices[i][2], vertices[i][3]);
        } glEnd();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}
*/

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.0, 0.0, 0.0, 1.0);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles
    //debug();

    drawSpace();
    enableThrowBack();
    light.disable();

    glColor3f(sunColor.ambient.r, sunColor.ambient.g, sunColor.ambient.b);
    //drawCircle(Vector(0.0f, 0.0f, 0.0f), 0.5);
    sun.draw();
    light.enable();

    earth.draw();

    disableThrowBack();
    satellite.draw();

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

