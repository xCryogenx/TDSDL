﻿#include "control.h"
#include <QDebug>

Control::Control()
{
}

Control::~Control()
{
}

void Control::doControl(sf::Event *event)
{
    switch (event->type)
    {
    case sf::Event::Closed:
    {
        end();
        break;
    }
    case sf::Event::KeyPressed:
    {
//        if (event->key.code == sf::Keyboard::Up)    emit setCamControl(1);
//        if (event->key.code == sf::Keyboard::Down)  emit setCamControl(3);
//        if (event->key.code == sf::Keyboard::Left)  emit setCamControl(5);
//        if (event->key.code == sf::Keyboard::Right) emit setCamControl(7);

        if (event->key.code == sf::Keyboard::Escape) emit end();
        if (event->key.code == sf::Keyboard::Up) emit setEntControl(Unit::JUMP);
        if (event->key.code == sf::Keyboard::Left) emit setEntControl(Unit::MOVE_LEFT);
        if (event->key.code == sf::Keyboard::Right) emit setEntControl(Unit::MOVE_RIGHT);

        break;
    }
    case sf::Event::KeyReleased:
    {
        if (event->key.code == sf::Keyboard::Left)  emit setEntControl(Unit::STOP_LEFT);
        if (event->key.code == sf::Keyboard::Right) emit setEntControl(Unit::STOP_RIGHT);

        break;
    }
    case sf::Event::MouseWheelMoved:
    {
        break;
    }
    case sf::Event::MouseMoved:
    {
        break;
    }
    case sf::Event::MouseButtonPressed:
    {
        int x = event->mouseButton.x;
        int y = event->mouseButton.y;
        emit createGround(x,y);
        break;
    }
    case sf::Event::MouseButtonReleased:
    {
        break;
    }
    }
}
