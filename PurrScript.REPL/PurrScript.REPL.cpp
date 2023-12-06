// TODO: extract Lua out into a .dll that is dynamically loaded and configures PurrScript for Lua

#include <PurrScript/Implementations/Lua/Lua_LanguageHandler_Impl.h>
#include <PurrScript/Implementations/PurrScriptAPI_Impl.h>
#include <PurrScript/Packages/Console.h>
#include <PurrScript/Packages/Core.h>

using namespace PurrScript;
using namespace PurrScript::Implementations;

#include <atomic>
#include <csignal>
#include <iostream>
#include <string>

using namespace std;

std::atomic<bool> signalReceived{false};

void signalHandler(int signal) { signalReceived = true; }

constexpr auto CONTINUATION_CHARACTER = '\\';
constexpr auto EXIT_COMMAND           = "exit";

int main() {
    signal(SIGINT, signalHandler);

    PurrScriptAPI_Impl purrScript;

    // Setup PurrScript packages
    auto* corePackage = Packages::Core::GetPackage();
    purrScript.GetPackageRegistry()->AddPackage(corePackage);
    purrScript.GetDefaultPackages()->AddPackage(corePackage);
    auto* consolePackage = Packages::Console::GetPackage();
    purrScript.GetPackageRegistry()->AddPackage(consolePackage);
    purrScript.GetDefaultPackages()->AddPackage(consolePackage);

    // Configure PurrScript for Lua
    Lua::Lua_LanguageHandler_Impl luaLanguageHandler{&purrScript};
    purrScript.SetDefaultLanguageHandler(&luaLanguageHandler);
    auto* scriptContext = purrScript.GetScriptContextManager()->CreateContext();

    string input, codeBlock;
    bool   isInCodeBlock = false;

    while (!signalReceived) {
        if (!isInCodeBlock) std::cout << ">> ";
        if (signalReceived) break;

        if (!std::getline(std::cin, input)) {
            if (signalReceived) break;
            std::cin.clear();
            continue;
        }

        if (signalReceived) break;
        if (input.empty()) continue;

        if (input == EXIT_COMMAND) break;

        if (input.back() == CONTINUATION_CHARACTER) {
            input.pop_back();
            codeBlock += input + "\n";
            isInCodeBlock = true;
        } else {
            codeBlock += input;
            try {
                auto* result = scriptContext->ExecuteCode(codeBlock.c_str());
                switch (result->GetStatus()) {
                    case PurrScript::ExecuteCodeResult::Status::SyntaxError:
                        cout << "Syntax error: " << result->GetErrorMessage() << endl;
                        break;
                    case PurrScript::ExecuteCodeResult::Status::RuntimeError:
                        cout << "Runtime error: " << result->GetErrorMessage() << endl;
                        break;
                    case PurrScript::ExecuteCodeResult::Status::Exception:
                        cout << "Exception: " << result->GetErrorMessage() << endl;
                        break;
                    case PurrScript::ExecuteCodeResult::Status::OK: {
                        if (auto* value = result->GetResult())
                            cout << "Result: " << value->AsString() << endl;
                        break;
                    }
                    case PurrScript::ExecuteCodeResult::Status::Unset:
                        cout << "Code was not executed" << endl;
                        break;
                }
            } catch (const exception& e) {
                cout << e.what() << endl;
            }
            codeBlock.clear();
            isInCodeBlock = false;
        }
    }

    std::cout << "Exiting..." << std::endl;

    return 0;
}
