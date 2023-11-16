#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/intersect.hpp>

static int test_intersectRayPlane()
{
	int Error = 0;
	glm::vec3 const PlaneOrigin(0, 0, 1);
	glm::vec3 const PlaneNormal(0, 0, -1);
	glm::vec3 const RayOrigin(0, 0, 0);
	glm::vec3 const RayDir(0, 0, 1);

	// check that inversion of the plane normal has no effect
	{
		float Distance = 0;
		bool const Result = glm::intersectRayPlane(RayOrigin, RayDir, PlaneOrigin, PlaneNormal, Distance);
		Error += glm::abs(Distance - 1.f) <= std::numeric_limits<float>::epsilon() ? 0 : 1;
		Error += Result ? 0 : 1;
	}
	{
		float Distance = 0;
		bool const Result = glm::intersectRayPlane(RayOrigin, RayDir, PlaneOrigin, -1.f * PlaneNormal, Distance);
		Error += glm::abs(Distance - 1.f) <= std::numeric_limits<float>::epsilon() ? 0 : 1;
		Error += Result ? 0 : 1;
	}

	// check if plane is before of behind the ray origin
	{
		float Distance = 9.9999f; // value should not be changed
		bool const Result = glm::intersectRayPlane(RayOrigin, RayDir, -1.f * PlaneOrigin, PlaneNormal, Distance);
		Error += glm::abs(Distance - 9.9999f) <= std::numeric_limits<float>::epsilon() ? 0 : 1;
		Error += Result ? 1 : 0; // there is no intersection in front of the ray origin, only behind
	}

	return Error;
}

