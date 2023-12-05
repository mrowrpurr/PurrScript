#pragma once

#include <function_pointer.h>
#include <void_pointer.h>

#include <cstdint>

// TODO: don't use these 'Merge' functions which are O(n) - use O(1)-ish full structure copying

namespace PurrScript {

    namespace SpecialFunctions {
        inline constexpr auto GETTER      = "[]";
        inline constexpr auto SETTER      = "[]=";
        inline constexpr auto CALL        = "()";
        inline constexpr auto CONSTRUCTOR = ".ctor";
    }

    struct Package;
    struct PackageLookupHandlers;
    struct PackageImportHandlers;
    struct ScriptContext;
    struct PurrScriptAPI;
    struct FunctionArguments;
    struct Value;
    struct Type;
    struct VariableCollection;

    struct ExecuteCodeResult {
        enum class Status { OK, SyntaxError, RuntimeError, Exception, Unset };
        virtual ~ExecuteCodeResult()                             = default;
        virtual Status      GetStatus() const                    = 0;
        virtual void        SetStatus(Status status)             = 0;
        virtual const char* GetErrorMessage() const              = 0;
        virtual void        SetErrorMessage(const char* message) = 0;
        virtual Value*      GetResult() const                    = 0;
        virtual void        SetResult(Value* result)             = 0;
    };

    // TODO: consider doing away with this and having even the global context be a Value* so there's
    // always a 'self'
    using FunctionFn = IFunctionPointer<Value*(ScriptContext* context, FunctionArguments* args)>;

    using InstanceFunctionFn =
        IFunctionPointer<Value*(ScriptContext* context, Value* self, FunctionArguments* args)>;

    using StaticFunctionFn =
        IFunctionPointer<Value*(ScriptContext* context, Type* type, FunctionArguments* args)>;

    struct LanguageContext {
        virtual ~LanguageContext()                               = default;
        virtual ExecuteCodeResult* ExecuteCode(const char* code) = 0;
    };

    struct LanguageContextManager {
        virtual ~LanguageContextManager()                                 = default;
        virtual LanguageContext* CreateContext(ScriptContext*)            = 0;
        virtual bool             DestroyContext(LanguageContext* context) = 0;
    };

    struct LanguageHandler {
        virtual ~LanguageHandler()                                  = default;
        virtual LanguageContextManager* GetLanguageContextManager() = 0;
    };

    enum class ValueType {
        Undefined,
        None,
        Void,
        Boolean,
        Integer,
        UnsignedInteger,
        Float,
        String,
        Type,
        Namespace,
        Error,
        Function,
        Property,
        Object,
        Map,
        Array,
        Set
    };

    struct Value {
        inline static constexpr auto TYPE_NAME        = "Value";
        inline static constexpr auto PARENT_TYPE_NAME = "";

        // Important: Values can define their own destructors
        // Values are thrown away when no longer needed
        virtual ~Value() = default;

        // This is the heart of a 'Value'
        // It is *anything* of a defined "type"
        virtual const char* GetTypeName() = 0;

        // For performant lookup of the built-in types
        virtual ValueType GetValueType() { return ValueType::Undefined; }

        // Should this value be copied when passed around?
        virtual bool IsValueType() { return false; }
        virtual bool IsReferenceType() { return true; }

        // Is this like a C pointer? Not memory managed?
        // Or should references to the Value destroy the Value when it's no longer referenced?
        virtual bool IsDestructable()                            = 0;
        virtual void SetIsDestructable(bool destructable = true) = 0;

        // Copy this value
        virtual Value* Copy() = 0;

        // TODO TODO TODO - add 'const' to a variety of the getters which can clearly be const!

        // Here are built-in simple primitive types
        virtual bool IsBoolean() { return false; }
        virtual bool IsInteger() { return false; }
        virtual bool IsUnsignedInteger() { return false; }
        virtual bool IsFloat() { return false; }
        virtual bool IsString() { return false; }
        virtual bool IsNone() { return false; }
        virtual bool IsVoid() { return false; }
        virtual bool IsType() { return false; }
        virtual bool IsNamespace() { return false; }
        virtual bool IsError() { return false; }
        virtual bool IsFunction() { return false; }

