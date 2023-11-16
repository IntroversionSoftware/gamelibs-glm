#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_relational.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <iostream>

// Helper function to create quaternions consistently regardless of internal layout
static glm::quat make_quat(float w, float x, float y, float z) {
    return glm::quat::wxyz(w, x, y, z);
}

// Print quaternion components for debugging
static void print_quat(const std::string& label, const glm::quat& q) {
    std::cout << label << ": w=" << q.w << ", x=" << q.x << ", y=" << q.y << ", z=" << q.z << std::endl;
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

        if (!glm::all(glm::equal(C, Expected, glm::epsilon<float>()))) {
            std::cout << "Error: Conjugate did not produce expected result" << std::endl;
            print_quat("Original", A);
            print_quat("Conjugate", C);
            print_quat("Expected", Expected);
            Error++;
        }
    }

    // Test 2: Using axis-up constructor
    {
        glm::quat const A(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
        glm::quat const C = glm::conjugate(A);

        // Fix original test: A and C should NOT be equal
        if (glm::all(glm::equal(A, C, glm::epsilon<float>()))) {
            std::cout << "Error: Quaternion and its conjugate should not be equal" << std::endl;
            print_quat("Original", A);
            print_quat("Conjugate", C);
            Error++;
        }

        // Conjugate of conjugate should equal original
        glm::quat const B = glm::conjugate(C);
        if (!glm::all(glm::equal(A, B, glm::epsilon<float>()))) {
            std::cout << "Error: Double conjugate should equal original" << std::endl;
            print_quat("Original", A);
            print_quat("Double conjugate", B);
            Error++;
        }
    }

    // Test 3: Test conjugate of identity quaternion
    {
        glm::quat Identity = make_quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::quat Conj = glm::conjugate(Identity);

        if (!glm::all(glm::equal(Identity, Conj, glm::epsilon<float>()))) {
            std::cout << "Error: Conjugate of identity should equal identity" << std::endl;
            print_quat("Identity", Identity);
            print_quat("Conjugate", Conj);
            Error++;
        }
    }

    return Error;
}

static int test_mix()
{
    int Error = 0;

    // Test with explicit quaternions rather than using constructors that might be affected by layout
    glm::quat const Q1 = glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 0, 1));
    glm::quat const Q2 = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1));

    // Print the test quaternions for debugging
    std::cout << "Testing mix/slerp with quaternions:" << std::endl;
    print_quat("Q1 (0° rotation)", Q1);
    print_quat("Q2 (90° rotation)", Q2);

    // Test mix
    {
        glm::quat const Q3 = glm::mix(Q1, Q2, 0.5f);
        float const Angle = glm::degrees(glm::angle(Q3));

        if (!glm::equal(Angle, 45.0f, 0.001f)) {
            std::cout << "Error: mix(Q1, Q2, 0.5) angle should be 45 degrees, got " << Angle << std::endl;
            print_quat("Result", Q3);
            Error++;
        }

        glm::quat const Q4 = glm::mix(Q2, Q1, 0.5f);
        float const Angle2 = glm::degrees(glm::angle(Q4));

        if (!glm::equal(Angle2, 45.0f, 0.001f)) {
            std::cout << "Error: mix(Q2, Q1, 0.5) angle should be 45 degrees, got " << Angle2 << std::endl;
            print_quat("Result", Q4);
            Error++;
        }
    }

    // Test slerp
    {
        glm::quat const Q3 = glm::slerp(Q1, Q2, 0.5f);
        float const Angle = glm::degrees(glm::angle(Q3));

        if (!glm::equal(Angle, 45.0f, 0.001f)) {
            std::cout << "Error: slerp(Q1, Q2, 0.5) angle should be 45 degrees, got " << Angle << std::endl;
            print_quat("Result", Q3);
            Error++;
        }

        glm::quat const Q4 = glm::slerp(Q2, Q1, 0.5f);
        float const Angle2 = glm::degrees(glm::angle(Q4));

        if (!glm::equal(Angle2, 45.0f, 0.001f)) {
            std::cout << "Error: slerp(Q2, Q1, 0.5) angle should be 45 degrees, got " << Angle2 << std::endl;
            print_quat("Result", Q4);
            Error++;
        }
    }

    // Additional test: Verify interpolation at different points (0.25, 0.75)
    {
        glm::quat const Q3 = glm::slerp(Q1, Q2, 0.25f);
        float const Angle = glm::degrees(glm::angle(Q3));

        if (!glm::equal(Angle, 22.5f, 0.001f)) {
            std::cout << "Error: slerp(Q1, Q2, 0.25) angle should be 22.5 degrees, got " << Angle << std::endl;
            Error++;
        }

        glm::quat const Q4 = glm::slerp(Q1, Q2, 0.75f);
        float const Angle2 = glm::degrees(glm::angle(Q4));

        if (!glm::equal(Angle2, 67.5f, 0.001f)) {
            std::cout << "Error: slerp(Q1, Q2, 0.75) angle should be 67.5 degrees, got " << Angle2 << std::endl;
            Error++;
        }
    }

    return Error;
}

int main()
{
    int Error = 0;

    std::cout << "=== Quaternion Common Extension Tests ===" << std::endl;
    std::cout << "Quaternion memory layout: " <<
    #ifdef GLM_FORCE_QUAT_DATA_WXYZ
        "WXYZ"
    #else
        "XYZW"
    #endif
    << std::endl;

    std::cout << "\nTesting conjugate function:" << std::endl;
    Error += test_conjugate();

    std::cout << "\nTesting mix/slerp functions:" << std::endl;
    Error += test_mix();

    if (Error == 0)
        std::cout << "\nAll tests passed!" << std::endl;
    else
        std::cout << "\nFailed tests: " << Error << std::endl;

    return Error;
}
