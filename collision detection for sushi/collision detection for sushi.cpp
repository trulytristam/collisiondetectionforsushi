// collision detection for sushi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define OLC_PGE_APPLICATION
#include <iostream>
#include "olcPixelGameEngine.h"
#include <list>
#include <vector>
using namespace std;

typedef olc::vf2d v2;

v2 rotate(v2 p, float a) {
    float c = cos(a); float s = sin(a);
    return { c * p.x - s * p.y, s * p.x + c * p.y };
}
struct Shape;
struct mani {
   
    Shape* a = nullptr;
    Shape* b = nullptr;
    int refID = 0;
    v2 colPoints[2];
    float depth[2];
    int nColPoints = 0;

    

    v2 norm;


};

int GetfurthestID(Shape*, v2 );
int GetFaceID(Shape*, v2);
void ClipSide(v2*, Shape*, int);
void Clip(mani&);

struct Shape {
    v2 p;
    float a = 0;
    float scale = 100;

    vector<v2> points;
    vector<v2> pointsTrans;
    Shape(){}

    Shape(v2 pp, int np) {
        p = pp;
        float ang = 0;
        float inc = 6.28 / (float)np;
        for (int i = 0; i < np; i++) {

            points.push_back(v2(sin(ang), cos(ang)));
            ang += inc;
        }
    }
    
    void Update() {

        pointsTrans.clear();
        for (int i = 0; i < points.size(); i++) {

            pointsTrans.push_back(rotate(points[i],a) * scale + p);
        }



    }
    void Draw(olc::PixelGameEngine* scr) {
        for (int i = 0; i < pointsTrans.size();i++) {
            scr->DrawLine(pointsTrans[i], pointsTrans[(i + 1)% pointsTrans.size()], olc::BLACK);
        }
    }

    bool PointInside(v2 point) {
        int s = pointsTrans.size();
        for (int i = 0; i < s; i++) {
            v2 a = pointsTrans[i];
            v2 b = pointsTrans[(i+1)%s];

            v2 n = (b - a).perp();

            if (n.dot(point - a) > 0) {
                return false;
            }
        }
        return true;
    }

    bool Sat(Shape& b, mani& m) {
        float minpen = -INFINITY;
        v2 bestnorm; Shape* referenceShape = nullptr;
        Shape* First = this;
        Shape* Second = &b;
        for (int t = 0; t < 2; t++) {
            if (t == 1) {
                First = &b;
                Second = this;
            }
            int A_size = First->pointsTrans.size();
            int B_size = Second->pointsTrans.size();
            for (int i = 0; i < A_size; i++) {

                v2 facepoint1 = First->pointsTrans[i];
                v2 facepoint2 = First->pointsTrans[(i + 1) % A_size];
                v2 norm = (facepoint2 - facepoint1).perp().norm();

                float deepest = INFINITY;
                for (int j = 0; j < B_size; j++) {

                    v2 vert = Second->pointsTrans[j];
                    float vertdepth = (vert - facepoint1).dot(norm);
                    if (vertdepth < deepest) {
                        deepest = vertdepth;
                    }
                }
                if (deepest > 0) { return false; }
                else if (deepest > minpen) {
                    minpen = deepest;
                    bestnorm = norm;
                    referenceShape = First;
                }
            }
        }
        m.a = referenceShape;
        m.b = (this == referenceShape) ? &b : this;
        m.norm = bestnorm;  

        Clip(m);

        return true;
    }
};

int GetfurthestID(Shape* s, v2 n) {
    float furthest = -INFINITY;
    int furthestID = 0;
    for (int i = 0; i < s->pointsTrans.size(); i++) {
        float dist = n.dot(s->pointsTrans[i]);
        if (dist > furthest) {
            furthest = dist; 
            furthestID = i;
        }
    }
    return furthestID;
}


int GetFaceID(Shape* s, v2 n) {

    int furthestPointID = GetfurthestID(s, n);
    float mostSimilar = -1.0;
    int ID = 0;

    for (int i = 0; i < s->pointsTrans.size(); i++) {
        int ione = i; int itwo = (i + 1) % s->pointsTrans.size();

        v2 pointone = s->pointsTrans[ione];
        v2 pointtwo = s->pointsTrans[itwo];
        if (ione == furthestPointID or itwo == furthestPointID) {

            v2 norm = (pointtwo - pointone).perp().norm();

            float similarity = norm.dot(n);
            if (similarity > mostSimilar) {
                mostSimilar = similarity;
                ID = i;
            }
        }
    }
    return ID;
}

void ClipSide(v2* collisionPoints, Shape* s, int faceID) {

    int outside = 0;
    int inside = 0;
    v2 facepoint = s->pointsTrans[faceID];
    v2 facepoint2 = s->pointsTrans[(faceID + 1) % s->pointsTrans.size()];
    v2 norm = (facepoint2 - facepoint).perp().norm();

    float dists[2];

    for (int i = 0; i < 2; i++) {      
        float dist = (collisionPoints[i] - facepoint).dot(norm);
        dists[i] = dist;
        if (dist > 0) {
            outside++;

        }
        else {
            inside++;
        }
    }

    if (inside == 2) {
        return;
    }
    else if (inside == 1) {
        float total = abs(dists[0]) + abs(dists[1]);     
        int insideID = (dists[0] > 0) ? 1 : 0;
        int outsideID = !insideID;
        float ratio = abs(dists[insideID]) / total;
        v2 v = (collisionPoints[outsideID] - collisionPoints[insideID]) * ratio;
        v2 newPoint = collisionPoints[insideID] + v;
        collisionPoints[outsideID] = newPoint;

    }


}