        // And collection types too
        virtual bool IsArray() { return false; }
        virtual bool IsMap() { return false; }
        virtual bool IsSet() { return false; }

        // Is it one of the collection types?
        virtual bool IsCollection() { return false; }

        // Can you call this value?
        virtual bool IsCallable() { return false; }

        // Does it inherit from 'Object'?
        virtual bool IsObject() { return false; }

        // Build-in conversions to simple types
        virtual bool          AsBoolean() { return false; }
        virtual std::int64_t  AsInteger() { return 0; }
        virtual std::uint64_t AsUnsignedInteger() { return 0; }
        virtual double        AsFloat() { return 0.0; }
        virtual const char*   AsString() { return GetTypeName(); }
        virtual const char*   AsDebugString() { return GetTypeName(); }

        // Every value can be called as a function
        virtual Value* Call(ScriptContext*, Value* self, FunctionArguments* args) {
            return nullptr;
        }

        // Note: if you want a Pointer* you need to specify it in your <Template*> type
        template <typename T>
        T As() {
            return dynamic_cast<T>(this);
        }
    };

    struct ValueReferenceCounter {
        using ValueCallbackFn = IFunctionPointer<void(Value*)>;
        using TypeAndCountCallbackFn =
            IFunctionPointer<void(const char* typeName, std::uint32_t count)>;

        virtual ~ValueReferenceCounter()                                   = default;
        virtual bool          IsEnabled()                                  = 0;
        virtual void          AddReference(Value*)                         = 0;
        virtual void          RemoveReference(Value*)                      = 0;
        virtual std::uint32_t GetReferenceCount(Value*)                    = 0;
        virtual std::uint32_t GetReferenceCountByType(const char*)         = 0;
        virtual void          ForEachValue(ValueCallbackFn* fn)            = 0;
        virtual void          ForEachValueType(TypeAndCountCallbackFn* fn) = 0;
        virtual void          ForEachValueOfType(const char* typeName, ValueCallbackFn* fn) = 0;
        virtual void          OnReferenceAdded(ValueCallbackFn* fn)                         = 0;
        virtual void          OnReferenceRemoved(ValueCallbackFn* fn)                       = 0;
    };

    struct GlobalContext : public virtual Value {
        virtual ~GlobalContext() = default;
        // TODO!

        // Add an IsGlobalContext() method to Value
    };

    struct Boolean : public virtual Value {
        inline static constexpr auto TYPE_NAME        = "Boolean";
        inline static constexpr auto PARENT_TYPE_NAME = Value::TYPE_NAME;
        inline static constexpr auto TRUE_STRING      = "true";
        inline static constexpr auto FALSE_STRING     = "false";
        inline static constexpr auto TRUE_INTEGER     = 1;
        inline static constexpr auto FALSE_INTEGER    = 0;
        inline static constexpr auto TRUE_FLOAT       = 1.0;
        inline static constexpr auto FALSE_FLOAT      = 0.0;
        virtual ~Boolean()                            = default;
        const char*   GetTypeName() override { return TYPE_NAME; }
        ValueType     GetValueType() override { return ValueType::Boolean; }
        virtual bool  GetValue()           = 0;
        virtual void  SetValue(bool value) = 0;
        bool          IsValueType() override { return true; }
        bool          IsBoolean() override { return true; }
        bool          AsBoolean() override { return GetValue(); }
        std::int64_t  AsInteger() override { return GetValue() ? TRUE_INTEGER : FALSE_INTEGER; }
        std::uint64_t AsUnsignedInteger() override {
            return GetValue() ? TRUE_INTEGER : FALSE_INTEGER;
        }
        double      AsFloat() override { return GetValue() ? TRUE_FLOAT : FALSE_FLOAT; }
        const char* AsString() override { return GetValue() ? TRUE_STRING : FALSE_STRING; }
    };

