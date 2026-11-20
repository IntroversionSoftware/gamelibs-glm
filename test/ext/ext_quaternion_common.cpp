#include <glm/ext/vector_float3.hpp>
#include "glm_test.hpp"
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_relational.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/scalar_relational.hpp>

// Helper function to create quaternions consistently regardless of internal layout
static glm::quat make_quat(float w, float x, float y, float z) {
    return glm::quat::wxyz(w, x, y, z);
}

static int test_conjugate()
{
    int Error = 0;

    // Test 1: Check that conjugate flips the vector part
    {
        // Create a known quaternion
        glm::quat A = make_quat(0.7071f, 0.7071f, 0.0f, 0.0f);
        glm::quat C = glm::conjugate(A);

        // Conjugate should negate x, y, z but keep w the same
        glm::quat Expected = make_quat(0.7071f, -0.7071f, 0.0f, 0.0f);

        Error += GLM_CHECK_NEAR(C, Expected, glm::epsilon<float>()); // Conjugate did not produce expected result
    }

    // Test 2: Using axis-up constructor
    {
        glm::quat const A(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
        glm::quat const C = glm::conjugate(A);

        // A quaternion with a vector part differs from its conjugate
        Error += glm_test::context(GLM_CHECK(!glm::all(glm::equal(A, C, glm::epsilon<float>()))), "A", A, "C", C);

        // Conjugate of conjugate should equal original
        glm::quat const B = glm::conjugate(C);
        Error += GLM_CHECK_NEAR(A, B, glm::epsilon<float>()); // Double conjugate should equal original
    }

    // Test 3: Test conjugate of identity quaternion
    {
        glm::quat Identity = make_quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::quat Conj = glm::conjugate(Identity);

        Error += GLM_CHECK_NEAR(Identity, Conj, glm::epsilon<float>()); // Conjugate of identity should equal identity
    }

    return Error;
}

static int test_mix()
{
    int Error = 0;

    // Test with explicit quaternions rather than using constructors that might be affected by layout
    glm::quat const Q1 = glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 0, 1));
    glm::quat const Q2 = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1));

    // Test mix
    {
        glm::quat const Q3 = glm::mix(Q1, Q2, 0.5f);
        float const Angle = glm::degrees(glm::angle(Q3));

        Error += GLM_CHECK_NEAR(Angle, 45.0f, 0.001f); // mix(Q1, Q2, 0.5) angle should be 45 degrees

        glm::quat const Q4 = glm::mix(Q2, Q1, 0.5f);
        float const Angle2 = glm::degrees(glm::angle(Q4));

        Error += GLM_CHECK_NEAR(Angle2, 45.0f, 0.001f); // mix(Q2, Q1, 0.5) angle should be 45 degrees
    }

    // Test slerp
    {
        glm::quat const Q3 = glm::slerp(Q1, Q2, 0.5f);
        float const Angle = glm::degrees(glm::angle(Q3));

        Error += GLM_CHECK_NEAR(Angle, 45.0f, 0.001f); // slerp(Q1, Q2, 0.5) angle should be 45 degrees

        glm::quat const Q4 = glm::slerp(Q2, Q1, 0.5f);
        float const Angle2 = glm::degrees(glm::angle(Q4));

        Error += GLM_CHECK_NEAR(Angle2, 45.0f, 0.001f); // slerp(Q2, Q1, 0.5) angle should be 45 degrees
    }

    // Additional test: Verify interpolation at different points (0.25, 0.75)
    {
        glm::quat const Q3 = glm::slerp(Q1, Q2, 0.25f);
        float const Angle = glm::degrees(glm::angle(Q3));

        Error += GLM_CHECK_NEAR(Angle, 22.5f, 0.001f); // slerp(Q1, Q2, 0.25) angle should be 22.5 degrees

        glm::quat const Q4 = glm::slerp(Q1, Q2, 0.75f);
        float const Angle2 = glm::degrees(glm::angle(Q4));

        Error += GLM_CHECK_NEAR(Angle2, 67.5f, 0.001f); // slerp(Q1, Q2, 0.75) angle should be 67.5 degrees
    }

    return Error;
}

int main()
{
    int Error = 0;

    Error += GLM_TEST(test_conjugate());
    Error += GLM_TEST(test_mix());

    return GLM_TEST_FINISH(Error);
}