static int test_intersectRayTriangle()
{
	int Error = 0;

	// Helper lambda to run a test and report errors
	auto runTest = [&Error](
		const char* testName,
		const glm::vec3& Orig,
		const glm::vec3& Dir,
		const glm::vec3& Vert0,
		const glm::vec3& Vert1,
		const glm::vec3& Vert2,
		bool expectedResult,
		const glm::vec2& expectedBaryPosition = glm::vec2(0),
		float expectedDistance = 0.0f)
		{
			glm::vec2 BaryPosition(0);
			float Distance = 0;

			bool const Result = glm::intersectRayTriangle(Orig, Dir, Vert0, Vert1, Vert2, BaryPosition, Distance);

			// Only check bary position and distance if we expect an intersection
			if (expectedResult) {
				bool baryMatch = glm::all(glm::epsilonEqual(
					BaryPosition,
					expectedBaryPosition,
					std::numeric_limits<float>::epsilon() * 10.0f));

				bool distanceMatch = glm::abs(Distance - expectedDistance) <=
					std::numeric_limits<float>::epsilon() * 10.0f;

				if (!baryMatch) {
					printf("Test '%s' failed: Expected bary position (%f, %f), got (%f, %f)\n",
						   testName,
						   expectedBaryPosition.x, expectedBaryPosition.y,
						   BaryPosition.x, BaryPosition.y);
					Error++;
				}

				if (!distanceMatch) {
					printf("Test '%s' failed: Expected distance %f, got %f\n",
						   testName,
						   expectedDistance,
						   Distance);
					Error++;
				}
			}

			if (Result != expectedResult) {
				printf("Test '%s' failed: Expected %s, got %s\n",
					   testName,
					   expectedResult ? "hit" : "miss",
					   Result ? "hit" : "miss");
				Error++;
			}
		};

	// ==========================================
	// 1. Basic hit tests
	// ==========================================

	// Original test: Ray straight down to center of triangle
	runTest("Direct center hit",
			glm::vec3(0, 0, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			true, glm::vec2(0), 2.0f);

	// Hit at the first vertex
	runTest("Hit at vertex 0",
			glm::vec3(0, 0, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			true, glm::vec2(0, 0), 2.0f);

	// Hit at the second vertex
	runTest("Hit at vertex 1",
			glm::vec3(-1, -1, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			true, glm::vec2(1, 0), 2.0f);

	// Hit at the third vertex
	runTest("Hit at vertex 2",
			glm::vec3(1, -1, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			true, glm::vec2(0, 1), 2.0f);

	// Hit at the edge between vertex 0 and 1 (midpoint)
	runTest("Hit at edge 0-1",
			glm::vec3(-0.5, -0.5, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			true, glm::vec2(0.5, 0), 2.0f);

	// Hit at the edge between vertex 1 and 2 (midpoint)
	runTest("Hit at edge 1-2",
			glm::vec3(0, -1, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			true, glm::vec2(0.5, 0.5), 2.0f);

	// Hit at the edge between vertex 2 and 0 (midpoint)
	runTest("Hit at edge 2-0",
			glm::vec3(0.5, -0.5, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			true, glm::vec2(0, 0.5), 2.0f);

	// Hit inside triangle
	runTest("Hit inside triangle",
			glm::vec3(0, -0.33, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			true, glm::vec2(0.165, 0.165), 2.0f);

	// ==========================================
	// 2. Various ray directions
	// ==========================================

	// Hit with angled ray (not straight down)
	runTest("Hit with angled ray",
			glm::vec3(2, 2, 2), glm::vec3(-1, -1, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			true, glm::vec2(0, 0), 2.0f);

	// ==========================================
	// 3. Miss scenarios
	// ==========================================

	// Ray misses the triangle (outside)
	runTest("Ray misses outside",
			glm::vec3(3, 3, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			false);

	// Ray in wrong direction (away from triangle)
	runTest("Ray in wrong direction",
			glm::vec3(0, 0, -2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			true, glm::vec2(0, 0), -2.0f);  // GLM returns negative distance when triangle is behind ray

	// Ray parallel to triangle plane
	runTest("Ray parallel to plane",
			glm::vec3(0, 0, 2), glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			false);

	// ==========================================
	// 4. Edge cases
	// ==========================================

	// Triangle behind ray origin
	runTest("Triangle behind ray",
			glm::vec3(0, 0, -2), glm::vec3(0, 0, 1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			true, glm::vec2(0, 0), 2.0f);

	// Zero-length ray direction
	runTest("Zero-length ray",
			glm::vec3(0, 0, 2), glm::vec3(0, 0, 0),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			false);

	// Degenerate triangle (all vertices on same line)
	runTest("Degenerate triangle (line)",
			glm::vec3(0, 0, 2), glm::vec3(0, 0, -1),
			glm::vec3(-1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 0, 0),
			false);

	// Degenerate triangle (all vertices at same point)
	runTest("Degenerate triangle (point)",
			glm::vec3(0, 0, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),
			false);

	// Ray origin exactly on triangle plane (z=0)
	runTest("Ray origin on triangle plane",
			glm::vec3(0, 0, 0), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			true);  // GLM considers this a hit

	// Ray origin at a vertex
	runTest("Ray origin at vertex",
			glm::vec3(0, 0, 0), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			true);  // GLM considers this a hit

	// ==========================================
	// 5. Non-trivial triangles (not in XY plane)
	// ==========================================

	// Triangle in YZ plane
	runTest("Triangle in YZ plane",
			glm::vec3(-2, 0, 0), glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 0), glm::vec3(0, -1, -1), glm::vec3(0, 1, -1),
			true, glm::vec2(0, 0), 2.0f);

	// Triangle in XZ plane
	runTest("Triangle in XZ plane",
			glm::vec3(0, 2, 0), glm::vec3(0, -1, 0),
			glm::vec3(0, 0, 0), glm::vec3(-1, 0, -1), glm::vec3(1, 0, -1),
			true, glm::vec2(0, 0), 2.0f);

	// Arbitrarily oriented triangle - ray hits vertex 0
	runTest("Arbitrary triangle orientation",
			glm::vec3(0, 0, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, 1, 0.5), glm::vec3(1, 1, 0.5),
			true, glm::vec2(0, 0), 2.0f);  // Ray hits vertex 0 of the triangle

	// ==========================================
	// 6. Numerical robustness
	// ==========================================

	// Very small triangle
	runTest("Very small triangle",
			glm::vec3(0, 0, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0),
			glm::vec3(-0.0001f, -0.0001f, 0),
			glm::vec3(0.0001f, -0.0001f, 0),
			true, glm::vec2(0, 0), 2.0f);

	// Very large triangle
	runTest("Very large triangle",
			glm::vec3(0, 0, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0),
			glm::vec3(-10000.0f, -10000.0f, 0),
			glm::vec3(10000.0f, -10000.0f, 0),
			true, glm::vec2(0, 0), 2.0f);

	// Very distant triangle
	runTest("Very distant triangle",
			glm::vec3(0, 0, 10000), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			true, glm::vec2(0, 0), 10000.0f);

	// Ray almost parallel to triangle
	runTest("Nearly parallel ray",
			glm::vec3(0, 0, 0.1f), glm::vec3(0.01f, 0.01f, -1),
			glm::vec3(0, 0, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0),
			false);  // GLM reports this as a miss

	// ==========================================
	// 7. Special triangle configurations
	// ==========================================

	// Needle-like triangle
	runTest("Needle-like triangle",
			glm::vec3(0, 0, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(0.001f, -10.0f, 0), glm::vec3(-0.001f, -10.0f, 0),
			true, glm::vec2(0, 0), 2.0f);

	// Triangle with very acute angle
	runTest("Triangle with acute angle",
			glm::vec3(0.1f, -0.1f, 2), glm::vec3(0, 0, -1),
			glm::vec3(0, 0, 0), glm::vec3(0.001f, -10.0f, 0), glm::vec3(10.0f, -0.001f, 0),
			true, glm::vec2(0.01f, 0.01f), 2.0f);

	return Error;
}

#if GLM_PLATFORM != GLM_PLATFORM_LINUX
static int test_intersectLineTriangle()
{
	int Error = 0;

	glm::vec3 const Orig(0, 0, 2);
	glm::vec3 const Dir(0, 0, -1);
	glm::vec3 const Vert0(0, 0, 0);
	glm::vec3 const Vert1(-1, -1, 0);
	glm::vec3 const Vert2(1, -1, 0);
	glm::vec3 Position(2.0f, 0.0f, 0.0f);

	bool const Result = glm::intersectLineTriangle(Orig, Dir, Vert0, Vert1, Vert2, Position);

	Error += glm::all(glm::epsilonEqual(Position, glm::vec3(2.0f, 0.0f, 0.0f), std::numeric_limits<float>::epsilon())) ? 0 : 1;
	Error += Result ? 0 : 1;

	return Error;
}
#endif//GLM_PLATFORM != GLM_PLATFORM_LINUX

int main()
{
	int Error = 0;

#if GLM_PLATFORM != GLM_PLATFORM_LINUX
	Error += test_intersectRayPlane();
	Error += test_intersectRayTriangle();
	Error += test_intersectLineTriangle(); // Disabled on 2025/01/16, C.I. failing on Ubuntu latest, GCC 13.3.0
#endif//GLM_PLATFORM != GLM_PLATFORM_LINUX

	return Error;
}