    struct Integer : public virtual Value {
        inline static constexpr auto TYPE_NAME        = "Integer";
        inline static constexpr auto PARENT_TYPE_NAME = Value::TYPE_NAME;
        virtual ~Integer()                            = default;
        const char*          GetTypeName() override { return TYPE_NAME; }
        ValueType            GetValueType() override { return ValueType::Integer; }
        virtual std::int64_t GetValue()                   = 0;
        virtual void         SetValue(std::int64_t value) = 0;
        bool                 IsValueType() override { return true; }
        bool                 IsInteger() override { return true; }
        bool                 AsBoolean() override { return GetValue() != 0; }
        std::int64_t         AsInteger() override { return GetValue(); }
        std::uint64_t        AsUnsignedInteger() override { return GetValue(); }
        double               AsFloat() override { return static_cast<double>(GetValue()); }
    };

    struct UnsignedInteger : public virtual Value {
        inline static constexpr auto TYPE_NAME        = "UnsignedInteger";
        inline static constexpr auto PARENT_TYPE_NAME = Value::TYPE_NAME;
        virtual ~UnsignedInteger()                    = default;
        const char*           GetTypeName() override { return TYPE_NAME; }
        ValueType             GetValueType() override { return ValueType::UnsignedInteger; }
        virtual std::uint64_t GetValue()              = 0;
        virtual void          SetValue(std::uint64_t) = 0;
        bool                  IsValueType() override { return true; }
        bool                  IsInteger() override { return true; }
        bool                  IsUnsignedInteger() override { return true; }
        bool                  AsBoolean() override { return GetValue() != 0; }
        std::int64_t          AsInteger() override { return static_cast<std::int64_t>(GetValue()); }
        std::uint64_t         AsUnsignedInteger() override { return GetValue(); }
        double                AsFloat() override { return static_cast<double>(GetValue()); }
    };

    struct Float : public virtual Value {
        inline static constexpr auto TYPE_NAME        = "Float";
        inline static constexpr auto PARENT_TYPE_NAME = Value::TYPE_NAME;
        virtual ~Float()                              = default;
        const char*    GetTypeName() override { return TYPE_NAME; }
        ValueType      GetValueType() override { return ValueType::Float; }
        virtual double GetValue()       = 0;
        virtual void   SetValue(double) = 0;
        bool           IsValueType() override { return true; }
        bool           IsFloat() override { return true; }
        bool           AsBoolean() override { return GetValue() != 0.0; }
        std::int64_t   AsInteger() override { return static_cast<std::int64_t>(GetValue()); }
        std::uint64_t  AsUnsignedInteger() override {
            return static_cast<std::uint64_t>(GetValue());
        }
        double AsFloat() override { return GetValue(); }
    };

    struct String : public virtual Value {
        inline static constexpr auto TYPE_NAME        = "String";
        inline static constexpr auto PARENT_TYPE_NAME = Value::TYPE_NAME;
        virtual ~String()                             = default;
        const char*         GetTypeName() override { return TYPE_NAME; }
        ValueType           GetValueType() override { return ValueType::String; }
        virtual const char* GetValue()            = 0;
        virtual void        SetValue(const char*) = 0;
        bool                IsValueType() override { return true; }
        bool                IsString() override { return true; }
        bool                AsBoolean() override { return GetValue() != nullptr; }
        std::int64_t        AsInteger() override { return GetValue() != nullptr; }
        std::uint64_t       AsUnsignedInteger() override { return GetValue() != nullptr; }
        double              AsFloat() override { return GetValue() != nullptr; }
        const char*         AsString() override { return GetValue(); }
    };

