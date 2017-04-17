
#include "test.hpp"

#include "datatypes.hpp"
#include "spatial.hpp"

int main(int argc, char* argv[])
{
	std::list<_vs_test_adapter::tester_factory *>::iterator it = _vs_test_adapter::testers.begin();
	for (; it != _vs_test_adapter::testers.end(); it++) {
		_vs_test_adapter::tester *instance = (*it)->create();
		std::list<_vs_test_adapter::method *>::iterator mit = _vs_test_adapter::methods.begin();
		for (; mit != _vs_test_adapter::methods.end(); mit++) {
			(*mit)->run(instance);
		}
	}

	return 0;
}


