#include "glm_test.hpp"
#include <glm/ext/scalar_packing.hpp>
#include <glm/ext/scalar_relational.hpp>

int test_packUnorm()
{
	int Error = 0;


	return Error;
}

int test_packSnorm()
{
	int Error = 0;


	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_packUnorm());
	Error += GLM_TEST(test_packSnorm());

	return GLM_TEST_FINISH(Error);
}