    struct None : public virtual Value {
        inline static constexpr auto TYPE_NAME        = "None";
        inline static constexpr auto PARENT_TYPE_NAME = Value::TYPE_NAME;
        virtual ~None()                               = default;
        const char*   GetTypeName() override { return TYPE_NAME; }
        ValueType     GetValueType() override { return ValueType::None; }
        bool          IsValueType() override { return true; }
        bool          IsNone() override { return true; }
        bool          AsBoolean() override { return false; }
        std::int64_t  AsInteger() override { return 0; }
        std::uint64_t AsUnsignedInteger() override { return 0; }
        double        AsFloat() override { return 0.0; }
    };

    struct Void : public virtual None {
        inline static constexpr auto TYPE_NAME        = "Void";
        inline static constexpr auto PARENT_TYPE_NAME = None::TYPE_NAME;
        virtual ~Void()                               = default;
        const char* GetTypeName() override { return TYPE_NAME; }
        ValueType   GetValueType() override { return ValueType::Void; }
        bool        IsValueType() override { return true; }
        bool        IsVoid() override { return true; }
    };

    struct Type : public virtual Value {
        inline static constexpr auto TYPE_NAME        = "Type";
        inline static constexpr auto PARENT_TYPE_NAME = Value::TYPE_NAME;
        virtual ~Type()                               = default;
        const char*         GetTypeName() override { return TYPE_NAME; }
        ValueType           GetValueType() override { return ValueType::Type; }
        bool                IsType() override { return true; }
        bool                IsValueType() override { return true; }
        virtual const char* GetName() = 0;
    };

    struct Namespace : public virtual Value {
        inline static constexpr auto TYPE_NAME        = "Namespace";
        inline static constexpr auto PARENT_TYPE_NAME = Value::TYPE_NAME;
        virtual ~Namespace()                          = default;
        const char*                 GetTypeName() override { return TYPE_NAME; }
        ValueType                   GetValueType() override { return ValueType::Namespace; }
        bool                        IsNamespace() override { return true; }
        bool                        IsValueType() override { return true; }
        virtual const char*         GetName()      = 0;
        virtual VariableCollection* GetVariables() = 0;
    };

    struct Error : public virtual Value {
        inline static constexpr auto TYPE_NAME        = "Error";
        inline static constexpr auto PARENT_TYPE_NAME = Value::TYPE_NAME;
        virtual ~Error()                              = default;
        const char*         GetTypeName() override { return TYPE_NAME; }
        ValueType           GetValueType() override { return ValueType::Error; }
        bool                IsError() override { return true; }
        virtual const char* GetErrorMessage() = 0;
        virtual void*       GetErrorData()    = 0;
        virtual Value*      GetErrorValue()   = 0;
        const char*         AsString() override { return GetErrorMessage(); }
    };

    struct Function : public virtual Value {
        inline static constexpr auto TYPE_NAME        = "Function";
        inline static constexpr auto PARENT_TYPE_NAME = Value::TYPE_NAME;
        virtual ~Function()                           = default;
        const char*         GetTypeName() override { return TYPE_NAME; }
        ValueType           GetValueType() override { return ValueType::Function; }
        virtual FunctionFn* GetValue()            = 0;
        virtual void        SetValue(FunctionFn*) = 0;
        bool                IsFunction() override { return true; }
        bool                IsCallable() override { return true; }
    };

    struct StaticFunction : public virtual Value {
        inline static constexpr auto TYPE_NAME        = "StaticFunction";
        inline static constexpr auto PARENT_TYPE_NAME = Value::TYPE_NAME;
        virtual ~StaticFunction()                     = default;
        const char*               GetTypeName() override { return TYPE_NAME; }
        ValueType                 GetValueType() override { return ValueType::Function; }
        virtual StaticFunctionFn* GetValue()                  = 0;
        virtual void              SetValue(StaticFunctionFn*) = 0;
        bool                      IsFunction() override { return true; }
        bool                      IsCallable() override { return true; }
    };

