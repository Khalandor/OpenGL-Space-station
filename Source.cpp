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
        if (length() > 0)
            return *this / length();
        else return *this;
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

    void asArray(float array[]) const {
        array[0] = r;
        array[1] = g;
        array[2] = b;
    }
};

const int screenWidth = 600;    // alkalmazás ablak felbontása
const int screenHeight = 600;

class Material {
    Color diffuse, ambient, specular;
    float ambientArr[3];
    float diffuseArr[3];
    float specularArr[3];
    float shine;

public:
    Material() {
    };

    Material(Color const &diffuse, Color const &ambient, Color const &specular, float shine)
            : diffuse(diffuse), ambient(ambient), specular(specular), shine(shine) {
        ambient.asArray(ambientArr);
        diffuse.asArray(diffuseArr);
        specular.asArray(specularArr);
    }

    Material operator*(float a) const {
        Material m = Material(diffuse * a, ambient * a, specular * a, shine * a);
        return m;
    }

    void setOGL() {
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArr);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArr);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specularArr);
        glMaterialf(GL_FRONT, GL_SHININESS, shine);
    }

    Color const &getDiffuse() const {
        return diffuse;
    }

    Color const &getAmbient() const {
        return ambient;
    }

    Color const &getSpecular() const {
        return specular;
    }

    float getShine() const {
        return shine;
    }
};

// diffuse, ambient, specular, shine
const Material chrome = Material(Color(0.4, 0.4, 0.4), Color(0.25, 0.25, 0.25) * 0.4, Color(0.77, 0.77, 0.77), 0.6);
const Material solarPanelMaterial = Material(Color(0.01, 0.01, 0.01), Color(0.01, 0.01, 0.01), Color(0.1, 0.1, 0.1), 0.8);
const Material planet = Material(Color(0.06, 0.06, 0.39) * 7, Color(0, 0, 0), Color(0, 0, 0), 0.0);
const Material sunColor = Material(Color(0.93, 0.88, 0.14), Color(0.93, 0.88, 0.14), Color(0.93, 0.88, 0.14), 0.0) * 2.5f;
const Material sunLight = Material(Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f), 1.0) * 0.5f;
Color atmosphereColor = Color(0.62, 0.85, 0.93 * 1.2) * 0.8;
const Material atmosphereMat = Material(atmosphereColor * 0.5, Color(0, 0, 0), atmosphereColor, 10.0);

class Texture {
protected:
    GLuint tex;
    const static int textureWidth = 1024;
    const static int textureHeight = 1024;
    GLubyte texture_data[textureWidth * textureHeight][3];

public:
    Texture() {
    }

    void setOGL() {
        glGenTextures(1, &tex);

        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data[0]);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint getTexture() const {
        return tex;
    }
};

// A Perlin-zaj elméleti hátterének és pszeudo-kódjának forrása: http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
class PlanetTexture : public Texture {
    const static int continentDistribution = 9;
    const static int complexity = 6;
    const static int hardness = 20;

    float noise(int x, int y) const {
        int n = x + y * 57;
        n = (n << 13) ^ n;
        int rand1 = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
        return (1.0f - ((float) rand1 / 1073741824.0f));
    }

    float smoothedNoise(int x, int y) const {
        float corners = (noise(x - 1, y - 1) + noise(x + 1, y - 1) + noise(x - 1, y + 1) + noise(x + 1, y + 1)) / 16;
        float sides = (noise(x - 1, y) + noise(x + 1, y) + noise(x, y - 1) + noise(x, y + 1)) / 8;
        float center = noise(x, y) / 4;
        return corners + sides + center;
    }

    float interpolate(float a, float b, float x) const {
        float ft = x * PI;
        float f = (1.0f - cosf(ft)) * 0.5f;
        return a * (1 - f) + b * f;
    }

