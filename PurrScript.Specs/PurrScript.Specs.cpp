#include "SpecHelper.h"  // IWYU pragma: keep

// [x] Instance Function
// [x] Static Function
// [x] Global Function
// [ ] Constructor
// [ ] Instance Function from PARENT TYPE
// [ ] Instance Getter / Setter property
// [ ] List members!

Describe("PurrScript") {
    describe("Function Invocation", []() {
        it("invoke global function (print() function)", []() {
            PurrScriptSpecEnvironment env;
            auto*                     context = env.CreateContext();

            context->ExecuteCode(R"(
                print("Testing, testing, 1 2 3...")
            )");

            AssertThat(ReadLogs(), Contains("[print]Testing, testing, 1 2 3..."));
        });

        it("invoke instance function on variable (and delete variable)", []() {
            unique_ptr<PurrScriptSpecEnvironment> env = make_unique<PurrScriptSpecEnvironment>();
            auto*                                 context = env->CreateContext();

            auto& dogPackage = env->NewPackage("Dog");
            dogPackage->DefineInstanceFunction(
                "Dog", "Bark",
                unique_function_pointer(
                    [](ScriptContext*, Value* self, FunctionArguments*) -> Value* {
                        self->As<Object*>()->GetValueAs<Dog*>()->Bark();
                        return VoidValue();
                    }
                )
            );
            context->ImportPackage(dogPackage.get());

            auto* value = CreateObject("Dog", new Dog_Impl("Rover"));
            context->GetVariables()->AddVariable("rover", value);

            auto output = ReadLogs();
            AssertThat(output, Contains("[+] Dog_Impl"));
            AssertThat(output, Is().Not().Containing("[~] Dog_Impl"));

            context->ExecuteCode(R"(
                rover.Bark()
            )");

            output = ReadLogs();

            // The Dog is destroyed when the context is destroyed
            AssertThat(output, Contains("Woof"));
            AssertThat(output, Is().Not().Containing("[~] Dog_Impl"));

            env.reset();
            output = ReadLogs();

            // The Dog is destroyed when the context is destroyed
            AssertThat(output, Contains("[~] Dog_Impl"));
            AssertThat(output, Is().Not().Containing("[+] Dog_Impl"));
        });

        it("invoke static function on type", []() {
            PurrScriptSpecEnvironment env;
            auto*                     context = env.CreateContext();

            auto& somePackage = env.NewPackage("SomeStuff");
            somePackage->GetTypeRegistry()->RegisterType("SomeType");
            somePackage->DefineStaticFunction(
                "SomeType", "Multiply",
                unique_function_pointer(
                    [](ScriptContext*, Type*, FunctionArguments* args) -> Value* {
                        auto firstNumber  = args->GetArgumentByIndex(0)->GetValue()->AsFloat();
                        auto secondNumber = args->GetArgumentByIndex(1)->GetValue()->AsFloat();
                        return FloatValue(firstNumber * secondNumber);
                    }
                )
            );

            context->ExecuteCode(R"(
                import("SomeStuff")

                print("Multiply Result: " .. SomeType.Multiply(34.5, 2))
            )");

            AssertThat(ReadLogs(), Contains("Multiply Result: 69"));
        });
    });

    describe("Importing package components", []() {
        it("can import package and access its variables via namespace", []() {
            PurrScriptSpecEnvironment env;
            auto*                     context = env.CreateContext();

            auto& somePackage   = env.NewPackage("CoolThings");
            auto* coolNamespace = somePackage->DefineNamespaces({"Hello", "World"});
            somePackage->DefineGlobalFunction(
                "MyFunction",
                unique_function_pointer([](ScriptContext*, FunctionArguments*) -> Value* {
                    _Log_("Called Hello.World.MyFunction()");
                    return VoidValue();
                }),
                coolNamespace
            );

            context->ExecuteCode(R"(
                import("CoolThings")

                Hello.World.MyFunction()
            )");

            AssertThat(ReadLogs(), Contains("Called Hello.World.MyFunction()"));
        });

        it("can 'include' a namespace to include all of its variables directly", []() {
            PurrScriptSpecEnvironment env;
            auto*                     context = env.CreateContext();

            auto& somePackage   = env.NewPackage("CoolThings");
            auto* coolNamespace = somePackage->DefineNamespaces({"Hello", "World"});
            somePackage->DefineGlobalFunction(
                "MyFunction",
                unique_function_pointer([](ScriptContext*, FunctionArguments*) -> Value* {
                    _Log_("Called Hello.World.MyFunction()");
                    return VoidValue();
                }),
                coolNamespace
            );

            context->ExecuteCode(R"(
                import("CoolThings")
                include("Hello")

                World.MyFunction()
            )");

            AssertThat(ReadLogs(), Contains("Called Hello.World.MyFunction()"));
        });

        it("can 'include' a namespace to include all of its variables directly", []() {
            PurrScriptSpecEnvironment env;
            auto*                     context = env.CreateContext();

            auto& somePackage   = env.NewPackage("CoolThings");
            auto* coolNamespace = somePackage->DefineNamespaces({"Hello", "World"});
            somePackage->DefineGlobalFunction(
                "MyFunction",
                unique_function_pointer([](ScriptContext*, FunctionArguments*) -> Value* {
                    _Log_("Called Hello.World.MyFunction()");
                    return VoidValue();
                }),
                coolNamespace
            );

            context->ExecuteCode(R"(
                import("CoolThings")
                include("Hello.World")

                MyFunction()
            )");

            AssertThat(ReadLogs(), Contains("Called Hello.World.MyFunction()"));
        });

        it("can 'include' and 'import' in the same line somehow", []() {
            PurrScriptSpecEnvironment env;
            auto*                     context = env.CreateContext();

            auto& somePackage   = env.NewPackage("CoolThings");
            auto* coolNamespace = somePackage->DefineNamespaces({"Hello", "World"});
            somePackage->DefineGlobalFunction(
                "MyFunction",
                unique_function_pointer([](ScriptContext*, FunctionArguments*) -> Value* {
                    _Log_("Called Hello.World.MyFunction()");
                    return VoidValue();
                }),
                coolNamespace
            );

            context->ExecuteCode(R"(
                include("Hello.World", { from = "CoolThings" })

                MyFunction()
            )");

            AssertThat(ReadLogs(), Contains("Called Hello.World.MyFunction()"));
        });
    });
}