    struct InstanceFunction : public virtual Value {
        inline static constexpr auto TYPE_NAME        = "InstanceFunction";
        inline static constexpr auto PARENT_TYPE_NAME = Value::TYPE_NAME;
        virtual ~InstanceFunction()                   = default;
        const char*                 GetTypeName() override { return TYPE_NAME; }
        ValueType                   GetValueType() override { return ValueType::Function; }
        virtual InstanceFunctionFn* GetValue()                    = 0;
        virtual void                SetValue(InstanceFunctionFn*) = 0;
        bool                        IsFunction() override { return true; }
        bool                        IsCallable() override { return true; }
    };

    struct Collection : public virtual Value {
        inline static constexpr auto TYPE_NAME        = "Collection";
        inline static constexpr auto PARENT_TYPE_NAME = Value::TYPE_NAME;
        virtual ~Collection()                         = default;
        const char* GetTypeName() override { return TYPE_NAME; }
        bool        IsCollection() override { return true; }
    };

    struct Map : public virtual Collection {
        using StringForEachValueFn  = IFunctionPointer<void(const char* key, Value* value)>;
        using IntegerForEachValueFn = IFunctionPointer<void(std::uint64_t key, Value* value)>;

        inline static constexpr auto TYPE_NAME        = "Map";
        inline static constexpr auto PARENT_TYPE_NAME = Collection::TYPE_NAME;
        virtual ~Map()                                = default;
        const char* GetTypeName() override { return TYPE_NAME; }
        ValueType   GetValueType() override { return ValueType::Map; }
        bool        IsMap() override { return true; }

        virtual std::uint64_t GetCount() = 0;

        // TODO: don't do this ?

        virtual Value* StringGetValue(const char* key)               = 0;
        virtual void   StringSetValue(const char* key, Value* value) = 0;
        virtual bool   StringRemoveValue(const char* key)            = 0;
        virtual void   StringForEachValue(StringForEachValueFn* fn)  = 0;

        virtual Value* IntegerGetValue(std::int64_t key)               = 0;
        virtual void   IntegerSetValue(std::int64_t key, Value* value) = 0;
        virtual bool   IntegerRemoveValue(std::int64_t key)            = 0;
        virtual void   IntegerForEachValue(IntegerForEachValueFn* fn)  = 0;

        // TODO templates :)
    };

    struct Set : public virtual Collection {
        using ForEachValueFn = IFunctionPointer<void(Value* value)>;

        inline static constexpr auto TYPE_NAME        = "Set";
        inline static constexpr auto PARENT_TYPE_NAME = Collection::TYPE_NAME;
        virtual ~Set()                                = default;
        const char*           GetTypeName() override { return TYPE_NAME; }
        ValueType             GetValueType() override { return ValueType::Set; }
        bool                  IsSet() override { return true; }
        virtual bool          AddValue(Value* value)           = 0;
        virtual bool          RemoveValue(Value* value)        = 0;
        virtual std::uint64_t GetCount()                       = 0;
        virtual void          ForEachValue(ForEachValueFn* fn) = 0;
    };

    struct Array : public virtual Collection {
        using ForEachValueFn = IFunctionPointer<void(std::uint64_t index, Value* value)>;

        inline static constexpr auto TYPE_NAME        = "Array";
        inline static constexpr auto PARENT_TYPE_NAME = Collection::TYPE_NAME;
        virtual ~Array()                              = default;
        const char*           GetTypeName() override { return TYPE_NAME; }
        ValueType             GetValueType() override { return ValueType::Array; }
        bool                  IsArray() override { return true; }
        virtual Value*        GetValue(std::uint64_t index)               = 0;
        virtual void          SetValue(std::uint64_t index, Value* value) = 0;
        virtual void          AddValue(Value* value)                      = 0;
        virtual void          RemoveValue(std::uint64_t index)            = 0;
        virtual std::uint64_t GetCount()                                  = 0;
        virtual void          ForEachValue(ForEachValueFn* fn)            = 0;
    };

    struct Object : public virtual Value {
        inline static constexpr auto TYPE_NAME        = "Object";
        inline static constexpr auto PARENT_TYPE_NAME = Value::TYPE_NAME;
        virtual ~Object()                             = default;

