#ifndef SYSPLEX_GLM_HELPER_H
#define SYSPLEX_GLM_HELPER_H

#include <ostream>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
extern "C" {
#include <glmc.h>
};

glm::ivec3 glmc_to_glm_ivec3(t_ivec3 orig);
glm::vec3 glmc_to_glm_ivec(t_vec3 orig);

void glm_to_glmc_ivec3(glm::ivec3 orig, t_ivec3 target);
void glm_to_glmc_vec3(glm::vec3 orig, t_vec3 target);

std::ostream& operator<<(std::ostream& target, const glm::ivec2& source);
std::ostream& operator<<(std::ostream& target, const glm::vec2& source);
std::ostream& operator<<(std::ostream& target, const glm::ivec3& source);
std::ostream& operator<<(std::ostream& target, const glm::vec3& source);
std::ostream& operator<<(std::ostream& target, const glm::vec4& source);
std::ostream& operator<<(std::ostream& target, const glm::ivec4& source);

#endif //SYSPLEX_GLM_HELPER_H
