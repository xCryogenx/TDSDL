#include "app.h"

App::App()
{
    this->screen = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "SFML window", sf::Style::Close);
    this->control = new Control;
    connect(this->control, SIGNAL(moveCamera(bool,bool,bool,bool)), this, SLOT(moveCamera(bool,bool,bool,bool)));
    connect(this->control, SIGNAL(end()), this, SLOT(Close()));
    this->_images   = new Resources<sf::Image>();
    this->_sprites  = new Resources<sf::Sprite>();
    this->_entities = new Resources<Entity>();
    this->_anims    = new Resources<Animation>();
    this->_cameras  = new Resources<sf::View>();

    cam_up    = false;
    cam_down  = false;
    cam_left  = false;
    cam_right = false;
//    this->_maps     = new Resources<Map>();
//    this->_texts    = new Resources<sf::Text>();
}

// Инициализация окна и связанных параметров
bool App::Init()
{
    sf::View *camera = new sf::View();
    this->mainCamera = camera;
    this->_cameras->add(camera, "default");

    camera->SetFromRect(sf::FloatRect(0,0,800,600));
    this->screen->SetView(*camera);


    // Выделяем память под спрайт и не забываем выгрузить в деструкторе Entity
    sf::Image *image = new sf::Image();
    if (!image->LoadFromFile("images/abcd.png")) return false;
    _images->add(image,"ani_dragon_runRight");

    image = new sf::Image();
    if (!image->LoadFromFile("images/abcde.png")) return false;
    _images->add(image,"ani_dragon_runLeft");

    // Добавляем спрайт в ресурсы
    sf::Sprite *spr = new sf::Sprite(*_images->getRes("ani_dragon_runRight"));
    this->_sprites->add(spr, "ani_dragon_runRight");

    Animation * anim = new Animation(this->_sprites->getRes("ani_dragon_runRight"), 8, 75, 0);
    this->_anims->add(anim, "ani_dragon_runRight");

    Entity *ent = new Entity;
    ent->addAnim(this->_anims->getRes("ani_dragon_runRight"), "runRight");
    ent->setAnim("runRight");
    this->_entities->add(ent,"enemy_Dragon");

//    Sprite * spr_ground1 = new Sprite;
//    spr_ground1->Load("images/green.png");
//    Sprite * spr_ground2 = new Sprite;
//    spr_ground2->Load("images/road.png");
//    Sprite * spr_ground3 = new Sprite;
//    spr_ground3->Load("images/water.png");

//    Sprite * animSpr = new Sprite;
//    animSpr->Load("images/abcd.png");
//    Animation * anim = new Animation(animSpr, 8, 75, 0);


//    this->_sprites->add(spr_ground1, "green");
//    this->_sprites->add(spr_ground2, "road");
//    this->_sprites->add(spr_ground3, "water");
//    this->_anims->add(anim, "run");


/*
    //считывание карты и создание сущностей земли
    this->readMap("maps/map.txt");

    //вот здесь содержится то, что хорошо бы упаковать в класс e_Ground
    //здесь распределение картинок в зависимости от типа земли
    for (int y=0; y<this->_maps->getRes("Ground")->getHeight(); y++)
    {
        for (int x=0; x<this->_maps->getRes("Ground")->getWidth(); x++)
        {
            e_Ground *ground = new e_Ground;
            switch (this->_maps->getRes("Ground")->getType(x,y))
            {
                case 0:
                {
                    ground->setSprite(this->_sprites->getRes("water"));
                    break;
                }
                case 1:
                {
                    ground->setSprite(this->_sprites->getRes("road"));
                    break;
                }
                case 2:
                {
                    ground->setSprite(this->_sprites->getRes("green"));
                    break;
                }
            }
            ground->setXY(x*60,y*60)->setHW(60,60);
            this->_entities->add(ground,QString::number(x)+","+QString::number(y));
            ground = NULL;
        }
    }

    this->control.setCamera(this->mainCamera);

    Text *text = new Text("Дракоша", "font/univers.ttf", 25, 0, -50, -25, 255, 0, 0, 0, 0, false);
    this->_texts->add(text, "text_drakoXY");
*/
    return true;
}

