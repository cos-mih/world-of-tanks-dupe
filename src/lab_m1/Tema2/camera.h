#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"



namespace implemented
{
    class Camera
    {
    public:
        Camera()
        {
            position = glm::vec3(0, 2, 5);
            forward = glm::vec3(0, 0, -1);
            up = glm::vec3(0, 1, 0);
            right = glm::vec3(1, 0, 0);
            distanceToTarget = 13;
        }

        Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            Set(position, center, up);
        }

        ~Camera()
        { }

        // Update camera
        void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            this->position = position;
            forward = glm::normalize(center - position);
            right = glm::cross(forward, up);
            this->up = glm::cross(right, forward);
        }

        void SetForward(const glm::vec3& forw)
        {
            this->forward = forw;
            this->right = glm::cross(forw, this->up);
        }


        void MoveForward(float distance, glm::vec3 dir)
        {
            position += dir * distance;
        }

        void MoveWithPosition(glm::vec3 offset)
        {
            position += offset;
        }

        void TranslateForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward));
            position += dir * distance;
        }

        void TranslateUpward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(up));
            position += dir * distance;
        }

        void TranslateRight(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
            position += dir * distance;
        }

        void RotateFirstPerson_OY(float angle)
        {
            glm::vec4 newVectorF = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
            glm::vec4 newVectorU = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(up, 1);
            glm::vec4 newVectorR = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);

            forward = glm::normalize(glm::vec3(newVectorF));
            up = glm::normalize(glm::vec3(newVectorU));
            right = glm::normalize(glm::vec3(newVectorR));
        }

        void RotateThirdPerson_OY(float angle)
        {
           
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

      
        glm::mat4 GetViewMatrix()
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

    public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace implemented