    float interpolatedNoise(float x, float y) const {
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

    float perlin(float x, float y) const {
        float total = 0;
        int n = complexity;

        for (int i = 0; i < n; i++) {
            float frequency = 2 * i;
            float amplitude = i;
            total = total + interpolatedNoise(x * frequency, y * frequency) * amplitude;
        }
        return total;
    }

    void generateNoise(float noiseArr[], float &minNoise, float &maxNoise, int textureWidth, int textureHeight) const {
        float noiseMapSize = continentDistribution;
        float xDelta = noiseMapSize / textureWidth;
        float yDelta = noiseMapSize / textureHeight;
        minNoise = 1000.0f;
        maxNoise = -1000.0f;
        for (float Y = 0.0f; Y < noiseMapSize; Y += yDelta)
            for (float X = 0.0f; X < noiseMapSize; X += xDelta) {
                float noise = perlin(X, Y);
                if (noise < minNoise)
                    minNoise = noise;
                if (noise > maxNoise)
                    maxNoise = noise;
                int xIndex = (int) (X / xDelta);
                int yIndex = (int) (Y / yDelta);
                noiseArr[yIndex * textureWidth + xIndex] = noise;
            }
    }

    // [0, 1] tartományba skáláz
    float scale(float value, float currentMin, float currentMax) const {
        return (value - currentMin) / (currentMax - currentMin);
    }

    // a kapott [0, 1] tartományba eső értéket eltolja a skála egyik vége felé
    float hardenEdges(float value) const {
        //kis noise = zöld, nagy noise = kék
        float hardnessMultiplier = 1 + ((float) hardness / 10.0f);
        if (value > 0.5) {
            if (value * hardnessMultiplier < 1.0f)
                return value * hardnessMultiplier;
            else
                return 1.0f;
        }
        return value / hardnessMultiplier;
    }

public:
    void generate() {
        float noiseArr[textureWidth * textureHeight];
        float minNoise, maxNoise;
        generateNoise(noiseArr, minNoise, maxNoise, textureWidth, textureHeight);

        for (int i = 0; i < textureWidth * textureHeight; i++) {
            float noise = scale(noiseArr[i], minNoise, maxNoise);
            noise = hardenEdges(noise);

            // skálázás [0, 255] tartományba
            noise *= 255;

            // ennyi/255 -el szorozza a bolygó színét
            texture_data[i][0] = (GLubyte) (noise);
            texture_data[i][1] = (GLubyte) (255);
            texture_data[i][2] = (GLubyte) (noise);
        }
    }

    PlanetTexture() {
    }
};

void glVertex(Vector const &v) {
    glVertex3f(v.x, v.y, v.z);
}

void glNormal(Vector const &v) {
    glNormal3f(v.x, v.y, v.z);
}

void glTexCoord(Vector const &v) {
    glTexCoord2f(v.x, v.y);
}

void enableThrowBackCW() {
    // hátsólap-eldobás
    glFrontFace(GL_CW); // Az normál irányából nézve CCW a körüljárási irány
    glCullFace(GL_BACK); // A hátsó oldalt akarjuk eldobni
    glEnable(GL_CULL_FACE); // És engedélyezzük a lapeldobást.
}

void enableThrowBackCCW() {
    // hátsólap-eldobás
    glFrontFace(GL_CCW); // Az normál irányából nézve CCW a körüljárási irány
    glCullFace(GL_BACK); // A hátsó oldalt akarjuk eldobni
    glEnable(GL_CULL_FACE); // És engedélyezzük a lapeldobást.
}

void disableThrowBack() {
    glDisable(GL_CULL_FACE);
}

class Object {
protected:
    Material material;
    Texture *texture;

public :
    virtual void draw() = 0;


    Object() {
    }

    Object(Material const &material) : material(material) {
    }

    void setTexture(Texture *texture) {
        Object::texture = texture;
    }
};

struct ControlPoint {
    Vector p;
    float t;
};

class Shape {
protected:
    const static size_t maxControlPoints = 10;
    ControlPoint cp[maxControlPoints];
    size_t cpSize;

public:
    Shape() {
        cpSize = 0;
    }

    void addControlPoint(Vector p, float t) {
        if (cpSize < maxControlPoints) {
            cp[cpSize].p = p;
            cp[cpSize].t = t;
            cpSize++;
        }
    }

    ControlPoint getCp(size_t i) const {
        return cp[i];
    }

    ControlPoint getLastCp() const {
        return cp[cpSize - 1];
    }
};

class CatmullRomSpline : public Shape {
    Vector v[maxControlPoints];
    Vector startV;
    Vector endV;

    Vector a0(size_t prev) const {
        return cp[prev].p;
    }

    Vector a1(size_t prev) const {
        return v[prev];
    }

