#include <ostream>
#include "glm_helper.h"

glm::ivec3 glmc_to_glm_ivec3(t_ivec3 orig) {
    return glm::ivec3(orig[0], orig[1], orig[2]);
}

glm::vec3 glmc_to_glm_ivec(t_vec3 orig) {
    return glm::vec3(orig[0], orig[1], orig[2]);
}

void glm_to_glmc_ivec3(glm::ivec3 orig, t_ivec3 target) {
    target[0] = orig.x;
    target[1] = orig.y;
    target[2] = orig.z;
}

void glm_to_glmc_vec3(glm::vec3 orig, t_vec3 target) {
    target[0] = orig.x;
    target[1] = orig.y;
    target[2] = orig.z;
}

std::ostream& operator<<(std::ostream& target, const glm::ivec2& source)
{
    target << "glm::ivec2(" << source.x << "," << source.y << ")";
    return target;
}

std::ostream& operator<<(std::ostream& target, const glm::vec2& source)
{
    target << "glm::vec2(" << source.x << "," << source.y << ")";
    return target;
}

std::ostream& operator<<(std::ostream& target, const glm::ivec3& source)
{
    target << "glm::ivec3(" << source.x << "," << source.y << "," << source.z << ")";
    return target;
}

std::ostream& operator<<(std::ostream& target, const glm::vec3& source)
{
    target << "glm::vec3(" << source.x << "," << source.y << "," << source.z << ")";
    return target;
}

std::ostream& operator<<(std::ostream& target, const glm::ivec4& source)
{
    target << "glm::ivec4(" << source.x << "," << source.y << "," << source.z << "," << source.w << ")";
    return target;
}

std::ostream& operator<<(std::ostream& target, const glm::vec4& source)
{
    target << "glm::vec4(" << source.x << "," << source.y << "," << source.z << "," << source.w << ")";
    return target;
}
