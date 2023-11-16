#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/quaternion_relational.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>
#include <iostream>

// Helper function to create quaternions consistently regardless of layout
template <typename T = float, glm::qualifier Q = glm::defaultp>
static glm::qua<T, Q> make_quat(T w, T x, T y, T z) {
#ifdef GLM_FORCE_QUAT_DATA_XYZW
	// When GLM_FORCE_QUAT_DATA_XYZW is defined, constructor expects (x,y,z,w)
	return glm::qua<T, Q>(x, y, z, w);
#else
	// Default behavior with constructor expecting (w,x,y,z)
	return glm::qua<T, Q>(w, x, y, z);
#endif
}

// Print quaternion components for debugging
template <typename T, glm::qualifier Q>
static void print_quat(const std::string& label, const glm::qua<T, Q>& q) {
	std::cout << label << ": w=" << q.w << ", x=" << q.x << ", y=" << q.y << ", z=" << q.z << std::endl;
}

// Print vector components for debugging
template <typename T, glm::qualifier Q>
static void print_vec(const std::string& label, const glm::vec<3, T, Q>& v) {
	std::cout << label << ": x=" << v.x << ", y=" << v.y << ", z=" << v.z << std::endl;
}

static int test_quat_fastMix()
{
	int Error = 0;
	const float Epsilon = 0.01f;

	std::cout << "Testing quaternion fastMix:" << std::endl;

	glm::quat A = glm::angleAxis(0.0f, glm::vec3(0, 0, 1));
	glm::quat B = glm::angleAxis(glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
	glm::quat C = glm::fastMix(A, B, 0.5f);
	glm::quat D = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 1));

	// Check all components at once
	bool componentsEqual =
		glm::epsilonEqual(C.x, D.x, Epsilon) &&
		glm::epsilonEqual(C.y, D.y, Epsilon) &&
		glm::epsilonEqual(C.z, D.z, Epsilon) &&
		glm::epsilonEqual(C.w, D.w, Epsilon);

	if (!componentsEqual) {
		std::cout << "Error: fastMix result doesn't match expected quaternion" << std::endl;
		print_quat("Result", C);
		print_quat("Expected", D);
		Error++;
	}

	return Error;
}

static int test_quat_shortMix()
{
	int Error = 0;
	const float Epsilon = 0.01f;

	std::cout << "\nTesting quaternion shortMix:" << std::endl;

	glm::quat A = glm::angleAxis(0.0f, glm::vec3(0, 0, 1));
	glm::quat B = glm::angleAxis(glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
	glm::quat C = glm::shortMix(A, B, 0.5f);
	glm::quat D = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 1));

	// Check all components at once
	bool componentsEqual =
		glm::epsilonEqual(C.x, D.x, Epsilon) &&
		glm::epsilonEqual(C.y, D.y, Epsilon) &&
		glm::epsilonEqual(C.z, D.z, Epsilon) &&
		glm::epsilonEqual(C.w, D.w, Epsilon);

	if (!componentsEqual) {
		std::cout << "Error: shortMix result doesn't match expected quaternion" << std::endl;
		print_quat("Result", C);
		print_quat("Expected", D);
		Error++;
	}

	return Error;
}

static int test_orientation()
{
	int Error = 0;
	const float Epsilon = 0.0001f;

	std::cout << "\nTesting quaternion orientation (roll, pitch, yaw):" << std::endl;

	// Test roll (rotation around x-axis)
	{
		// Create a quaternion representing a 90-degree roll
		glm::quat q = make_quat(1.0f, 0.0f, 0.0f, 1.0f);
		q = glm::normalize(q);

		float p = glm::roll(q);

		if (!glm::epsilonEqual(p, glm::pi<float>() * 0.5f, Epsilon)) {
			std::cout << "Error: Roll angle incorrect" << std::endl;
			std::cout << "Expected: " << glm::pi<float>() * 0.5f << ", got: " << p << std::endl;
			print_quat("Quaternion", q);
			Error++;
		}
	}

	// Test pitch (rotation around y-axis)
	{
		glm::quat q = make_quat(1.0f, 0.0f, 0.0f, 1.0f);
		q = glm::normalize(q);

		float p = glm::pitch(q);

		if (!glm::epsilonEqual(p, 0.f, Epsilon)) {
			std::cout << "Error: Pitch angle incorrect" << std::endl;
			std::cout << "Expected: 0, got: " << p << std::endl;
			print_quat("Quaternion", q);
			Error++;
		}
	}

	// Test yaw (rotation around z-axis)
	{
		glm::quat q = make_quat(1.0f, 0.0f, 0.0f, 1.0f);
		q = glm::normalize(q);

		float p = glm::yaw(q);

		if (!glm::epsilonEqual(p, 0.f, Epsilon)) {
			std::cout << "Error: Yaw angle incorrect" << std::endl;
			std::cout << "Expected: 0, got: " << p << std::endl;
			print_quat("Quaternion", q);
			Error++;
		}
	}

	// Test with a combined rotation
	{
		// Create a quaternion with a rotation around all three axes
		glm::quat qRoll = glm::angleAxis(glm::radians(30.0f), glm::vec3(1, 0, 0));
		glm::quat qPitch = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
		glm::quat qYaw = glm::angleAxis(glm::radians(60.0f), glm::vec3(0, 0, 1));

		glm::quat q = qYaw * qPitch * qRoll; // Order matters: yaw, then pitch, then roll

		float roll = glm::roll(q);
		float pitch = glm::pitch(q);
		float yaw = glm::yaw(q);

		std::cout << "Combined rotation:" << std::endl;
		std::cout << "  Roll: " << glm::degrees(roll) << " degrees" << std::endl;
		std::cout << "  Pitch: " << glm::degrees(pitch) << " degrees" << std::endl;
		std::cout << "  Yaw: " << glm::degrees(yaw) << " degrees" << std::endl;

		// Note: We don't check exact values here because Euler angles can have multiple
		// representations of the same rotation, especially for combined rotations
	}

	return Error;
}