        ValueType     GetValueType() override { return ValueType::Object; }
        bool          IsObject() override { return true; }
        virtual void* GetValue() = 0;

        virtual void CastTo(const char* typeName) = 0;

        template <typename T>
        T GetValueAs() {
            return static_cast<T>(GetValue());
        }
    };

    struct VariableCollection {
        using ForEachVariableFn =
            IFunctionPointer<void(const char* variableName, Value* variableValue)>;

        virtual ~VariableCollection()                              = default;
        virtual void   AddVariable(const char* name, Value* value) = 0;
        virtual bool   RemoveVariable(const char* name)            = 0;
        virtual Value* GetValue(const char* name)                  = 0;
        virtual bool   HasVariable(const char* name)               = 0;
        virtual void   ForEachVariable(ForEachVariableFn* fn)      = 0;
        virtual void   Merge(VariableCollection* other)            = 0;
    };

    struct FunctionArgument {
        virtual ~FunctionArgument()        = default;
        virtual const char*  GetName()     = 0;
        virtual const char*  GetTypeName() = 0;
        virtual std::uint8_t GetIndex()    = 0;
        virtual Value*       GetValue()    = 0;
    };

    struct FunctionArguments {
        using ForEachArgumentFn = IFunctionPointer<void(FunctionArgument*)>;

        virtual ~FunctionArguments()                                          = default;
        virtual std::uint8_t      GetArgumentCount()                          = 0;
        virtual FunctionArgument* GetArgumentByIndex(std::uint8_t index)      = 0;
        virtual FunctionArgument* GetArgumentByName(const char* name)         = 0;
        virtual void              AddArgument(const char* name, Value* value) = 0;
        virtual void              ForEachArgument(ForEachArgumentFn* fn)      = 0;
    };

    struct TypeRegistry {
        using ForEachCallbackFn = IFunctionPointer<void(const char* typeName)>;
        using ForEachTypeAndParentCallbackFn =
            IFunctionPointer<void(const char* childName, const char* parentName)>;

        virtual ~TypeRegistry()                              = default;
        virtual std::uint32_t GetTypeCount()                 = 0;
        virtual void          RegisterType(const char* name) = 0;
        virtual void          AddParentType(const char* childName, const char* parentName)   = 0;
        virtual bool          IsType(const char* name)                                       = 0;
        virtual void          ForEachType(ForEachCallbackFn* callback)                       = 0;
        virtual void          ForEachParent(const char* name, ForEachCallbackFn* callback)   = 0;
        virtual void          ForEachTypeAndParent(ForEachTypeAndParentCallbackFn* callback) = 0;
        virtual void          Merge(TypeRegistry* other)                                     = 0;
    };

    struct InstanceMemberRegistry {
        using ForEachMemberFn = IFunctionPointer<
            void(const char* typeName, const char* memberName, InstanceFunctionFn* fn)>;

        virtual ~InstanceMemberRegistry() = default;
        virtual void AddMember(
            const char* typeName, const char* memberName, InstanceFunctionFn* invocationFn
        )                                                                                   = 0;
        virtual bool                HasMember(const char* typeName, const char* memberName) = 0;
        virtual InstanceFunctionFn* GetMemberFunction(
            const char* typeName, const char* memberName
        )                                                                           = 0;
        virtual void ForEachMember(ForEachMemberFn* callback)                       = 0;
        virtual void ForEachMember(const char* typeName, ForEachMemberFn* callback) = 0;
        virtual void Merge(InstanceMemberRegistry* other)                           = 0;
    };

    struct StaticMemberRegistry {
        using ForEachMemberFn = IFunctionPointer<
            void(const char* typeName, const char* memberName, StaticFunctionFn* fn)>;