    Vector a2(size_t prev) const {
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

    Vector a3(size_t prev) const {
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

    Vector getPos(float t, size_t prevIndex) const {
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
};

class RotatedSpline : public Object {
    Material darkMaterial;

    int vertexNr;
    Vector *vertexes;
    Vector *normals;

    float circleDelta;
    float splineDelta;
    float firstT;
    float lastT;

    Vector holeMiddle;
    float holeRadius;

    Vector getSurfacePoint(Vector const &splinePoint, float angle) const {
        return Vector(splinePoint.x * sinf(angle), splinePoint.y, splinePoint.x * cosf(angle));
    }

    Vector getNormal(Vector const &currentP, float t, float angle, Vector const &currentSplineP, CatmullRomSpline const &spline) const {
        // currentP-hez képest a körön előző pont
        Vector prevSurfacePointCircle = getSurfacePoint(currentSplineP, angle - circleDelta);

        // currentP-hez képest a körön következő pont
        Vector nextSurfacePointCircle = getSurfacePoint(currentSplineP, angle + circleDelta);

        // currentP -hez képest a spline-on előző pont
        float prevT = t - splineDelta;
        if (prevT < firstT)
            prevT = firstT;
        size_t prevCp = 0;
        while (spline.getCp(prevCp + 1).t < prevT)
            prevCp++;
        Vector prevSplineP = spline.getPos(prevT, prevCp);
        Vector prevSurfacePointSpline = getSurfacePoint(prevSplineP, angle);

        // currentP -hez képest a spline-on következő pont
        float nextT = t + splineDelta;
        if (nextT > lastT)
            nextT = lastT;
        prevCp = 0;
        while (spline.getCp(prevCp + 1).t < nextT)
            prevCp++;
        Vector nextSplineP = spline.getPos(nextT, prevCp);
        Vector nextSurfacePointSpline = getSurfacePoint(nextSplineP, angle);

        Vector comp1 = ((nextSurfacePointCircle - currentP) % (nextSurfacePointSpline - currentP)).normalized();
        Vector comp2 = ((nextSurfacePointSpline - currentP) % (prevSurfacePointCircle - currentP)).normalized();
        Vector comp3 = ((prevSurfacePointCircle - currentP) % (prevSurfacePointSpline - currentP)).normalized();
        Vector comp4 = ((prevSurfacePointSpline - currentP) % (nextSurfacePointCircle - currentP)).normalized();

        return ((comp1 + comp2 + comp3 + comp4) / 4.0f).normalized();
    }

    bool isInsideHole(Vector const &p1) const {
        return ((p1 - holeMiddle).length() < holeRadius);
    }

    bool areInsideHole(Vector const &p1, Vector const &p2, Vector const &p3, Vector const &p4) const {
        return (isInsideHole(p1) && isInsideHole(p2) && isInsideHole(p3) && isInsideHole(p4));
    }

public:
    RotatedSpline() {
        vertexes = NULL;
        normals = NULL;
    }

    RotatedSpline(Material const &material)
            : Object(material) {
        vertexes = NULL;
        normals = NULL;
        darkMaterial = material * 0.01f;
    }

    void draw() {
        material.setOGL();

        glBegin(GL_QUAD_STRIP);
        for (size_t i = 0; i < vertexNr; i += 4) {
            if (areInsideHole(vertexes[i], vertexes[i + 1], vertexes[i + 2], vertexes[i + 3]))
                darkMaterial.setOGL();
            else
                material.setOGL();

            glNormal(normals[i]);
            glVertex(vertexes[i]);
            glNormal(normals[i + 1]);
            glVertex(vertexes[i + 1]);
            glNormal(normals[i + 2]);
            glVertex(vertexes[i + 2]);
            glNormal(normals[i + 3]);
            glVertex(vertexes[i + 3]);
        }
        glEnd();

    }

    void generate(int splineRes, int circleRes) {
        vertexNr = (splineRes) * (circleRes + 1) * 2;
        vertexes = new Vector[vertexNr];
        normals = new Vector[vertexNr];

        CatmullRomSpline spline;
        spline.addControlPoint(Vector(0.426, -1.485f), 0.863);
        spline.addControlPoint(Vector(0.500, -0.995f), 1.367);
        spline.addControlPoint(Vector(0.424, -0.625f), 1.853);
        spline.addControlPoint(Vector(0.509, -0.305f), 2.370);
        spline.addControlPoint(Vector(0.432, 0.000f), 2.910);
        spline.addControlPoint(Vector(0.506, 0.425f), 3.490);
        spline.addControlPoint(Vector(0.441, 0.785f), 4.050);
        spline.addControlPoint(Vector(0.505, 1.085f), 4.640);
        spline.addControlPoint(Vector(0.430, 1.485f), 5.220);
        spline.computeV();

        holeMiddle = getSurfacePoint(Vector(0.432, 0.000f), 0);
        holeRadius = 0.4f;

        circleDelta = 2 * PI / circleRes;
        firstT = spline.getCp(0).t;
        lastT = spline.getLastCp().t;
        splineDelta = (lastT - firstT) / (float) splineRes;


        size_t currentVertex = 0;
        for (float t = firstT; t < lastT; t += splineDelta) {
            size_t prevCp = 0;
            while (spline.getCp(prevCp + 1).t < t)
                prevCp++;

            Vector splineP1 = spline.getPos(t, prevCp);
            Vector splineP2 = spline.getPos(t + splineDelta, prevCp);

            for (int i = 0; i <= circleRes; i++) {
                float angle = i * circleDelta;
                Vector bottom = getSurfacePoint(splineP1, angle);
                Vector top = getSurfacePoint(splineP2, angle);
                Vector bottomNormal = getNormal(bottom, t, angle, splineP1, spline);
                Vector topNormal = getNormal(top, t + splineDelta, angle, splineP2, spline);

                normals[currentVertex] = topNormal;
                vertexes[currentVertex++] = top;
                normals[currentVertex] = bottomNormal;
                vertexes[currentVertex++] = bottom;
            }
        }
    }

    ~RotatedSpline() {
        if (vertexes != NULL)
            delete[] vertexes;
        if (normals != NULL)
            delete[] normals;
    }
};

class Ellipsoid : public Object {
    float a, b, c;
    Vector center;
    bool textured;

    int vertexNr;
    Vector *vertexes;
    Vector *normals;
    Vector *texVertexes;

    Vector getFirstPartialDerivative(float u, float v) const {
        return Vector(
                a * (-1.0f * sinf(u)) * cosf(v),
                b * (-1.0f * sinf(u)) * sinf(v),
                c * cosf(u)
        );
    }

    Vector getSecondPartialDerivative(float u, float v) const {
        return Vector(
                a * cosf(u) * (-1.0f * sinf(v)),
                b * cosf(u) * cosf(v),
                0.0f
        );
    }

    Vector getSurfacePoint(float u, float v) const {
        return Vector(
                a * cosf(u) * cosf(v),
                b * cosf(u) * sinf(v),
                c * sinf(u)
        );
    }

    Vector getNormal(float u, float v) const {
        if (PI / (-2.0f) < u && u < PI / 2.0f)
            return (getSecondPartialDerivative(u, v) % getFirstPartialDerivative(u, v)).normalized();
        if (PI / (-2.0f) >= u)
            return Vector(0, 0, -1);
        return Vector(0, 0, 1);
    }

public:
    Ellipsoid() {
        vertexes = NULL;
        normals = NULL;
        texVertexes = NULL;
    }

    Ellipsoid(float a, float b, float c, Material const &material, Vector const &pos, bool textured)
            : Object(material), a(a), b(b), c(c), center(pos), textured(textured) {
        vertexes = NULL;
        normals = NULL;
        texVertexes = NULL;
    }


    void draw() {
        glPushMatrix();
        glTranslatef(center.x, center.y, center.z);

        if (textured) {
            // szép oldal felé forgatás
            glRotatef(180.0f, 0, 0, 1);
            glRotatef(-90.0f, 1, 0, 0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture->getTexture());
        }

        material.setOGL();

        glBegin(GL_QUAD_STRIP);
        for (size_t i = 0; i < vertexNr; i++) {
            if (textured)
                glTexCoord(texVertexes[i]);
            glNormal(normals[i]);
            glVertex(vertexes[i]);
        }
        glEnd();

        if (textured) {
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_TEXTURE_2D);
        }
        glPopMatrix();
    }

    void generate(int resolution) {
        vertexNr = (resolution + 1) * (resolution + 1) * 2;
        vertexes = new Vector[vertexNr];
        normals = new Vector[vertexNr];
        texVertexes = new Vector[vertexNr];

        // -90 -> 90, -180 -> 180
        float startU = -PI / 2;
        float startV = -PI;
        float endU = PI / 2;
        float endV = PI;
        float du = (endU - startU) / resolution;
        float dv = (endV - startV) / resolution;

        size_t currentVertex = 0;
        for (float u = startU; u <= endU; u += du)
            for (float v = startV; v <= endV; v += dv) {
                vertexes[currentVertex] = getSurfacePoint(u, v);
                vertexes[currentVertex + 1] = getSurfacePoint(u + du, v);
                normals[currentVertex] = getNormal(u, v);
                normals[currentVertex + 1] = getNormal(u + du, v);

                if (textured) {
                    float uX = (u - startU) / (endU - startU);
                    float uX1 = (u - startU + du) / (endU - startU);
                    float vX = (v - startV) / (endV - startV);
                    texVertexes[currentVertex] = Vector(uX, vX);
                    texVertexes[currentVertex + 1] = Vector(uX1, vX);
                }

                currentVertex += 2;
            }
    }

    Vector const &getCenter() const {
        return center;
    }

    ~Ellipsoid() {
        if (vertexes != NULL)
            delete[] vertexes;
        if (normals != NULL)
            delete[] normals;
        if (texVertexes != NULL)
            delete[] texVertexes;
    }
};

class Cone : public Object {
    float r, height;
    Vector center, rotate;

    int vertexNr;
    Vector *vertexes;

public:
    Cone() {
        vertexes = NULL;
    }

    Cone(float r, float height, Vector const &center, Vector const &rotate, Material const &material)
            : Object(material), r(r), height(height), center(center), rotate(rotate) {
        vertexes = NULL;
    }

    void draw() {
        glPushMatrix();
        glTranslatef(center.x, center.y, center.z);
        glRotatef(rotate.x, 1, 0, 0);
        glRotatef(rotate.y, 0, 1, 0);
        glRotatef(rotate.z, 0, 0, 1);
        glScalef(r, height, r);

        material.setOGL();

        glBegin(GL_TRIANGLE_FAN);
        Vector top = Vector(0, 1, 0);
        glNormal(top);
        glVertex(top);
        for (int i = 0; i < vertexNr; i++) {
            glNormal(vertexes[i]);
            glVertex(vertexes[i]);
        }
        glEnd();
        glPopMatrix();
    }

    void generate(int resolution) {
        vertexNr = resolution + 1;
        vertexes = new Vector[vertexNr];

        float delta = 2 * PI / (float) resolution;
        for (int i = 0; i <= resolution; i++) {
            Vector pointOnCircle = Vector(
                    (r * (float) cos(i * delta)),
                    0.0f,
                    0.0f + (r * (float) sin(i * delta))
            );
            vertexes[i] = pointOnCircle;
        }
    }

    ~Cone() {
        if (vertexes != NULL)
            delete[] vertexes;
    }
};

class Rectangle : public Object {
    Vector a, b, c, d;
    Vector normal;

public:
    Rectangle() {
    }

    Rectangle(Material const &material, Vector const &a, Vector const &b, Vector const &c, Vector const &d, Vector const &normal)
            : Object(material), a(a), b(b), c(c), d(d), normal(normal) {
    }

    void draw() {
        // TODO tesellation
        material.setOGL();
        glBegin(GL_QUADS);
        glNormal(normal);
        glVertex(a);
        glNormal(normal);
        glVertex(b);
        glNormal(normal);
        glVertex(c);
        glNormal(normal);
        glVertex(d);
        glEnd();
    }
};

class FramedRectangle : public Object {
    Vector pos, rotate;
    Rectangle frame, inside, back;

public:
    FramedRectangle() {

    }

    FramedRectangle(Vector const &bottomLeft, Vector const &topRight, Vector const &pos, Vector const &rotate)
            : pos(pos), rotate(rotate) {

        Vector b = bottomLeft;
        Vector d = topRight;
        Vector a = Vector(bottomLeft.x, topRight.y);
        Vector c = Vector(topRight.x, bottomLeft.y);

        Vector ad = d - a;
        Vector ab = b - a;
        Vector ba = a - b;
        Vector bc = c - b;
        Vector cd = d - c;
        Vector cb = b - c;
        Vector da = a - d;
        Vector dc = c - d;
        float frameSize = (ad.length() + ab.length() + bc.length() + cd.length()) / 4.0f / 20.0f;

        Vector aInside = a + ad.normalized() * frameSize + ab.normalized() * frameSize + Vector(0, 0, 0.05f);
        Vector bInside = b + ba.normalized() * frameSize + bc.normalized() * frameSize + Vector(0, 0, 0.05f);
        Vector cInside = c + cd.normalized() * frameSize + cb.normalized() * frameSize + Vector(0, 0, 0.05f);
        Vector dInside = d + da.normalized() * frameSize + dc.normalized() * frameSize + Vector(0, 0, 0.05f);

        Vector aBack = a - Vector(0, 0, 0.05f);
        Vector bBack = b - Vector(0, 0, 0.05f);
        Vector cBack = c - Vector(0, 0, 0.05f);
        Vector dBack = d - Vector(0, 0, 0.05f);

        frame = Rectangle(chrome, a, b, c, d, Vector(0, 0, 1));
        inside = Rectangle(solarPanelMaterial, aInside, bInside, cInside, dInside, Vector(0, 0, 1));
        back = Rectangle(chrome, aBack, bBack, cBack, dBack, Vector(0, 0, -1));

    }

    void draw() {
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);

        frame.draw();
        inside.draw();
        back.draw();

        glPopMatrix();
    }
};

class Space : public Object {
    Color space[screenWidth * screenHeight];
public:
    Space() {
    }