static int test_rotation()
{
	int Error = 0;
	const float Epsilon = glm::epsilon<float>();

	std::cout << "\nTesting quaternion rotation function:" << std::endl;

	glm::vec3 v(1, 0, 0);
	glm::vec3 u(0, 1, 0);

	glm::quat Rotation = glm::rotation(v, u);
	float Angle = glm::angle(Rotation);

	if (!(glm::abs(Angle - glm::pi<float>() * 0.5f) < Epsilon)) {
		std::cout << "Error: Rotation angle incorrect" << std::endl;
		std::cout << "Expected: " << glm::pi<float>() * 0.5f << ", got: " << Angle << std::endl;
		print_quat("Quaternion", Rotation);
		Error++;
	}

	// Additional test to verify the rotation actually works as expected
	glm::vec3 rotated = Rotation * v;
	if (!glm::all(glm::epsilonEqual(rotated, u, 0.0001f))) {
		std::cout << "Error: Rotation doesn't properly rotate vector" << std::endl;
		print_vec("Original", v);
		print_vec("Expected", u);
		print_vec("Actual", rotated);
		Error++;
	}

	return Error;
}

static int test_log()
{
	int Error = 0;
	const float Epsilon = 0.0001f;

	std::cout << "\nTesting quaternion logarithm and exponential:" << std::endl;

	glm::vec3 v(1, 0, 0);
	glm::vec3 u(0, 1, 0);

	glm::quat q = glm::rotation(v, u);
	glm::quat p = glm::log(q);
	glm::quat r = glm::exp(p);

	if (!glm::all(glm::equal(q, r, Epsilon))) {
		std::cout << "Error: exp(log(q)) should equal q" << std::endl;
		print_quat("Original", q);
		print_quat("Log", p);
		print_quat("Exp(Log)", r);
		Error++;
	}

	// Additional test with multiple rotations
	for (float angle = 0.1f; angle < glm::pi<float>(); angle += 0.5f) {
		glm::quat rot = glm::angleAxis(angle, glm::normalize(glm::vec3(1, 2, 3)));
		glm::quat logRot = glm::log(rot);
		glm::quat expLogRot = glm::exp(logRot);

		if (!glm::all(glm::equal(rot, expLogRot, Epsilon))) {
			std::cout << "Error: exp(log(q)) should equal q for angle " << angle << std::endl;
			print_quat("Original", rot);
			print_quat("Exp(Log)", expLogRot);
			Error++;
		}
	}

	return Error;
}

int main()
{
	int Error = 0;

	std::cout << "=== Quaternion GTX Tests ===" << std::endl;
	std::cout << "Quaternion memory layout: ";

#ifdef GLM_FORCE_QUAT_DATA_WXYZ
	std::cout << "WXYZ (storage)";
#else
	std::cout << "XYZW (storage)";
#endif

	std::cout << " / ";

#ifdef GLM_FORCE_QUAT_DATA_XYZW
	std::cout << "XYZW (constructor)" << std::endl;
#else
	std::cout << "WXYZ (constructor)" << std::endl;
#endif
	std::cout << std::endl;

	Error += test_log();
	Error += test_rotation();
	Error += test_orientation();
	Error += test_quat_fastMix();
	Error += test_quat_shortMix();

	if (Error == 0)
		std::cout << "\nAll tests passed!" << std::endl;
	else
		std::cout << "\nFailed tests: " << Error << std::endl;

	return Error;
}