        virtual ~StaticMemberRegistry() = default;
        virtual void AddMember(
            const char* typeName, const char* memberName, StaticFunctionFn* invocationFn
        )                                                                                 = 0;
        virtual bool              HasMember(const char* typeName, const char* memberName) = 0;
        virtual StaticFunctionFn* GetMemberFunction(
            const char* typeName, const char* memberName
        )                                                                           = 0;
        virtual void ForEachMember(ForEachMemberFn* callback)                       = 0;
        virtual void ForEachMember(const char* typeName, ForEachMemberFn* callback) = 0;
        virtual void Merge(StaticMemberRegistry* other)                             = 0;
    };

    struct NameLookupHandlers {
        using NameLookupHandlerFn         = IFunctionPointer<Value*(
            const char* nameToLookup, Value* leftHandSideVariableValueIfAny,
            ScriptContext* parentScriptContext
        )>;
        using NameLookupHandlerCallbackFn = IFunctionPointer<void(Value* result)>;
        using ForEachNameHandlerFn        = IFunctionPointer<void(NameLookupHandlerFn*)>;

        virtual ~NameLookupHandlers()                           = default;
        virtual void   AddHandler(NameLookupHandlerFn* handler) = 0;
        virtual Value* LookupName(
            const char* nameToLookup, Value* leftHandSideVariableIfAny,
            ScriptContext* parentScriptContext
        )                                                     = 0;
        virtual void ForEachHandler(ForEachNameHandlerFn* fn) = 0;
        virtual void Merge(NameLookupHandlers* other)         = 0;
    };

    struct NameListHandlers {
        using NameListCallbackFn = IFunctionPointer<void(const char*)>;
        using NameListFn         = IFunctionPointer<void(
            ScriptContext* parentScriptContext, Value* leftHandSideVariableValueIfAny,
            NameListCallbackFn* callback
        )>;
        using ForEachFn          = IFunctionPointer<void(NameListFn*)>;

        virtual ~NameListHandlers()                  = default;
        virtual void AddHandler(NameListFn* handler) = 0;
        virtual void ListNames(
            ScriptContext* parentScriptContext, Value* leftHandSideVariableIfAny,
            NameListCallbackFn* callback
        )                                           = 0;
        virtual void ForEachHandler(ForEachFn* fn)  = 0;
        virtual void Merge(NameListHandlers* other) = 0;
    };

    struct Package {
        virtual ~Package() = default;

        virtual const char* GetName()    = 0;
        virtual const char* GetVersion() = 0;
        virtual const char* GetAuthor()  = 0;

        virtual VariableCollection* GetVariables() = 0;

        virtual TypeRegistry*           GetTypeRegistry()           = 0;
        virtual InstanceMemberRegistry* GetInstanceMemberRegistry() = 0;
        virtual StaticMemberRegistry*   GetStaticMemberRegistry()   = 0;

        virtual NameLookupHandlers* GetNameLookupHandlers() = 0;
        virtual NameListHandlers*   GetNameListHandlers()   = 0;

        virtual PackageImportHandlers* GetPackageImportHandlers() = 0;
        virtual PackageLookupHandlers* GetPackageLookupHandlers() = 0;
    };

    struct ReadOnlyPackageCollection {
        using ForEachPackageFn = IFunctionPointer<void(Package*)>;

        virtual ~ReadOnlyPackageCollection()                       = default;
        virtual Package*      GetPackage(const char* name)         = 0;
        virtual bool          HasPackage(const char* name)         = 0;
        virtual bool          HasPackage(Package*)                 = 0;
        virtual std::uint32_t GetPackageCount()                    = 0;
        virtual void          ForEachPackage(ForEachPackageFn* fn) = 0;
    };

    struct PackageCollection : public ReadOnlyPackageCollection {
        virtual ~PackageCollection()         = default;
        virtual bool AddPackage(Package*)    = 0;
        virtual bool RemovePackage(Package*) = 0;
    };

    struct PackageImportHandlers {
        using PackageImportHandlerFn =
            IFunctionPointer<void(Package* importingPackage, ScriptContext* intoThisContext)>;
        using ForEachHandlerFn = IFunctionPointer<void(PackageImportHandlerFn*)>;

