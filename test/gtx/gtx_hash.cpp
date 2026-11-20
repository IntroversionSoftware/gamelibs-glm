#include "glm_test.hpp"
#include <glm/detail/setup.hpp>

// __cplusplus is 199711L on MSVC without /Zc:__cplusplus, so ask GLM.
#if GLM_LANG & GLM_LANG_CXX11

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wfloat-equal"
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <unordered_map>

// Two different keys in an unordered_map: hashing a copy finds the same entry, and the keys stay apart.
template<typename T>
static int test_map(T const& A, T const& B)
{
    int Error = 0;

    std::unordered_map<T, int> Map;
    ++Map[A];
    ++Map[T(A)];
    ++Map[B];

    Error += GLM_CHECK_EQ(Map.size(), 2);
    Error += GLM_CHECK_EQ(Map.count(A), 1);
    Error += GLM_CHECK_EQ(Map.count(B), 1);
    Error += GLM_CHECK_EQ(Map.find(A) != Map.end() ? Map.find(A)->second : 0, 2);
    Error += GLM_CHECK_EQ(Map.find(B) != Map.end() ? Map.find(B)->second : 0, 1);
    Error += GLM_CHECK_EQ(std::hash<T>()(A), std::hash<T>()(T(A)));

    return Error;
}

int main()
{
    int Error = 0;

    // Vector types
    Error += GLM_TEST(test_map(glm::ivec1(0), glm::ivec1(1)));
    Error += GLM_TEST(test_map(glm::ivec2(0), glm::ivec2(1)));
    Error += GLM_TEST(test_map(glm::ivec3(0), glm::ivec3(1)));
    Error += GLM_TEST(test_map(glm::ivec4(0), glm::ivec4(1)));

    // Quaternion types
    Error += GLM_TEST(test_map(glm::quat(0.0f, glm::vec3(0.0f)), glm::quat(1.0f, glm::vec3(0.0f))));
    Error += GLM_TEST(test_map(glm::dualquat(glm::vec3(0.0f)), glm::dualquat(glm::vec3(1.0f))));

    // Matrix types
    Error += GLM_TEST(test_map(glm::mat2x2(0.0f), glm::mat2x2(1.0f)));
    Error += GLM_TEST(test_map(glm::mat2x3(0.0f), glm::mat2x3(1.0f)));
    Error += GLM_TEST(test_map(glm::mat2x4(0.0f), glm::mat2x4(1.0f)));
    Error += GLM_TEST(test_map(glm::mat3x2(0.0f), glm::mat3x2(1.0f)));
    Error += GLM_TEST(test_map(glm::mat3x3(0.0f), glm::mat3x3(1.0f)));
    Error += GLM_TEST(test_map(glm::mat3x4(0.0f), glm::mat3x4(1.0f)));
    Error += GLM_TEST(test_map(glm::mat4x2(0.0f), glm::mat4x2(1.0f)));
    Error += GLM_TEST(test_map(glm::mat4x3(0.0f), glm::mat4x3(1.0f)));
    Error += GLM_TEST(test_map(glm::mat4x4(0.0f), glm::mat4x4(1.0f)));

    return GLM_TEST_FINISH(Error);
}

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic pop
#endif

#else

int main()
{
    return GLM_TEST_SKIP_ALL("requires C++11");
}

#endif// GLM_LANG & GLM_LANG_CXX11