    void generate() {
        for (int Y = 0; Y < screenHeight; Y++)
            for (int X = 0; X < screenWidth; X++) {
                int current = Y * screenWidth + X;
                if ((current % 184) == (Y * X % 211))
                    space[current] = Color(1.0f, 1.0f, 1.0f);
                else
                    space[current] = Color(0.0f, 0.0f, 0.0f);
            }
    }

    void draw() {
        glDisable(GL_DEPTH_TEST);
        glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_FLOAT, space);
        glEnable(GL_DEPTH_TEST);
    }
};

class Jet : public Object {
    Cone jetOutside;
    Vector pos;
    // Cone jetInside, fire;
    bool lit;
    long startTime;


public:
    Jet() : Object() {
    }

    Jet(float size, Vector const &pos, Vector const &rotation) : Object(), pos(pos) {
        jetOutside = Cone(size, size, pos, rotation, chrome);
        lit = false;
    }

    void draw() {
        jetOutside.draw();
    }

    void generate(int resolution) {
        jetOutside.generate(resolution);
    }

    void setLit(bool lit) {
        Jet::lit = lit;
    }


    void setStartTime(long startTime) {
        Jet::startTime = startTime;
    }


    Vector const &getPos() const {
        return pos;
    }
};

class Satellite : public Object {
    Vector pos;
    Vector v;
    float size;

