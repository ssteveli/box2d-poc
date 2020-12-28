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

    bool OnUserCreate() override
    {
        b2olc::transX = ScreenWidth() / 2;
        b2olc::transY = ScreenHeight() / 2;
        b2olc::screenHeight = ScreenHeight();

        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.0f, -50.0f);
        b2Body *groundBody = world->CreateBody(&groundBodyDef);

        b2PolygonShape groundBox;
        groundBox.SetAsBox(50.0f, 10.0f);

        groundBody->CreateFixture(&groundBox, 0.0f);

        for (int i = 0; i < 25; i++)
        {
            b2Body *body;
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;

            olc::vi2d p = {rand() % ScreenWidth(), rand() % ScreenHeight()};
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

        if (GetMouse(1).bPressed || GetMouse(1).bHeld)
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

        world->Step(fElapsedTime, velocityIterations, positionIterations);

        Clear(olc::BLACK);
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
        return true;
    }
};

int main(int, char **)
{
    Balls balls;
    if (balls.Construct(800, 800, 1, 1))
        balls.Start();

    return 0;
}
