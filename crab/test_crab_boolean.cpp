
#include "boolean.hpp"

using namespace crab::datatypes;
using namespace crab;

static_assert(True.And(True), "");
static_assert(True.All(True, True, True), "");
static_assert(True.Any(false, True, false), "");


int main(){}