// Запуск приложения
int App::Execute()
{
    if (!this->Init())
    {
        printf("Error while init window");
        return -1;
    }

    sf::Event event;
    while (this->screen->IsOpened())
    {
        freq = screen->GetFrameTime();
        while (this->screen->GetEvent(event))
        {
            this->Event(&event);
        }

        this->Loop();

        this->Render();
    }

    this->Cleanup();
    return 0;
}

// Функция обрабатывает все поступающие события от мышки, клавиатуры, джойстика и других устройств.
void App::Event(sf::Event *event)
{
    control->doControl(event);
}

// Функция обрабатывает обновление данных, например движение NPC по экрану, уменьшение здоровье персонажа и так далее.
void App::Loop()
{
    mainCamera->Move(1000 * (cam_right * freq - cam_left * freq), 1000 * (cam_down * freq - cam_up * freq));
    float x = _entities->getRes("enemy_Dragon")->getX() + 100 * freq;
    float y = _entities->getRes("enemy_Dragon")->getY() + 100 * freq;
    this->_entities->getRes("enemy_Dragon")->setXY(x,y);
//    this->_texts->getRes("text_drakoXY")->setXY(x-30,y-50)->setText("x: "+QString::number(x)+", y: "+QString::number(y));
}

// Функция занимается отображением всего на экране. Она НЕ обрабатывает манипуляции с данными - этим занимается Loop.
void App::Render()
{
    // Clear screen
    this->screen->Clear();

    // Выведем все ресурсы из Entity
    QMap<QString, Entity*>::iterator i;
    for (i = _entities->getBegin(); i != _entities->getEnd(); ++i)
    {
        (*i)->refresh(this->screen);
    }

    // Update the window
    this->screen->Display();

//    // Выведем все ресурсы из Text
//    QMap<QString, Text*>::iterator t;
//    for (t = _texts->getBegin(); t != _texts->getEnd(); ++t)
//    {
//        if (((*t)->getX() - this->mainCamera->getX() >= 0) && ((*t)->getY() + (*t)->getSize() - this->mainCamera->getY() >= 0)
//                && ((*t)->getX() - this->mainCamera->getX() <= this->screen->w) && ((*t)->getY() - this->mainCamera->getY() <= this->screen->h))
//        {
//            (*t)->setXY((*t)->getX() - this->mainCamera->getX(), (*t)->getY() - this->mainCamera->getY());
//            (*t)->refresh(this->screen);
//            (*t)->setXY((*t)->getX() + this->mainCamera->getX(), (*t)->getY() + this->mainCamera->getY());
//        }
//        //printf("%s",(*i)->get_name());
//    }
//    // Обновление экрана
//    SDL_Flip(this->screen);
}

// Функция просто отключает все использующиеся ресурсы и закрывает игру.
void App::Cleanup()
{
    delete this->screen;
    delete this->_sprites;
    delete this->_images;

    delete this->_anims;
    delete this->_entities;
    //delete this->_maps;
    delete this->_cameras;
    //delete this->_texts;
}

void App::Close()
{
    this->screen->Close();
}

void App::moveCamera(bool up, bool down, bool left, bool right)
{
    if (up)    cam_up    = !cam_up;
    if (down)  cam_down  = !cam_down;
    if (left)  cam_left  = !cam_left;
    if (right) cam_right = !cam_right;
}



//void App::readMap(QString path)
//{
//    QFile file(path);
//    if (!file.open(QIODevice::ReadOnly))
//    {
//        printf("Unable to open file\n");
//        return;
//    }
//    QTextStream in(&file);
//    // а вот то, что в комментах, нужно будет для бинарников
//    //QDataStream in(&file);
//    //in.setVersion(QDataStream::Qt_4_3);

//    int width, height;
//    in >> width;
//    in >> height;

//    // Инициализация карт и добавление их в ресурсы
//    Map *map1 = new Map(width,height);
//    Map *map2 = new Map(width,height);
//    Map *map3 = new Map(width,height);

//    this->_maps->add(map1,"Ground");
//    this->_maps->add(map2,"Enemy");
//    this->_maps->add(map3,"Tower");

//    int type;
//    for (int y=0; y<height; y++)
//    {
//        for (int x=0; x<width; x++)
//        {
//            in >> type;
//            map1->setCell(x,y,type);
//        }
//    }
//}
