#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine/olcPixelGameEngine.h"

#include <box2d/box2d.h>
#include <box2d/b2_world.h>

#include "olcBox2d.h"

#include <iostream>

class Balls : public olc::PixelGameEngine
{
private:
    b2Vec2 *gravity;
    b2World *world;
    int32 velocityIterations = 8;
    int32 positionIterations = 3;
    std::vector<b2Body *> bodies;

    float blockerRadius = 300.0f;

public:
    Balls()
    {
        sAppName = "Box2d Balls";

        gravity = new b2Vec2(0.0f, -10.0f);
        world = new b2World(*gravity);
    }

    ~Balls()
    {
        delete gravity;
        delete world;
    }

    void addBall()
    {
        b2Body *body;
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;

        olc::vi2d p = GetMousePos();
        b2Vec2 wp = b2olc::pixelsToWorld((float)p.x, (float)p.y);

        bodyDef.position.Set(wp.x, wp.y);
        body = world->CreateBody(&bodyDef);

        b2CircleShape circle;
        circle.m_radius = 2.0f;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circle;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.restitution = 1.0f;

        body->CreateFixture(&fixtureDef);

        bodies.push_back(body);
    }

    void addRect()
    {
        b2Body *body;
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;

        olc::vi2d p = GetMousePos();
        b2Vec2 wp = b2olc::pixelsToWorld((float)p.x, (float)p.y);

        bodyDef.position.Set(wp.x, wp.y);
        body = world->CreateBody(&bodyDef);

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(0.75f, 3.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.restitution = 0.8f;

        body->CreateFixture(&fixtureDef);

        bodies.push_back(body);
    }

    bool OnUserCreate() override
    {
        b2olc::transX = ScreenWidth() / 2;
        b2olc::transY = ScreenHeight() / 2;
        b2olc::screenHeight = ScreenHeight();

        b2BodyDef groundBodyDef;
        b2Vec2 bottom = b2olc::pixelsToWorld(ScreenWidth() / 2, ScreenHeight());
        groundBodyDef.position.Set(bottom.x, bottom.y);
        b2Body *groundBody = world->CreateBody(&groundBodyDef);

        b2PolygonShape groundBox;
        groundBox.SetAsBox(b2olc::scalarPixelsToWorld(ScreenWidth()), 0.1f);

        groundBody->CreateFixture(&groundBox, 0.0f);

        b2BodyDef leftBodyDef;
        b2Vec2 left = b2olc::pixelsToWorld(0.0f, 0.0f);
        leftBodyDef.position.Set(left.x, left.y);
        b2Body *leftBody = world->CreateBody(&leftBodyDef);

        b2PolygonShape leftBox;
        leftBox.SetAsBox(0.1f, b2olc::scalarPixelsToWorld(ScreenHeight()));
        leftBody->CreateFixture(&leftBox, 0.0f);

        b2BodyDef rightBodyDef;
        b2Vec2 right = b2olc::pixelsToWorld(ScreenWidth(), 0.0f);
        rightBodyDef.position.Set(right.x, right.y);
        b2Body *rightBody = world->CreateBody(&rightBodyDef);

        b2PolygonShape rightBox;
        rightBox.SetAsBox(0.1f, b2olc::scalarPixelsToWorld(ScreenHeight()));
        rightBody->CreateFixture(&rightBox, 0.0f);

        b2BodyDef blockerBodyDef;
        b2Vec2 blocker = b2olc::pixelsToWorld(ScreenWidth() / 2, ScreenHeight() / 2);
        blockerBodyDef.position.Set(blocker.x, blocker.y);
        b2Body *blockerBody = world->CreateBody(&blockerBodyDef);

        b2CircleShape blockerCircle;
        blockerCircle.m_radius = b2olc::scalarPixelsToWorld(blockerRadius);
        blockerBody->CreateFixture(&blockerCircle, 0.0f);

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        if (GetKey(olc::Key::R).bPressed)
        {
            for (auto &body : bodies)
            {
                world->DestroyBody(body);
            }
            bodies.clear();
        }

        if (GetMouse(0).bPressed || GetMouse(0).bHeld)
        {
            addRect();
        }

        if (GetMouse(1).bPressed || GetMouse(1).bHeld)
        {
            addBall();
        }

        world->Step(fElapsedTime, velocityIterations, positionIterations);

        Clear(olc::BLACK);
        DrawCircle(ScreenWidth() / 2, ScreenHeight() / 2, blockerRadius, olc::CYAN);
        for (auto &body : bodies)
        {
            b2Fixture *f = body->GetFixtureList();

            switch (f->GetType())
            {
            case b2Shape::e_circle:
            {
                b2CircleShape *circle = (b2CircleShape *)f->GetShape();
                b2Vec2 center = b2Mul(body->GetTransform(), circle->m_p);
                float radius = circle->m_radius;

                olc::vf2d p = b2olc::worldToPixels(center.x, center.y);
                DrawCircle(p, radius * b2olc::scaleFactor, olc::BLUE);
            }
            break;
            case b2Shape::e_polygon:
            {
                b2PolygonShape *poly = (b2PolygonShape *)f->GetShape();
                int32 vertexCount = poly->m_count;
                b2Vec2 vertices[vertexCount];

                for (int32 i = 0; i < vertexCount; ++i)
                {
                    vertices[i] = b2Mul(body->GetTransform(), poly->m_vertices[i]);
                }

                for (int i = 0; i < vertexCount; i++)
                {
                    olc::vf2d p1 = b2olc::worldToPixels(vertices[i].x, vertices[i].y);
                    olc::vf2d p2 = b2olc::worldToPixels(vertices[(i + 1) % vertexCount].x, vertices[(i + 1) % vertexCount].y);

                    DrawLine(p1, p2, olc::WHITE);
                }
            }
            break;
            default:
                break;
            }
        }

        return !GetKey(olc::Key::ESCAPE).bPressed;
    }
};

int main(int, char **)
{
    Balls balls;
    if (balls.Construct(1900, 1600, 1, 1))
        balls.Start();

    return 0;
}
