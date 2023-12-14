#pragma once

#define spec_context TestTemplates

#include "../SpecHelper.h"  // IWYU pragma: keep

DefineTemplate("Context");

Setup {
    auto* env = new PurrScriptSpecEnvironment();
    auto* ctx = env->CreateContext();
    current_spec->var("env", env);
    current_spec->var("context", ctx, false);
}
