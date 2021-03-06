﻿#include "physics.h"
#include <QDebug>

Physics::Physics()
{
    this->type = b2_dynamicBody;
    this->density = 2.f;
    this->friction = 1.f;
    this->curSpeed = 0.f;
    this->maxSpeed = 10.f;
    this->x = 0.f;
    this->y = 0.f;
    this->w = 0.f;
    this->h = 0.f;
    this->pHworld = NULL;
    this->pHbody = NULL;
}

Physics::~Physics()
{
}

void Physics::setWorld(b2World * world)
{
    this->pHworld = world;
}

b2Body * Physics::getpHbody()
{
    return this->pHbody;
}

void Physics::setShape(float x, float y, b2PolygonShape shape)
{
    this->x = x;
    this->y = y;
    this->shape.append(shape);
}

void Physics::setShape(float x, float y, sf::ConvexShape shape)
{
    b2PolygonShape tob2shape;
    b2Vec2 verts[shape.getPointCount()];
    for(int i = 0; i < shape.getPointCount(); ++i)
        // TODO: 30.f -> SCALE
        verts[i] = b2Vec2(shape.getPoint(i).x/30.f, shape.getPoint(i).y/30.f);

    tob2shape.Set(verts, shape.getPointCount());
    this->setShape(x,y,tob2shape);
}

QList<b2PolygonShape> &Physics::getShapeList()
{
    b2Fixture * fixtures = this->pHbody->GetFixtureList();
    this->shape.clear();
    while(fixtures != NULL)
    {
        b2PolygonShape poly = (b2PolygonShape&)*fixtures->GetShape();
        this->shape.append(poly);

        fixtures = fixtures->GetNext();
    }

    return this->shape;
}


void Physics::setType(B2_BODY_TYPE type)
{
    this->type = (b2BodyType)type;
}

float Physics::getGravity()
{
    return this->pHworld->GetGravity().y;
}

bool Physics::isContactDown()
{
    for (b2ContactEdge *edge = this->getpHbody()->GetContactList(); edge; edge = edge->next)
    {
        b2WorldManifold worldManifold;
        edge->contact->GetWorldManifold(&worldManifold);

        if (worldManifold.normal.y > 0.75)
        {
            return true;
        }
    }
    return false;
}

bool Physics::isContactRight()
{
    bool flag = false;
    b2WorldManifold worldManifold;
    for (b2ContactEdge *edge = this->getpHbody()->GetContactList(); edge; edge = edge->next)
    {
        edge->contact->GetWorldManifold(&worldManifold);
        if (worldManifold.normal.x > 0.9) flag = true;
      //  qDebug()<<worldManifold.normal.y<<pHbody->GetPosition().x;
    }

    return flag;
}

bool Physics::isContactLeft()
{
    bool flag = false;
    b2WorldManifold worldManifold;
    for (b2ContactEdge *edge = this->getpHbody()->GetContactList(); edge; edge = edge->next)
    {
        edge->contact->GetWorldManifold(&worldManifold);
        if (worldManifold.normal.x < -0.9) flag = true;
    }
    return flag;
}

void Physics::begin(b2Contact *contact)
{
    b2WorldManifold man;
    contact->GetWorldManifold(&man);
    //if (man.normal.y > 0.75)
    if (isContactDown())
        emit this->landing();
}

void Physics::end(b2Contact *contact)
{
    b2WorldManifold man;
    contact->GetWorldManifold(&man);
    //if (man.normal.y < 0.75)
    if (!isContactDown())
        emit this->jumping();
}

//void Physics::BeginContact(b2Contact *contact)
//{
//    qDebug()<<111;
//}

//void Physics::EndContact(b2Contact *contact)
//{
//    qDebug()<<222;
//}

void Physics::createBody(void * data, float SCALE)
{
    this->data = data;
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(this->x/SCALE, this->y/SCALE);
    BodyDef.type = this->type;
    BodyDef.fixedRotation = true;
    this->pHbody = this->pHworld->CreateBody(&BodyDef);
  //  this->pHbody->SetUserData(data);
    this->pHbody->SetUserData(this);

    b2FixtureDef FixtureDef;
    if (!this->shape.isEmpty())
    {
        for (QList<b2PolygonShape>::iterator i = this->shape.begin(); i != this->shape.end(); i++)
        {
            FixtureDef.density = this->density;
            FixtureDef.friction = this->friction;
            FixtureDef.shape = &(*i);
            FixtureDef.restitution = 0;
            this->pHbody->CreateFixture(&FixtureDef);
        }
    }
}