    Ellipsoid satelliteBody;
    Jet jetLeft, jetRight, jetBack, jetFront, jetBottom, jetTop;

public:
    Satellite() {
    }

    Satellite(Vector const &pos, float size)
            : pos(pos),
              size(size) {
        satelliteBody = Ellipsoid(size, size, size, chrome, Vector(0, 0, 0), false);

        jetLeft = Jet(size, Vector(-size * 2.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, -90.0f));
        jetRight = Jet(size, Vector(size * 2.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 90.0f));
        jetBack = Jet(size, Vector(0.0f, 0.0f, size * 2.0f), Vector(-90.0f, 0.0f, 0.0f));
        jetFront = Jet(size, Vector(0.0f, 0.0f, -size * 2.0f), Vector(90.0f, 0.0f, 0.0f));
        jetBottom = Jet(size, Vector(0.0f, -size * 2.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f));
        jetTop = Jet(size, Vector(0.0f, size * 2.0f, 0.0f), Vector(180.0f, 0.0f, 0.0f));
        v = Vector(0, 0, 0);
    }

    void draw() {
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);

        enableThrowBackCW();
        satelliteBody.draw();
        jetLeft.draw();
        jetRight.draw();
        jetTop.draw();
        jetBottom.draw();
        disableThrowBack();
        jetBack.draw();
        jetFront.draw();

