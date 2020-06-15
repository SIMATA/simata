#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
using namespace std;
using namespace sf;

// ������ �޸��� Ʈ������ ������� ����
mt19937 engine((unsigned int)time(NULL));

class Bullet
{
public:
    Sprite shape;

    Bullet(Texture *texture, Vector2f pos)
    {
        this->shape.setTexture(*texture); // ����� �ؽ�ó ����

        this->shape.setScale(0.3f, 0.3f); // ����� ũ�� ����

        this->shape.setPosition(pos); // ����� ��ġ ����
    }
    ~Bullet() {}
};

class User
{
public:
    Sprite shape;
    Texture *texture;

    int HP;
    int HPMax;

    vector<Bullet> bullets; // ���ñ������ �迭�����

    User(Texture *texture)
    {
        this->HPMax = 10;
        this->HP = this->HPMax;

        this->texture = texture;
        this->shape.setTexture(*texture);

        this->shape.setScale(0.1f, 0.1f);
    }
    ~User() {}
};

class Enemy
{
public:
    Sprite shape;
    int HP;
    int HPMax;

    Enemy(Texture *texture, Vector2u windowSize)
    {
        this->HPMax = engine() % 3 + 1;
        this->HP = this->HPMax;

        this->shape.setTexture(*texture);

        this->shape.setScale(0.3f, 0.3f);

        this->shape.setPosition(windowSize.x - this->shape.getGlobalBounds().width, engine() % (int)(windowSize.y - this->shape.getGlobalBounds().height)); // ©�� �κ� ���ֱ� ���� int�� ����
    }
    ~Enemy() {}
};

class Fever
{
public:
    Sprite shape;

    Fever(Texture *texture, Vector2u windowSize)
    {
        this->shape.setTexture(*texture);

        this->shape.setScale(0.3f, 0.3f);

        this->shape.setPosition(windowSize.x - this->shape.getGlobalBounds().width, engine() % (int)(windowSize.y - this->shape.getGlobalBounds().height)); // ©�� �κ� ���ֱ� ���� int�� ����
    }
    ~Fever() {}
};

class Heal
{
public:
    Sprite shape;

    Heal(Texture *texture, Vector2u windowSize)
    {
        this->shape.setTexture(*texture);

        this->shape.setScale(0.15f, 0.15f);

        this->shape.setPosition(windowSize.x - this->shape.getGlobalBounds().width, engine() % (int)(windowSize.y - this->shape.getGlobalBounds().height)); // ©�� �κ� ���ֱ� ���� int�� ����
    }
    ~Heal() {}
};

