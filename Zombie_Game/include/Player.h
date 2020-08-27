#ifndef PLAYER_H
#define PLAYER_H

#include "Human.h"
#include "Bullet.h"

#include <Bengine/include/InputManager.h>
#include <Bengine/include/Camera2D.h>

#include <vector>

class Gun;


class Player : public Human
{
    public:
        Player();
        virtual ~Player();

        void init(float speed, glm::vec2 pos, Bengine::InputManager* inputManager,
                                              Bengine::Camera2D* camera,
                                              std::vector<Bullet>* bullets);

        void addGun(Gun* gun);

        void update(const std::vector<std::string>& levelData,
                            std::vector<Human*>& humans,
                            std::vector<Zombie*>& zombies,
                            float deltaTime) override;

        bool receiveInfection(int infectVal) override;

        //const Gun* getGun(int index) const{return _guns[index];}
        const std::vector<Gun*>& getGuns() const {return m_guns;}
        const int getCurrentGun() const {return m_currentGun;}

    protected:

    private:
        Bengine::InputManager* m_inputManager;
        std::vector<Gun*> m_guns;
        int m_currentGun;
        Bengine::Camera2D* m_camera;
        std::vector<Bullet>* m_bullets;
};

#endif // PLAYER_H