        glPopMatrix();
    }

    void generate() {
        satelliteBody.generate(140);
        int coneResolution = 10;
        jetLeft.generate(coneResolution);
        jetRight.generate(coneResolution);
        jetTop.generate(coneResolution);
        jetBottom.generate(coneResolution);
        jetBack.generate(coneResolution);
        jetFront.generate(coneResolution);
    }

    void startJet(Jet *jet, long time) {
        jet->setStartTime(time);
        jet->setLit(true);
        Vector impulse = jet->getPos() * (-1);
        v = v + impulse;
    }

    Jet *getJetLeft() {
        return &jetLeft;
    }

    Jet *getJetRight() {
        return &jetRight;
    }

    Jet *getJetBack() {
        return &jetBack;
    }

    Jet *getJetFront() {
        return &jetFront;
    }

    Jet *getJetBottom() {
        return &jetBottom;
    }

    Jet *getJetTop() {
        return &jetTop;
    }


    Vector const &getPos() const {
        return pos;
    }

    void setPos(Vector const &pos) {
        Satellite::pos = pos;
    }

    Vector const &getV() const {
        return v;
    }
} satellite;

class Station : public Object {
    Vector pos;

    float orbitAngleRad, orbitDistance;
    Vector orbitMiddle;

    float rotationAngleDeg;

    RotatedSpline rotatedSpline;
    FramedRectangle solarPanel1;
    FramedRectangle solarPanel2;

public:
    Station() {
    }

    Station(Ellipsoid const &planet, float orbitDistance, float orbitAngle)
            : orbitDistance(orbitDistance), orbitAngleRad(orbitAngle) {
        rotationAngleDeg = 0.0f;
        orbitMiddle = planet.getCenter();

        float posX = (float) (orbitMiddle.x - orbitDistance * cos(orbitAngle));
        float posZ = (float) (orbitMiddle.z + orbitDistance * sin(orbitAngle));
        pos = Vector(posX, 0.0f, posZ);

        solarPanel1 = FramedRectangle(Vector(0.0f, 0.0f, 0.0f), Vector(2.0f, 0.8f, 0.0f), Vector(0.5, 0.0, 0.0), Vector(-20.0, 0.0, 0.0));
        solarPanel2 = FramedRectangle(Vector(0.0f, 0.0f, 0.0f), Vector(2.0f, 0.8f, 0.0f), Vector(-2.5, 0.0, 0.0), Vector(-20.0, 0.0, 0.0));
        rotatedSpline = RotatedSpline(chrome);
    }

    void draw() {
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(20, 0, 0, 1);
        glRotatef(rotationAngleDeg, 0, 1, 0);

        enableThrowBackCCW();
        rotatedSpline.draw();
        disableThrowBack();
        solarPanel1.draw();
        solarPanel2.draw();
        glPopMatrix();
    }

    void generate() {
        rotatedSpline.generate(100, 100);
    }

    Vector const &getPos() const {
        return pos;
    }

    void setPos(Vector const &pos) {
        Station::pos = pos;
    }

    void setRotationAngleDeg(float rotationAngle) {
        Station::rotationAngleDeg = rotationAngle;
    }

    void setOrbitAngleRad(float orbitAngle) {
        Station::orbitAngleRad = orbitAngle;
        float posX = (float) (orbitMiddle.x - orbitDistance * cos(orbitAngle));
        float posZ = (float) (orbitMiddle.z + orbitDistance * sin(orbitAngle));
        pos = Vector(posX, 0.0f, posZ);
    }
};