int main()
{

    // ���θ޴�

    RenderWindow window(VideoMode(800, 600), "GGANG");
    window.setFramerateLimit(60);

    // �������
    Music music;
    if (!music.openFromFile("things/GGANG.ogg"))
    {
        return -1;
    }
    music.setVolume(3.f);
    music.play();

    // ��� �ǰ� ����
    SoundBuffer buffer1;
    if (!buffer1.loadFromFile("things/BAT.ogg"))
    {
        return -1;
    }
    Sound enemySound;
    enemySound.setVolume(5.f);
    enemySound.setBuffer(buffer1);

    // ���� �ǰ� ����
    SoundBuffer buffer2;
    if (!buffer2.loadFromFile("things/ping_warning.ogg"))
    {
        return -1;
    }
    Sound userSound;
    userSound.setVolume(5.f);
    userSound.setBuffer(buffer2);

    // ���ӿ��� ����
    SoundBuffer buffer3;
    if (!buffer3.loadFromFile("things/GAMEOVER.ogg"))
    {
        return -1;
    }
    Sound gameoverSound;
    gameoverSound.setBuffer(buffer3);

    // �ǹ� ����
    SoundBuffer buffer4;
    if (!buffer4.loadFromFile("things/sonega.ogg"))
    {
        return -1;
    }
    Sound feverSound;
    feverSound.setBuffer(buffer4);

    // ���ڱ�(�� ����)
    SoundBuffer buffer5;
    if (!buffer5.loadFromFile("things/summoner_heal.ogg"))
    {
        return -1;
    }
    Sound recoverSound;
    recoverSound.setVolume(5.f);
    recoverSound.setBuffer(buffer5);

    // ��Ʈ ����
    Font font;
    font.loadFromFile("things/BMDOHYEON_ttf.ttf");

    //�ؽ��� ����
    Texture userTex;
    userTex.loadFromFile("images/bee.PNG");

    Texture enemyTex;
    enemyTex.loadFromFile("images/rain.PNG");

    Texture bulletTex;
    bulletTex.loadFromFile("images/seawoo.png");

    Texture feverTex;
    feverTex.loadFromFile("images/chip.PNG");

    Texture healTex;
    healTex.loadFromFile("images/potato.PNG");

    // ����
    Text scoreTex;
    scoreTex.setFont(font);
    scoreTex.setCharacterSize(20);
    scoreTex.setFillColor(Color::White);
    scoreTex.setPosition(10.f, 10.f);
    int score = 0;

    // �÷��̾� ����
    User user(&userTex);
    int shootTimer = 20;
    Text hpText;
    hpText.setFont(font);
    hpText.setCharacterSize(12);
    hpText.setFillColor(Color::White);

    // ��(��) ����
    vector<Enemy> enemies;
    int enemyspawnTimer = 0;
    enemies.push_back(Enemy(&enemyTex, window.getSize()));
    Text ehpText;
    ehpText.setFont(font);
    ehpText.setCharacterSize(12);
    ehpText.setFillColor(Color::White);

    // ����� �ǹ� ����
    vector<Fever> fevers;
    int feverspawnTimer = 0;
    fevers.push_back(Fever(&feverTex, window.getSize()));

    // ���ڱ� �� ����
    vector<Heal> heals;
    int healspawnTimer = 0;
    heals.push_back(Heal(&healTex, window.getSize()));

    // �Ѿ� ����� �ʱ�ȭ
    int bullet_frequency = 40;

    // ���� ����
    Text gameoverText;
    gameoverText.setFont(font);
    gameoverText.setCharacterSize(20);
    gameoverText.setFillColor(Color::Red);
    gameoverText.setPosition(100.f, window.getSize().y / 2);
    gameoverText.setString("GAME OVER!!");

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }

        ///////////////////////////////////////////////////////////////////////////// �ǹ������ ��  //////////////////////////////////////////////
        if (user.HP > 0 && bullet_frequency == 30)
        {

            // User �����̱�
            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                user.shape.move(0.f, -5.f);
            }
            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                user.shape.move(-5.f, 0.f);
            }
            if (Keyboard::isKeyPressed(Keyboard::S))
            {
                user.shape.move(0.f, 5.f);
            }
            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                user.shape.move(5.f, 0.f);
            }
            // user HP, HPMax
            hpText.setPosition(user.shape.getPosition().x, user.shape.getPosition().y - hpText.getGlobalBounds().height);
            hpText.setString(to_string(user.HP) + " / " + to_string(user.HPMax));

            //  �����쿡 �ε�����
            if (user.shape.getPosition().x <= 0) // ������ �Ѿ ���
            {
                user.shape.setPosition(0.f, user.shape.getPosition().y);
            }
            if (user.shape.getPosition().x >= window.getSize().x - user.shape.getGlobalBounds().width) // �������� �Ѿ ���
            {
                user.shape.setPosition(window.getSize().x - user.shape.getGlobalBounds().width, user.shape.getPosition().y); // GlobalBounds�� bounding box�� �����մϴ�.
            }                                                                                                                // ���� �ε����� �ٿ���ϴ� ���� �������ν� �ٿ������ �ʰ� �߽��ϴ�.
            if (user.shape.getPosition().y <= 0)                                                                             // �Ʒ����� �Ѿ ���
            {
                user.shape.setPosition(user.shape.getPosition().x, 0.f);
            }
            if (user.shape.getPosition().y >= window.getSize().y - user.shape.getGlobalBounds().height) // ������ �Ѿ ���
            {
                user.shape.setPosition(user.shape.getPosition().x, window.getSize().y - user.shape.getGlobalBounds().height);
            }

            if (shootTimer < bullet_frequency)
            {
                shootTimer++;
            }

            // ������Ʈ �κ�
            if (Mouse::isButtonPressed(Mouse::Left) && shootTimer >= bullet_frequency) // <- �̺κе� �� ���� �������
            {
                user.bullets.push_back(Bullet(&bulletTex, user.shape.getPosition()));
                shootTimer = 0;
            }

            // �Ѿ� ������ ����� ������
            for (size_t i = 0; i < user.bullets.size(); i++)
            {
                // �Ѿ� ������ �ӵ�---------��
                user.bullets[i].shape.move(10.f, 0.f);

                if (user.bullets[i].shape.getPosition().x > window.getSize().x)
                {
                    user.bullets.erase(user.bullets.begin() + i);
                    break;
                }

                // ������̶� ���̶� �ε�����
                for (size_t k = 0; k < enemies.size(); k++)
                {
                    if (user.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))
                    {

                        if (enemies[k].HP <= 1)
                        {
                            enemySound.play();
                            score += enemies[k].HPMax; // ���� ü�¸�ŭ ���� ȹ��
                            enemies.erase(enemies.begin() + k);
                        }
                        else
                        {
                            enemies[k].HP--;
                        }
                        user.bullets.erase(user.bullets.begin() + i);
                        break;
                    }
                }
            }

            // �� �������� �̵� �� ���� �ε�����
            if (enemyspawnTimer < 10)
            {
                enemyspawnTimer++;
            }
            // �� ����
            if (enemyspawnTimer >= 10)
            {
                enemies.push_back(Enemy(&enemyTex, window.getSize()));
                enemyspawnTimer = 0;
            }

            // ���� ������ �ǰ����� ���� �� �ӵ� ����
            for (size_t i = 0; i < enemies.size(); i++)
            { // �� �ӵ�-------------��
                enemies[i].shape.move(-8.f, 0.f);

                if (enemies[i].shape.getPosition().x <= 0 - enemies[i].shape.getGlobalBounds().width)
                {
                    enemies.erase(enemies.begin() + i);
                    break;
                }

                if (enemies[i].shape.getGlobalBounds().intersects(user.shape.getGlobalBounds()))
                {
                    bullet_frequency = 40;
                    feverSound.stop();
                    userSound.play();
                    enemies.erase(enemies.begin() + i);

                    user.HP--; // ü�� ����

                    if (user.HP == 0) // ü�� 0 �Ǹ� �� ��ħ�մϴ�.
                    {
                        gameoverSound.play();
                    }
                    break;
                }
            }

            // ������ǹ� �������� �̵�
            if (feverspawnTimer < 800)
            {
                feverspawnTimer++;
            }
            // ����� �ǹ� ����
            if (feverspawnTimer >= 800)
            {
                fevers.push_back(Fever(&feverTex, window.getSize()));
                feverspawnTimer = 0;
            }

            for (size_t i = 0; i < fevers.size(); i++)
            {
                fevers[i].shape.move(-3.f, 0.f);

                if (fevers[i].shape.getPosition().x <= 0 - fevers[i].shape.getGlobalBounds().width)
                {
                    fevers.erase(fevers.begin() + i);
                    break;
                }

                if (fevers[i].shape.getGlobalBounds().intersects(user.shape.getGlobalBounds()))
                {

                    feverSound.play();
                    fevers.erase(fevers.begin() + i);
                    bullet_frequency = 30;
                    break;
                }
            }

            // ���ڱ� (��) ����
            if (healspawnTimer < 700)
            {
                healspawnTimer++;
            }
            if (healspawnTimer >= 700)
            {
                heals.push_back(Heal(&healTex, window.getSize()));
                healspawnTimer = 0;
            }
            for (size_t i = 0; i < heals.size(); i++)
            {
                heals[i].shape.move(-4.f, 0.f);

                if (heals[i].shape.getPosition().x <= 0 - heals[i].shape.getGlobalBounds().width)
                {
                    heals.erase(heals.begin() + i);
                    break;
                }

                if (heals[i].shape.getGlobalBounds().intersects(user.shape.getGlobalBounds()))
                {

                    recoverSound.play();
                    heals.erase(heals.begin() + i);
                    if (user.HP == user.HPMax)
                    {
                        break;
                    }
                    if (user.HP < user.HPMax)
                    {
                        user.HP++;
                    }
                    break;
                }
            }
            // ����ǥ ���� ������Ʈ
            scoreTex.setString("Total Score : " + to_string(score));
        } // ü���� 1 ���Ϸ� �������� �� �� ������ ��������..

        ///////////////////////////////////////////////////////////////////////////// �ǹ���� �ƴ� �� //////////////////////////////////////////////

        if (user.HP > 0 && bullet_frequency == 40)
        {
            // User �����̱�
            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                user.shape.move(0.f, -5.f);
            }
            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                user.shape.move(-5.f, 0.f);
            }
            if (Keyboard::isKeyPressed(Keyboard::S))
            {
                user.shape.move(0.f, 5.f);
            }
            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                user.shape.move(5.f, 0.f);
            }
            // user HP, HPMax
            hpText.setPosition(user.shape.getPosition().x, user.shape.getPosition().y - hpText.getGlobalBounds().height);
            hpText.setString(to_string(user.HP) + " / " + to_string(user.HPMax));

            //  �����쿡 �ε�����
            if (user.shape.getPosition().x <= 0) // ������ �Ѿ ���
            {
                user.shape.setPosition(0.f, user.shape.getPosition().y);
            }
            if (user.shape.getPosition().x >= window.getSize().x - user.shape.getGlobalBounds().width) // �������� �Ѿ ���
            {
                user.shape.setPosition(window.getSize().x - user.shape.getGlobalBounds().width, user.shape.getPosition().y); // GlobalBounds�� bounding box�� �����մϴ�.
            }                                                                                                                // ���� �ε����� �ٿ���ϴ� ���� �������ν� �ٿ������ �ʰ� �߽��ϴ�.
            if (user.shape.getPosition().y <= 0)                                                                             // �Ʒ����� �Ѿ ���
            {
                user.shape.setPosition(user.shape.getPosition().x, 0.f);
            }
            if (user.shape.getPosition().y >= window.getSize().y - user.shape.getGlobalBounds().height) // ������ �Ѿ ���
            {
                user.shape.setPosition(user.shape.getPosition().x, window.getSize().y - user.shape.getGlobalBounds().height);
            }

            if (shootTimer < bullet_frequency)
            {
                shootTimer++;
            }

            // ������Ʈ �κ�
            if (Mouse::isButtonPressed(Mouse::Left) && shootTimer >= bullet_frequency) // <- �̺κе� �� ���� �������
            {
                user.bullets.push_back(Bullet(&bulletTex, user.shape.getPosition()));
                shootTimer = 0;
            }

            // �Ѿ� ������ ����� ������
            for (size_t i = 0; i < user.bullets.size(); i++)
            {
                // �Ѿ� ������ �ӵ�---------��
                user.bullets[i].shape.move(10.f, 0.f);

                if (user.bullets[i].shape.getPosition().x > window.getSize().x)
                {
                    user.bullets.erase(user.bullets.begin() + i);
                    break;
                }

                // ������̶� ���̶� �ε�����
                for (size_t k = 0; k < enemies.size(); k++)
                {
                    if (user.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))
                    {

                        if (enemies[k].HP <= 1)
                        {
                            enemySound.play();
                            score += enemies[k].HPMax; // ���� ü�¸�ŭ ���� ȹ��
                            enemies.erase(enemies.begin() + k);
                        }
                        else
                        {
                            enemies[k].HP--;
                        }
                        user.bullets.erase(user.bullets.begin() + i);
                        break;
                    }
                }
            }

            // �� �������� �̵� �� ���� �ε�����
            if (enemyspawnTimer < 10)
            {
                enemyspawnTimer++;
            }
            // �� ����
            if (enemyspawnTimer >= 10)
            {
                enemies.push_back(Enemy(&enemyTex, window.getSize()));
                enemyspawnTimer = 0;
            }

            // ���� ������ �ǰ����� ���� �� �ӵ� ����
            for (size_t i = 0; i < enemies.size(); i++)
            { // �� �ӵ�-------------��
                enemies[i].shape.move(-8.f, 0.f);

                if (enemies[i].shape.getPosition().x <= 0 - enemies[i].shape.getGlobalBounds().width)
                {
                    enemies.erase(enemies.begin() + i);
                    break;
                }

                if (enemies[i].shape.getGlobalBounds().intersects(user.shape.getGlobalBounds()))
                {
                    bullet_frequency = 40;
                    feverSound.stop();
                    userSound.play();
                    enemies.erase(enemies.begin() + i);

                    user.HP--; // ü�� ����

                    if (user.HP == 0) // ü�� 0 �Ǹ� �� ��ħ�մϴ�.
                    {
                        gameoverSound.play();
                    }
                    break;
                }
            }

            // ������ǹ� �������� �̵�
            if (feverspawnTimer < 800)
            {
                feverspawnTimer++;
            }
            // ����� �ǹ� ����
            if (feverspawnTimer >= 800)
            {
                fevers.push_back(Fever(&feverTex, window.getSize()));
                feverspawnTimer = 0;
            }

            for (size_t i = 0; i < fevers.size(); i++)
            {
                fevers[i].shape.move(-3.f, 0.f);

                if (fevers[i].shape.getPosition().x <= 0 - fevers[i].shape.getGlobalBounds().width)
                {
                    fevers.erase(fevers.begin() + i);
                    break;
                }

                if (fevers[i].shape.getGlobalBounds().intersects(user.shape.getGlobalBounds()))
                {

                    feverSound.play();
                    fevers.erase(fevers.begin() + i);
                    bullet_frequency = 30;
                    break;
                }
            }

            // ���ڱ� (��) ����
            if (healspawnTimer < 700)
            {
                healspawnTimer++;
            }
            if (healspawnTimer >= 700)
            {
                heals.push_back(Heal(&healTex, window.getSize()));
                healspawnTimer = 0;
            }
            for (size_t i = 0; i < heals.size(); i++)
            {
                heals[i].shape.move(-4.f, 0.f);

                if (heals[i].shape.getPosition().x <= 0 - heals[i].shape.getGlobalBounds().width)
                {
                    heals.erase(heals.begin() + i);
                    break;
                }
                if (heals[i].shape.getGlobalBounds().intersects(user.shape.getGlobalBounds()))
                {

                    recoverSound.play();
                    heals.erase(heals.begin() + i);
                    if (user.HP == user.HPMax)
                    {
                        break;
                    }
                    if (user.HP < user.HPMax)
                    {
                        user.HP++;
                    }
                    break;
                }
                //����ǥ ���� ������Ʈ
                scoreTex.setString("Total Score : " + to_string(score));
            }

        } // ü���� 1 ���Ϸ� �������� �� �� ������ ��������..

        /////////////////////////////////////////////////////////// �׸��� �κ� ///////////////////////////////////////////////////////////////////////////////
        window.clear();
        // �׸��� - user
        window.draw(user.shape);
        // �׸��� - �Ѿ�(�����)
        for (size_t i = 0; i < user.bullets.size(); i++)
        {
            window.draw(user.bullets[i].shape);
        }
        // �׸��� - ��(��)
        for (size_t i = 0; i < enemies.size(); i++)
        {
            ehpText.setString(to_string(enemies[i].HP) + " / " + to_string(enemies[i].HPMax));
            ehpText.setPosition(enemies[i].shape.getPosition().x + 20.0f, enemies[i].shape.getPosition().y - ehpText.getGlobalBounds().height - 4.0f);
            window.draw(ehpText);
            window.draw(enemies[i].shape);
        }
        // �׸��� - ����� �ǹ�
        for (size_t i = 0; i < fevers.size(); i++)
        {
            window.draw(fevers[i].shape);
        }
        // �׸��� - ���ڱ� ��
        for (size_t i = 0; i < heals.size(); i++)
        {
            window.draw(heals[i].shape);
        }
        // HP�� ����, ���ھ� ����
        window.draw(scoreTex);
        window.draw(hpText);

        // ���ӿ���
        if (user.HP <= 0)
        {
            music.stop();
            window.draw(gameoverText);
        }

        window.display();
    }

    return 0;
}
