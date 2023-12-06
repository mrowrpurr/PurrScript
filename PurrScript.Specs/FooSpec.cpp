#define spec_name Foo

#include "SpecHelper.h"  // IWYU pragma: keep

Describe("Foo Something") {
    it("foo does something", []() { AssertThat(1, Equals(1)); });
}
