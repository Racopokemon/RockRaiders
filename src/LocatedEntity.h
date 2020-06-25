#ifndef LOCATED_ENTITY_H
#define LOCATED_ENTITY_H

#include "SFML/Graphics.hpp"
#include <iostream>
#include "Entity.h"

#define PI 3.1415926535f
/*
 * Located entities extend the concept of entities: LocatedEntity have a position and a rotation, and already feature stubs to interpolate them during rendering. 
 */

 class LocatedEntity : public Entity {
    public :

        static sf::Vector2i toTile(sf::Vector2f v) {
            return sf::Vector2i((int)v.x, (int)v.y);
        }

        //!Call this *before* you continue with your calculations in the subclass! [The return value of this doesn't matter of course if you overwrite this]
        virtual void update() {
            lastPosition = position;
            lastRotation = rotation; 
        }
        virtual void draw(sf::RenderTarget &target, float delta) {
            float invDelta = 1.f-delta;
            sf::Transform trans;
            trans.translate(position*delta + lastPosition*invDelta);
            trans.rotate(rotation*delta + lastRotation*invDelta);
            draw(target, sf::RenderStates(trans), delta, invDelta);
        }

        /*!For the main purposes it succeeds to implement this draw method instead of the one from Entity: 
        * If you use the given RenderStates to draw, this already applies the right interpolated transformation so that you can 
        * assume the center of the LocatedEntity to be at 0,0
        */ 
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states, float delta, float invDelta) = 0;

        /*!If you choose not to interpolate, then the LocatedEntity starts interpolating from the given position (we update the lastPosition variable)
         * If you change the position afterwards, it interpolates to this new, changed position
         */
        virtual void setPosition(sf::Vector2f pos, bool interpolate = false) {
            position = pos;
            if (!interpolate) {
                lastPosition = pos;
            }
        }
        virtual sf::Vector2f getPosition() {
            return position;
        }
        //!Always interpolates. 
        virtual void changePosition(sf::Vector2f pos) {
            position += pos;
        }
        virtual void moveForward(float amount) {
            float rotationInRadians = rotation / 180 * PI;
            changePosition(sf::Vector2f(amount * sinf(rotationInRadians), amount * -cosf(rotationInRadians)));
        }

        //!Just a helper to keep the rotation value in the bounds from (-180, 180]. The trick is to also update the lastRotation value to not affect the interpolation. 
        virtual void boundRotation() {
            if (rotation <= -180.f) {
                do {
                    rotation += 360.f;
                    lastRotation += 360.f;
                } while (rotation <= -180.f);
            } else {
                while (rotation > 180.f) {
                    rotation -= 360.f;
                    lastRotation -= 360.f;
                }
            }
        }
        
        //!Analogous to setPosition. Interpolating is not recommended, since you have take care of the direction in that we rotate. 
        //Use the rotateTo instead to control the direction. 
        virtual void setRotation(float rot, bool interpolate = false) {
            rotation = rot;
            boundRotation();
            if (!interpolate) {
                lastRotation = rot;
            }
        }

        //!Always interpolates. If you rotate more than 360°, this will happen during the interpolation as well.
        virtual void changeRotation(float rot) {
            rotation += rot;
            boundRotation();
        }

        void boundRotationValue(float & rot) {
            if (rot > 180.f) {
                do {
                    rot -= 360.f;
                } while (rot > 180.f);
            } else {
                while (rot <= -180.f) {
                    rot += 360.f;
                }
            }
        }

        enum rotateToMode {clockwise, counterclockwise, nearest};
        virtual void rotateTo(float destinationRotation, rotateToMode mode) {
            boundRotationValue(destinationRotation);
            if (mode == nearest) {
                float normalizedDestination = destinationRotation - rotation;
                boundRotationValue(normalizedDestination);
                if (normalizedDestination > 0.f) {
                    mode = clockwise;
                } else {
                    mode = counterclockwise;
                }
            }
            if (destinationRotation-rotation > 0.f) {
                //By default interpolation would rotate clockwise
                //So we correct this in case the other direction is requested, and else we just apply the value
                if (mode == counterclockwise) {
                    lastRotation -= 360.f; //This is already a step further, instead of rotating 360° further, 
                    //we rotate the previous vaule back (this would theoretically allow the exact value to be set)
                }
            } else {
                if (mode == clockwise) {
                    lastRotation += 360.f;
                }
            }
            rotation = destinationRotation;
        }

        //!Returns the angle (in degrees, of course) of the given vector (so that it fits in our coordinate concept)
        static float pointAtAngle(sf::Vector2f diff) {
            return atan2(diff.x, -diff.y) / PI * 180.f;
        }
        //!Returns the angle (in degrees, of course) of the given vector (so that it fits in our coordinate concept)
        static float pointAtAngle(sf::Vector2f from, sf::Vector2f pointingAt) {
            return pointAtAngle(pointingAt-from);
        }

        //... well-known from Scratch xD
        void pointAt(sf::Vector2f target) {
            rotateTo(pointAtAngle(position, target), nearest);
        }

        //!Points at the target and moves forward to it by the given value. 
        //The clue: If we reach the target and possibly step even over it, not the whole
        //motion is applied but we end just exactly on the target. 
        //Returns true in exactly the case that we reached the target. 
        bool moveForwardTo(sf::Vector2f target, float distance) {
            pointAt(target);
            sf::Vector2f diff = target - position;
            if (hypotf(diff.x, diff.y) <= distance) {
                setPosition(target, true);
                return true; 
            } else {
                moveForward(distance);
                return false;
            }
        }

        virtual void prepareDeletion() {Entity::prepareDeletion();};

        sf::Vector2i getTile() {
            return toTile(position);
        }

        static sf::Vector2f getTileCenter(sf::Vector2i v) {
            return sf::Vector2f(v.x + 0.5f, v.y + 0.5f);
        }
        
        bool insideInnerTile(sf::Vector2i pos, float distance) {
            sf::Vector2f p = getPosition();
            sf::Vector2f tile = getTileCenter(pos);
            return abs(p.x-tile.x) <= distance && abs(p.y-tile.y) <= distance;
        }


    private : 
        //!The rotation values are stored in degrees, for convenience
        float rotation = 0.f, lastRotation = 0.f;
        sf::Vector2f position, lastPosition; 
};

#endif