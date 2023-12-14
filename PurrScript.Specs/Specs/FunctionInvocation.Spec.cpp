#define spec_name FunctionInvocation

#include "SpecHelper.h"  // IWYU pragma: keep

UseTemplate("Context");

Test("invoke instance function on variable (and delete variable)") {
    auto* env     = get_env();
    auto* context = get_context();

    auto& dogPackage = env->NewPackage("Dog");
    dogPackage->DefineInstanceFunction(
        "Dog", "Bark",
        unique_function_pointer([](ScriptContext*, Value* self, FunctionArguments*) -> Value* {
            self->As<Object*>()->GetValueAs<Dog*>()->Bark();
            return VoidValue();
        })
    );
    context->ImportPackage(dogPackage.get());

    auto* value = CreateObject("Dog", new Dog_Impl("Rover"));
    context->GetVariables()->AddVariable("rover", value);

    auto output = ReadLogs();
    AssertThat(output, Contains("[+] Dog_Impl"));
    AssertThat(output, Is().Not().Containing("[~] Dog_Impl"));

    run_code("rover.Bark()");

    output = ReadLogs();

    // The Dog is destroyed when the context is destroyed
    AssertThat(output, Contains("Woof"));
    AssertThat(output, Is().Not().Containing("[~] Dog_Impl"));

    current_spec->vars()->unset("env");  // delete the environment
    output = ReadLogs();

    // The Dog is destroyed when the context is destroyed
    AssertThat(output, Contains("[~] Dog_Impl"));
    AssertThat(output, Is().Not().Containing("[+] Dog_Impl"));
}

Test("invoke static function on type") {
    auto* env     = get_env();
    auto* context = get_context();

    auto& somePackage = env->NewPackage("SomeStuff");
    somePackage->GetTypeRegistry()->RegisterType("SomeType");
    somePackage->DefineStaticFunction(
        "SomeType", "Multiply",
        unique_function_pointer([](ScriptContext*, Type*, FunctionArguments* args) -> Value* {
            auto firstNumber  = args->GetArgumentByIndex(0)->GetValue()->AsFloat();
            auto secondNumber = args->GetArgumentByIndex(1)->GetValue()->AsFloat();
            return FloatValue(firstNumber * secondNumber);
        })
    );

    run_code(R"(
        import("SomeStuff")

        print("Multiply Result: " .. SomeType.Multiply(34.5, 2))
    )");

    AssertThat(ReadLogs(), Contains("Multiply Result: 69"));
}
