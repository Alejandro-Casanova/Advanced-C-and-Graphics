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
                            std::vector<Zombie*>& zombies) override;

    protected:

    private:
        Bengine::InputManager* _inputManager;
        std::vector<Gun*> _guns;
        int _currentGun;
        Bengine::Camera2D* _camera;
        std::vector<Bullet>* _bullets;
};

#endif // PLAYER_H
