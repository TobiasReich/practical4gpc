#ifndef PLAYER_H
#define PLAYER_H

/** Simple convenience class for storing player
attributes like position, rotation, size and speed...**/
class Player{
    public:
        Player();
        virtual ~Player();

        // Player position and orientation
        float eyeX = 10;
        float eyeY = 1.5f;
        float eyeZ = -20;
        float atX, atY, atZ = 0;
        float upX = 0;
        float upY = 1;
        float upZ = 0;

        float rotLeftRight = 0;             // Rotation at HORIZONTAL Axis
        float rotUpDown = 0;                // Rotation at VERTICAL Axis

        float lightIntensity = 5.0;         // multiplicator for light intensity
        bool playerLight = true;            // player light on

        static const float WALKING_SPEED;    // Walking speed
        static const float ROTATION_SPEED;   // Walking speed
        static const float PLAYER_SIZE;      // Player Size for collision calculation

        void setPlayerToStartCoords(void);
        void setPlayerLight(void);
    protected:
    private:
};

#endif // PLAYER_H