        virtual ~PackageImportHandlers()                                   = default;
        virtual void AddHandler(PackageImportHandlerFn* handler)           = 0;
        virtual bool RunHandlers(Package* package, ScriptContext* context) = 0;
        virtual void ForEachHandler(ForEachHandlerFn* fn)                  = 0;
        virtual void Merge(PackageImportHandlers* other)                   = 0;
    };

    struct PackageLookupHandlers {
        using PackageLookupHandlerFn =
            IFunctionPointer<Package*(const char* requestedPackageName, ScriptContext* context)>;
        using ForEachHandlerFn = IFunctionPointer<void(PackageLookupHandlerFn*)>;

        virtual ~PackageLookupHandlers()                             = default;
        virtual void     AddHandler(PackageLookupHandlerFn* handler) = 0;
        virtual Package* LookupImport(const char* requestedPackageName, ScriptContext* context) = 0;
        virtual void     ForEachHandler(ForEachHandlerFn* fn)                                   = 0;
        virtual void     Merge(PackageLookupHandlers* other)                                    = 0;
    };

    struct ScriptContext {
        virtual ~ScriptContext() = default;

        virtual PurrScriptAPI* GetAPI() = 0;

        // TODO: rename to GlobalVariables
        virtual VariableCollection* GetVariables() = 0;

        // TODO bundle the Type stuff together???

        // TODO: Rename to something more specific like type inheritance
        virtual TypeRegistry* GetTypeRegistry() = 0;

        virtual InstanceMemberRegistry* GetInstanceMemberRegistry() = 0;
        virtual StaticMemberRegistry*   GetStaticMemberRegistry()   = 0;

        virtual NameLookupHandlers* GetNameLookupHandlers() = 0;
        virtual NameListHandlers*   GetNameListHandlers()   = 0;

        virtual PackageImportHandlers* GetPackageImportHandlers() = 0;
        virtual PackageLookupHandlers* GetPackageLookupHandlers() = 0;

        virtual void ImportPackage(Package*)                         = 0;
        virtual void ImportPackage(const char* requestedPackageName) = 0;
        virtual void ImportPackages(ReadOnlyPackageCollection*)      = 0;

        virtual ReadOnlyPackageCollection* GetImportedPackages() = 0;

        virtual ExecuteCodeResult* ExecuteCode(const char* code) = 0;
        virtual ExecuteCodeResult* GetLastExecuteCodeResult()    = 0;

        virtual LanguageHandler* GetLanguageHandler() = 0;
    };

    struct ScriptContextManager {
        using ForEachContextFn = IFunctionPointer<void(ScriptContext*)>;

        virtual ~ScriptContextManager()                                      = default;
        virtual ScriptContext* CreateContext(bool useDefaultPackages = true) = 0;
        virtual ScriptContext* CreateContext(
            LanguageHandler* languageHandler, bool useDefaultPackages = true
        )                                                            = 0;
        virtual bool          DestroyContext(ScriptContext* context) = 0;
        virtual bool          IsContextValid(ScriptContext* context) = 0;
        virtual std::uint32_t GetContextCount()                      = 0;
        virtual void          ForEachContext(ForEachContextFn* fn)   = 0;
    };

    // TODO: give the (each) PurrScriptAPI a GLOBAL context - more flexible than what I have!
    // ... maybe.
    struct PurrScriptAPI {
        virtual ~PurrScriptAPI()                                = default;
        virtual ScriptContextManager* GetScriptContextManager() = 0;
        virtual PackageCollection*    GetPackageRegistry()      = 0;
        virtual PackageCollection*    GetDefaultPackages()      = 0;
        // virtual ValueReferenceCounter* GetValueReferenceCounter() = 0;

        // TODO: move to ScriptContextManager
        virtual void             SetDefaultLanguageHandler(LanguageHandler* languageHandler) = 0;
        virtual LanguageHandler* GetDefaultLanguageHandler()                                 = 0;
    };
}