class Light {
    GLenum lightId;
    Material lightColor;
    bool spot;
    Vector pos;

public:
    Light(int id, Vector const &pos, Material const &lightColor, bool spot)
            : pos(pos), lightColor(lightColor), spot(spot) {
        switch (id) {
            case 0:
                lightId = GL_LIGHT0;
                break;
            case 1:
                lightId = GL_LIGHT1;
                break;
            case 3:
                lightId = GL_LIGHT3;
                break;
            case 4:
                lightId = GL_LIGHT4;
                break;
            case 5:
                lightId = GL_LIGHT5;
                break;
            case 6:
                lightId = GL_LIGHT6;
                break;
            case 7:
                lightId = GL_LIGHT7;
                break;
            default:
                lightId = NULL;
                break;
        }

        // pozíció, utolsó koordináta 0, ha irányfényforrás
        GLfloat lightPos[] = {pos.x, pos.y, pos.z, spot};
        glLightfv(lightId, GL_POSITION, lightPos);

        // szín
        Color diffuse = lightColor.getDiffuse();
        Color ambient = lightColor.getAmbient();
        Color specular = lightColor.getSpecular();

        GLfloat diffuseArr[] = {diffuse.r, diffuse.g, diffuse.b, 1.0f};
        GLfloat ambientArr[] = {ambient.r, ambient.g, ambient.b, 1.0f};
        GLfloat specularArr[] = {specular.r, specular.g, specular.b, 1.0f};

        glLightfv(lightId, GL_DIFFUSE, diffuseArr);
        glLightfv(lightId, GL_AMBIENT, ambientArr);
        glLightfv(lightId, GL_SPECULAR, specularArr);
        glLightf(lightId, GL_SHININESS, lightColor.getShine());
    }

    Light() {
    }

    void enable() {
        glEnable(GL_LIGHTING);
        glEnable(lightId);
    }

    void disable() {
        glDisable(GL_LIGHTING);
        glDisable(lightId);
    }
};

void debug() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

class Camera {
    Vector eye, lookat, up;
    float viewAngleDegree, zNear, zFar;

public:
    Camera() {
    }

    Camera(Vector const &eye, Vector const &lookat, Vector const &up, float viewAngleDegree, float zNear, float zFar)
            : eye(eye), lookat(lookat), up(up), viewAngleDegree(viewAngleDegree), zNear(zNear), zFar(zFar) {
    }

    void setOGL() {
        // képernyő méret
        glViewport(0, 0, screenWidth, screenHeight);

        // projekció mátrix beállítás
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glEnable(GL_DEPTH_TEST);
        gluPerspective(viewAngleDegree, (float) screenWidth / (float) screenHeight, zNear, zFar);

        // modelview mátrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eye.x, eye.y, eye.z, lookat.x, lookat.y, lookat.z, up.x, up.y, up.z);
    }
};

class Scene {
    long currentTime;

    Vector eye, lookat;

    Light light;
    Camera camera;
    Ellipsoid earth;
    Ellipsoid atmosphere;
    Ellipsoid sun;
    Station station;
    PlanetTexture planetTexture;
    Space space;

    float ropeLength, kickDistance;
    Vector floatingDirection;
    float floatingSpeed;
    float stretchLength;
    bool hasKicked;
    long stretchStartTime;
    bool stretching;

    long orbitTime;
    float orbitStartAngle;

    long rotationTime;

    void setCamera() {
        Vector up(0, 1, 0);
        float zNear = 0.1;
        float zFar = 55;
        float viewAngle = 60.0f;
        camera = Camera(eye, lookat, up, viewAngle, zNear, zFar);
        camera.setOGL();
    }

    float degreeToRad(float degree) {
        return degree * 2.0f * PI / 360.0f;
    }

    void moveStation(float ts) {
        float periodsDone = floorf(ts / orbitTime);
        float timePastInPeriod = ts - (periodsDone * orbitTime);
        float periodPart = timePastInPeriod / orbitTime;
        float angle = 2 * PI * periodPart + orbitStartAngle;
        station.setOrbitAngleRad(angle);
    }

    void rotateStation(float ts) {
        float periodsDone = floorf(ts / rotationTime);
        float timePastInPeriod = ts - (periodsDone * rotationTime);
        float periodPart = timePastInPeriod / rotationTime;
        float angle = 360 * periodPart;
        station.setRotationAngleDeg(angle);
    }

public:
    void build() {
        planetTexture.setOGL();

        Vector earthCenter = Vector(-4.0f, 0.0f, -25.0f);
        earth = Ellipsoid(10.0f, 10.0f, 10.0f, planet, earthCenter, true);
        earth.setTexture(&planetTexture);
        earth.generate(30);

        atmosphere = Ellipsoid(11.f, 11.0f, 11.0f, atmosphereMat, earthCenter, false);
        atmosphere.generate(40);

        Vector sunCenter = Vector(8, 5.0f, 3.0f);
        sun = Ellipsoid(1.0f, 1.0f, 1.0f, sunColor, sunCenter, false);
        sun.generate(10);

        light = Light(0, sunCenter, sunLight, false);

        orbitTime = 200000;
        orbitStartAngle = degreeToRad(60);
        rotationTime = 10000;
        float orbitDistance = 20;
        station = Station(earth, orbitDistance, orbitStartAngle);
        station.generate();

        Vector satellitePos = station.getPos() + Vector(1, -1, 2);
        satellite = Satellite(satellitePos, 0.3f);
        satellite.generate();

        ropeLength = 6;
        stretchLength = 3;
        kickDistance = 4;
        floatingDirection = Vector(0, 0, 1);
        floatingSpeed = 1;
        hasKicked = true;
        stretching = false;

        lookat = station.getPos();
        eye = lookat + Vector(0, 0, 3);
        setCamera();
    };

