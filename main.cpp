#include <SFML/Graphics.hpp>
#include <SFML/system.hpp>
#include <SFML/window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <math.h>
#include <ctime>
#include <vector>
#include <string>
#include <vector>
#include <utility>
using namespace std;
using namespace sf;

struct Bone {
    Sprite sprite;
    bool isVisible = false;
};

float calcuateThrowSpeedFromHoldingTime(float time) {
    return -(100.f * time);
}

int main() {
    srand(0);
    RenderWindow window(VideoMode(1210, 680), "cat vs dog");

    // Constants
    int HIGHT_THRESHOLD = 200;

    //background
    Texture backgroundTexture;
    backgroundTexture.loadFromFile("background2.png");
    Sprite background(backgroundTexture);
    background.setScale(0.9, 0.9);

    //wall
    Texture wallTexture;
    wallTexture.loadFromFile("wall2.png");
    Sprite wall(wallTexture);
    wall.setPosition(615, 310);
    wall.setOrigin(wall.getGlobalBounds().width / 2, wall.getGlobalBounds().height / 2);
    wall.setScale(0.9, 0.9);

    //cat
    Texture catTexture;
    catTexture.loadFromFile("cat.png");
    Sprite cat(catTexture);
    cat.setPosition(460, 520);
    cat.setOrigin(cat.getGlobalBounds().width / 2, cat.getGlobalBounds().height / 2);
    cat.setScale(0.7, 0.7);
    cat.setTextureRect(IntRect(0, 0, 228, 155));

    //dog
    Texture dogTexture;
    dogTexture.loadFromFile("dog.png");
    Sprite dog(dogTexture);
    dog.setPosition(1520, 600);
    dog.setOrigin(dog.getGlobalBounds().width / 2, dog.getGlobalBounds().height / 2);
    dog.setScale(0.7, 0.7);
    dog.setTextureRect(IntRect(0, 0, 328.6, 224));
    Vector2u texturesize = dogTexture.getSize();
    texturesize.x /= 5;
    texturesize.y /= 2;
    int throwSpeed = 0;

    //cat health
    RectangleShape cathpBack(Vector2f(120, 50));
    cathpBack.setSize(Vector2f(500.f, 20.f));
    cathpBack.setFillColor(Color(50, 50, 50, 200));
    cathpBack.setPosition(20.f, 20.f);


    RectangleShape cathpInner[5];
    for (size_t i = 0; i < 5; i++)
    {

        cathpInner[i].setSize(Vector2f(100.f, 20.f));
        cathpInner[i].setFillColor(Color(250, 20, 20, 200));
        cathpInner[i].setPosition(20 + (i * 100), 20);
    }




    //dog health

    RectangleShape doghpBack(Vector2f(120, 50));
    doghpBack.setSize(Vector2f(500.f, 20.f));
    doghpBack.setFillColor(Color(50, 50, 50, 200));
    doghpBack.setPosition(680.f, 20.f);




    RectangleShape doghpInner[5];
    for (size_t i = 0; i < 5; i++)
    {

        doghpInner[i].setSize(Vector2f(100.f, 20.f));
        doghpInner[i].setFillColor(Color(250, 20, 20, 200));
        doghpInner[i].setPosition(680 + (i * 100), 20);
    }

    //bone
    Texture boneTexture;
    boneTexture.loadFromFile("bone.png");

    Bone bone{};
    bone.sprite.setTexture(boneTexture);
    bone.sprite.setPosition(950, 450);
    bone.sprite.setScale(0.7, 0.7);

    //rubbish
    Texture rubbishTexture;
    rubbishTexture.loadFromFile("rubbish.png");
    Sprite rubbish(rubbishTexture);
    rubbish.setPosition(110, 450);
    rubbish.setScale(0.7, 0.7);



    //time
    Clock clock, deltaTimeClock, dogAnimationClock, catClock;

    int i = 0;

    int randomTime = (rand() % 3);

    int time = 0;
    bool isPressing = false;
    int hight = 0;
    float DeltaTime = 0;

    int dogAnimationIndecator = 0;

    bool isDogTurn = true, canPlay = true;

    //PAUSE
    RectangleShape pause(Vector2f(100, 100));
    pause.setPosition(550, 0);
    Texture tx;
    tx.loadFromFile("pause4.png");
    pause.setTexture(&tx);


    //CONTINUE
    RectangleShape Continue(Vector2f(350, 250));
    Continue.setPosition(400, 200);
    Texture tx2;
    tx2.loadFromFile("continue2.png");
    Continue.setTexture(&tx2);

    
    bool game = true;

    // cat pain sound
    SoundBuffer soundBuffer_cat;
    Sound sound_cat;
    if (!soundBuffer_cat.loadFromFile("cat2.wav"))
    {
        cout << " i cant find cat2.wav " << endl;
    }
    sound_cat.setBuffer(soundBuffer_cat);


    // DOG pain sound
    SoundBuffer soundBuffer_dog;
    Sound sound_dog;
    if (!soundBuffer_dog.loadFromFile("dog2.wav"))
    {
        cout << " i cant find dog2.wav " << endl;
    }
    sound_dog.setBuffer(soundBuffer_dog);


    //cat laughing sound
    SoundBuffer soundBuffer_laughcat;
    Sound sound_laughcat;
    if (!soundBuffer_laughcat.loadFromFile("cat laugh.wav"))
    {
        cout << " i cant find cat2.wav " << endl;
    }
    sound_laughcat.setBuffer(soundBuffer_laughcat);

    //dog laughing sound
    SoundBuffer soundBuffer_laughdog;
    Sound sound_laughdog;
    if (!soundBuffer_laughdog.loadFromFile("dog laugh.wav"))
    {
        cout << " i cant find dog2.wav " << endl;
    }
    sound_laughdog.setBuffer(soundBuffer_laughdog);



    // hit sound
    SoundBuffer soundBuffer_hit;
    Sound sound_hit;
    if (!soundBuffer_hit.loadFromFile("hit.wav"))
    {
        cout << " i cant find hit.wav " << endl;
    }
    sound_hit.setBuffer(soundBuffer_hit);

    // game sound
    SoundBuffer soundBuffer_game;
    Sound sound_game;
    if (!soundBuffer_game.loadFromFile("game2.wav"))
    {
        cout << " i cant find game2.wav " << endl;
    }
    sound_game.setBuffer(soundBuffer_game);
    sound_game.play();

    while (window.isOpen()) {

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {

                Vector2i mousepos = Mouse::getPosition(window);



                if (mousepos.x > 568 && mousepos.x < 632 && mousepos.y>16 && mousepos.y < 81)
                {

                    game = false;
                }
                

                if (mousepos.x > 412 && mousepos.x < 740 && mousepos.y >265 && mousepos.y < 385)
                {
                    game = true;

                }



            }
            if (game == true)
            {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && isDogTurn) {
                    bone.isVisible = true;
                    isPressing = true;
                    clock.restart();
                }

                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && isDogTurn) {
                    isPressing = false;
                    time = clock.getElapsedTime().asSeconds();
                    throwSpeed = calcuateThrowSpeedFromHoldingTime(time);
                    canPlay = false;
                    dogAnimationIndecator = 3;
                    dogAnimationClock.restart();
                }
            }
        }

        
          if (isPressing)
          {
            if (dogAnimationClock.getElapsedTime().asSeconds() > 1.f)
                dogAnimationIndecator = 2;
            else
                dogAnimationIndecator = 1;
          }

            if (canPlay && !isDogTurn)
            {
               if (catClock.getElapsedTime().asSeconds() > 3.f)
               {
                time = (rand() % 3) + 1;
                throwSpeed = calcuateThrowSpeedFromHoldingTime(time);
               }
            }

        dog.setTextureRect(IntRect(texturesize.x * dogAnimationIndecator, texturesize.y * 0, texturesize.x, texturesize.y));

        if (throwSpeed != 0)
        {
            if (isDogTurn)
                bone.sprite.move(-500 * DeltaTime, 5 * DeltaTime * throwSpeed);
            else
                rubbish.move(500 * DeltaTime, 5 * DeltaTime * throwSpeed);
        }

        if (bone.sprite.getPosition().y <= HIGHT_THRESHOLD)
        {
            throwSpeed *= -1;
        }


        if (bone.sprite.getPosition().y >= 690)
        {
            throwSpeed = 0;
            canPlay = true;
            dogAnimationIndecator = 0;
            catClock.restart();
            if (!isDogTurn)
            {
                isDogTurn = true;
                bone.sprite.setPosition(950, 450);
            }
            else
            {
                rubbish.setPosition(120, 450);
            }
            isDogTurn = false;

        
            
            for (size_t i = 0; i < 5; i++)
            {
                if (bone.sprite.getGlobalBounds().intersects(dog.getGlobalBounds()))
                {
                    doghpInner[i].setSize(Vector2f(0, 0));

                }
            }

            for (size_t i = 0; i < 5; i++)
            {
                if (bone.sprite.getGlobalBounds().intersects(cat.getGlobalBounds()))
                {
                    cathpInner[4 - i].setSize(Vector2f(0, 0));

                }
            }



        }
        if (game == true)
        {

            if (Keyboard::isKeyPressed(Keyboard::Key::P))
            {
                sound_hit.play();
                sound_cat.play();
                sound_laughdog.play();
            }
            //if (Keyboard::isKeyPressed(Keyboard::Key::W))


            //if (Keyboard::isKeyPressed(Keyboard::Key::K))


            if (Keyboard::isKeyPressed(Keyboard::Key::S))
            {
                sound_hit.play();
                sound_dog.play();
                sound_laughcat.play();
            }



        }



        window.clear();
        window.draw(background);
        window.draw(wall);
        window.draw(dog);
        window.draw(cat);
        window.draw(bone.sprite);
        window.draw(rubbish);


        window.draw(cathpBack);
        for (size_t i = 0; i < 5; i++)
        {
            window.draw(cathpInner[i]);

        }

        window.draw(doghpBack);
        for (size_t i = 0; i < 5; i++)
        {
            window.draw(doghpInner[i]);

        }



        window.draw(pause);
        if (game == false)
        {
            window.draw(Continue);
        }
        

        DeltaTime = deltaTimeClock.restart().asSeconds();
        window.display();

    }
    return 0;
}
