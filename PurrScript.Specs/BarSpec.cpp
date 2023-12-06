#define spec_name Bar

#include "SpecHelper.h"  // IWYU pragma: keep

Describe("Bar Something") {
    it("bar does something", []() { AssertThat(1, Equals(1)); });
}