    void render() {
        disableThrowBack();
        space.draw();
        light.enable();

        enableThrowBackCW();
        earth.draw();
        disableThrowBack();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        enableThrowBackCW();
        atmosphere.draw();
        disableThrowBack();
        glDisable(GL_BLEND);

        station.draw();
        satellite.draw();

        light.disable();
        glColor3f(sunColor.getAmbient().r, sunColor.getAmbient().g, sunColor.getAmbient().b);
        enableThrowBackCW();
        sun.draw();
    };

    void generateTextures() {
        planetTexture.generate();
        space.generate();
    }

    void simulateTimeSlice(long sliceStart, long sliceEnd) {
        rotateStation(sliceEnd);
        moveStation(sliceEnd);

        long deltaT = sliceEnd - sliceStart;
        satellite.setPos(satellite.getPos() + (satellite.getV() * deltaT / 1000));

        lookat = station.getPos();

        Vector newEye = eye + (floatingDirection * floatingSpeed * deltaT / 1000);
        float newDistance = (newEye - lookat).length();

        if (!hasKicked && newDistance < kickDistance) {
            eye = newEye;
            floatingDirection = (lookat - earth.getCenter()).normalized();
            floatingSpeed = 2;
            hasKicked = true;
        }
        else if (newDistance < ropeLength) {
            eye = newEye;
        }
        else if (stretching) {
            // Harmonikus rezgőmozgás
            // Kitérés = Amplitúdó * sin(szögsebesség * idő) (+ eltolás)
            // frekvencia = maxsebesség(sec) / amplitúdó

            float amplitude = stretchLength * 2;
            float frequency = floatingSpeed / amplitude / 1000.0f;
            float stretchDistance = amplitude * sinf(frequency * (sliceEnd - stretchStartTime));
            if (stretchDistance >= 0) {
                Vector direction = (eye - lookat).normalized();
                eye = lookat + (direction * (ropeLength + stretchDistance));
            }
            else {
                Vector direction = (eye - lookat).normalized();
                eye = lookat + (direction * ropeLength);
                stretching = false;
                floatingDirection = (lookat - eye).normalized();
            }
        }
        else {
            stretching = true;
            hasKicked = false;
            stretchStartTime = sliceEnd;
        }
        setCamera();
    }

    void simulateWorldSince(long tstart) {
        int dt = 25;
        for (long sliceStart = tstart; sliceStart < currentTime; sliceStart += dt) {
            float te = (currentTime < sliceStart + dt ? currentTime : sliceStart + dt);
            simulateTimeSlice(sliceStart, te);
        }
    }

    long getTime() const {
        return currentTime;
    }

    void setTime(long currentTime) {
        Scene::currentTime = currentTime;
    }

} scene;

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization() {
    scene.generateTextures();
    scene.build();
    glShadeModel(GL_SMOOTH);
    //debug();
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay() {
    glClearColor(0.0, 0.0, 0.0, 1.0);        // torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles
    scene.render();
    glutSwapBuffers();                    // Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y) {
    Jet *jet;
    switch (key) {
        case 'w' :
            jet = satellite.getJetBack();
            break;
        case 'a':
            jet = satellite.getJetRight();
            break;
        case 's':
            jet = satellite.getJetFront();
            break;
        case 'd':
            jet = satellite.getJetLeft();
            break;
        case 'q':
            jet = satellite.getJetBottom();
            break;
        case 'e':
            jet = satellite.getJetTop();
            break;
        default:
            jet = NULL;
    }
    if (jet != NULL) {
        satellite.startJet(jet, scene.getTime());
        glutPostRedisplay();
    }

}

// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y) {

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)   // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
        glutPostRedisplay();                         // Ilyenkor rajzold ujra a kepet
}

// Eger mozgast lekezelo fuggveny
void onMouseMotion(int x, int y) {

}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle() {
    long oldTime = scene.getTime();
    scene.setTime(glutGet(GLUT_ELAPSED_TIME));        // program inditasa ota eltelt ido
    scene.simulateWorldSince(oldTime);
    glutPostRedisplay();

}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv);                // GLUT inicializalasa
    glutInitWindowSize(600, 600);            // Alkalmazas ablak kezdeti merete 600x600 pixel
    glutInitWindowPosition(100, 100);            // Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);    // 8 bites R,G,B,A + dupla buffer + melyseg buffer

    glutCreateWindow("Grafika hazi feladat");        // Alkalmazas ablak megszuletik es megjelenik a kepernyon

    glMatrixMode(GL_MODELVIEW);                // A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);            // A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();

    onInitialization();                    // Az altalad irt inicializalast lefuttatjuk

    glutDisplayFunc(onDisplay);                // Esemenykezelok regisztralasa
    glutMouseFunc(onMouse);
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutMotionFunc(onMouseMotion);

    glutMainLoop();                    // Esemenykezelo hurok

    return 0;
}