void Clip(mani& m) {

    //find face id's for both shapes, we already know shape A in the manifold is the reference shape
    int refID = GetFaceID(m.a, m.norm);
    int incID = GetFaceID(m.b, -m.norm);

    int asize = m.a->pointsTrans.size();
    int bsize = m.b->pointsTrans.size();

    v2 collisionPoints[2] = {m.b->pointsTrans[incID], m.b->pointsTrans[(incID+1)% bsize]};

    //clip collisionPoints against adjacent faces of reference;

    ClipSide(collisionPoints, m.a, ((refID + (asize - 1)) % asize));
    ClipSide(collisionPoints, m.a, ((refID + 1) % asize));
    m.nColPoints = 2;
    
    //Get the distance from the reference face of collision points
    for (int i = 0; i < 2; i++) {
        float dist = (collisionPoints[i] - m.a->pointsTrans[refID]).dot(m.norm);
        m.depth[i] = dist;
    }
    m.colPoints[0] = collisionPoints[0];
    m.colPoints[1] = collisionPoints[1];

    //remove collision points that are above reference face and reorder them in the manifold if needed
    for (int i = 0; i < 2; i++) {
        if (m.depth[i] > 0) {
            m.nColPoints--;
            if (i == 0) {
                m.colPoints[0] = m.colPoints[1];
                m.depth[0] = m.depth[1];
            }

        }
    }

    m.refID = refID;
}

struct test;
void GetInput(test*);

struct test : public olc::PixelGameEngine {

    list<Shape> shapes;


    vector<mani> manifolds;


    bool OnUserCreate() {


        shapes.push_back(Shape(v2(40, 100), 5));
        shapes.push_back(Shape(v2(200, 100), 3));
        shapes.push_back(Shape(v2(200, 100), 4));
        
        return true;
    }


    Shape* selected = nullptr;

    v2 vMouse;


    void FillManifolds() {
        manifolds.clear();
        for (auto a = shapes.begin(); a != shapes.end(); a++) {
            auto anext = a;
            anext++;
            for (auto b = anext; b != shapes.end(); b++) {
                mani m;
                if ((*a).Sat((*b), m)) {
                    //cout << "collsion detected" << endl;
                    manifolds.push_back(m);
                }
            }
        }
    }

    void DrawManifolds() {
        //cout << "manifolds count << " << manifolds.size() << endl;
        v2 legone = v2(sin(0.4), -cos(0.4));
        v2 legtwo = v2(sin(-0.4), -cos(-0.4));
        for (auto& m : manifolds) {

            for (int i = 0; i < m.nColPoints; i++) {

                FillCircle(m.colPoints[i], 3, olc::RED);



                DrawLine(m.colPoints[i], m.colPoints[i] + (-m.norm * m.depth[i]), olc::GREEN);


            }

            v2 reffacepointone = m.a->pointsTrans[m.refID];
            v2 reffacepointtwo = m.a->pointsTrans[(m.refID+1)% m.a->pointsTrans.size()];
            v2 mid = (reffacepointtwo + reffacepointone) / 2;

            v2 tip = mid + (m.norm * 30);
            DrawLine(mid, tip, olc::YELLOW);


            v2 vy = (tip - mid).norm();
            v2 vx = -vy.perp();

            float legonex = vx.x * legone.x + vy.x * legone.y;
            float legoney = vx.y * legone.x + vy.y * legone.y;
            float legtwox = vx.x * legtwo.x + vy.x * legtwo.y;
            float legtwoy = vx.y * legtwo.x + vy.y * legtwo.y;

            DrawLine(tip, tip + v2(legonex, legoney)*10, olc::YELLOW);
            DrawLine(tip, tip + v2(legtwox, legtwoy)*10, olc::YELLOW);
        }
    }

    bool OnUserUpdate(float ft) {
        Clear(olc::DARK_GREY);
        vMouse = v2(GetMouseX(), GetMouseY());
        
        GetInput(this);

        for (auto& s : shapes) {
            s.Update();
        }

        FillManifolds();

        for (auto& s : shapes) {          
            s.Draw(this);
        }


        DrawManifolds();
        
        return true;
    }

};

void GetInput(test* scr) {

    if (scr->GetMouse(0).bPressed) {
        for (auto& s : scr->shapes) {

            if (s.PointInside(scr->vMouse)) {


                scr->selected = &s;

            }
            s.Update();

        }
    }

    if (scr->selected) {
        scr->selected->p = scr->vMouse;

        if (scr->GetMouseWheel() != 0) {
            if (scr->GetMouseWheel() > 0) {
                scr->selected->a += 0.2;
            }
            else {
                scr->selected->a -= 0.2;
            }
        }

    }
    if (scr->GetMouse(0).bReleased) {
        scr->selected = nullptr;
    }


}


int main()
{
    
    test t;
    if (t.Construct(600, 600, 1, 1)) {
        cout << "helo" << endl;
    }


    t.Start();
}

