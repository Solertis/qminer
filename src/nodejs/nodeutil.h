#ifndef QMINER_NODEJS_UTILS
#define QMINER_NODEJS_UTILS

#include <node.h>
#include <node_object_wrap.h>
#include "base.h"

#define JsDeclareProperty(Function) \
    static void Function(v8::Local<v8::String> Name, const v8::PropertyCallbackInfo<v8::Value>& Info); \
    static void _ ## Function(v8::Local<v8::String> Name, const v8::PropertyCallbackInfo<v8::Value>& Info) { \
        v8::Isolate* Isolate = v8::Isolate::GetCurrent(); \
        v8::HandleScope HandleScope(Isolate); \
        try { \
            Function(Name, Info); \
        } catch (const PExcept& Except) { \
            Isolate->ThrowException(v8::Exception::TypeError( \
            v8::String::NewFromUtf8(Isolate, TStr("[addon] Exception: " + Except->GetMsgStr()).CStr()))); \
        } \
    };

#define JsDeclIndexedProperty(Function) \
    static void Function(uint32_t Index, const v8::PropertyCallbackInfo<v8::Value>& Info); \
    static void _ ## Function(uint32_t Index, const v8::PropertyCallbackInfo<v8::Value>& Info) { \
        v8::Isolate* Isolate = v8::Isolate::GetCurrent(); \
        v8::HandleScope HandleScope(Isolate); \
        try { \
            Function(Index, Info); \
        } catch(const PExcept& Except) { \
            Isolate->ThrowException(v8::Exception::TypeError(\
            v8::String::NewFromUtf8(Isolate, TStr("[addon] Exception: " + Except->GetMsgStr()).CStr()))); \
        } \
    }

#define JsDeclareSetIndexedProperty(FunctionGetter, FunctionSetter) \
    static void FunctionGetter(uint32_t Index, const v8::PropertyCallbackInfo<v8::Value>& Info); \
    static void _ ## FunctionGetter(uint32_t Index, const v8::PropertyCallbackInfo<v8::Value>& Info) { \
        v8::Isolate* Isolate = v8::Isolate::GetCurrent(); \
        v8::HandleScope HandleScope(Isolate); \
        try { \
            FunctionGetter(Index, Info); \
        } catch(const PExcept& Except) { \
            Isolate->ThrowException(v8::Exception::TypeError(\
            v8::String::NewFromUtf8(Isolate, TStr("[addon] Exception: " + Except->GetMsgStr()).CStr()))); \
        } \
    } \
    static void FunctionSetter(uint32_t Index, v8::Local<v8::Value> Value, const v8::PropertyCallbackInfo<void>& Info); \
    static void _ ## FunctionSetter(uint32_t Index, v8::Local<v8::Value> Value, const v8::PropertyCallbackInfo<void>& Info) { \
        v8::Isolate* Isolate = v8::Isolate::GetCurrent(); \
        v8::HandleScope HandleScope(Isolate); \
        try { \
            FunctionSetter(Index, Value, Info); \
        } catch(const PExcept& Except) { \
            Isolate->ThrowException(v8::Exception::TypeError(\
            v8::String::NewFromUtf8(Isolate, TStr("[addon] Exception: " + Except->GetMsgStr()).CStr()))); \
        } \
    }

#define JsDeclareSetProperty(GetFunction, SetFunction) \
    static void GetFunction(v8::Local<v8::String> Name, const v8::PropertyCallbackInfo<v8::Value>& Info); \
    static void _ ## GetFunction(v8::Local<v8::String> Name, const v8::PropertyCallbackInfo<v8::Value>& Info) { \
        v8::Isolate* Isolate = v8::Isolate::GetCurrent(); \
        v8::HandleScope HandleScope(Isolate); \
        try { \
            GetFunction(Name, Info); \
        } catch (const PExcept& Except) { \
            Isolate->ThrowException(v8::Exception::TypeError( \
            v8::String::NewFromUtf8(Isolate, TStr("[addon] Exception: " + Except->GetMsgStr()).CStr()))); \
      } \
    } \
    static void SetFunction(v8::Local<v8::String> Name, v8::Local<v8::Value> Value, const v8::PropertyCallbackInfo<void>& Info); \
    static void _ ## SetFunction(v8::Local<v8::String> Name, v8::Local<v8::Value> Value, const v8::PropertyCallbackInfo<void>& Info) { \
        v8::Isolate* Isolate = v8::Isolate::GetCurrent(); \
        v8::HandleScope HandleScope(Isolate); \
        try { \
            SetFunction(Name, Value, Info); \
        } catch (const PExcept& Except) { \
            Isolate->ThrowException(v8::Exception::TypeError( \
            v8::String::NewFromUtf8(Isolate, TStr("[addon] Exception: " + Except->GetMsgStr()).CStr()))); \
        } \
    };

#define JsDeclareFunction(Function) \
    static void Function(const v8::FunctionCallbackInfo<v8::Value>& Args); \
    static void _ ## Function(const v8::FunctionCallbackInfo<v8::Value>& Args) { \
        v8::Isolate* Isolate = v8::Isolate::GetCurrent(); \
        v8::HandleScope HandleScope(Isolate); \
        try { \
            Function(Args); \
        } catch (const PExcept& Except) { \
            Isolate->ThrowException(v8::Exception::TypeError(\
            v8::String::NewFromUtf8(Isolate, TStr("[addon] Exception: " + Except->GetMsgStr()).CStr()))); \
        } \
    };

#define JsDeclareSpecializedFunction(Function) \
    static void Function(const v8::FunctionCallbackInfo<v8::Value>& Args) { throw TExcept::New("Not implemented!"); } \
    static void _ ## Function(const v8::FunctionCallbackInfo<v8::Value>& Args) { \
        v8::Isolate* Isolate = v8::Isolate::GetCurrent(); \
        v8::HandleScope HandleScope(Isolate); \
        try { \
            Function(Args); \
        } catch (const PExcept& Except) { \
            Isolate->ThrowException(v8::Exception::TypeError(\
            v8::String::NewFromUtf8(Isolate, TStr("[addon] Exception: " + Except->GetMsgStr()).CStr()))); \
        } \
    }; 

//////////////////////////////////////////////////////
// Node - Utilities
class TNodeJsUtil {
public:
    /// Convert v8 Json to GLib Json (PJsonVal). Is parameter IgnoreFunc is set to true the method will
	/// ignore functions otherwise an exception will be thrown when a function is encountered
    static PJsonVal GetObjJson(const v8::Local<v8::Object>& Obj, const bool IgnoreFunc=false);
    static PJsonVal GetObjProps(const v8::Local<v8::Object>& Obj) { return GetObjJson(Obj, true); }
    /// Convert GLib Json (PJsonVal) to v8 Json
    static v8::Local<v8::Value> ParseJson(v8::Isolate* Isolate, const PJsonVal& JsonVal);

    /// Transform V8 string to TStr
    static TStr GetStr(const v8::Local<v8::String>& V8Str);

    /// Gets the class name of the underlying glib object. the name is stored
    /// in an hidden variable "class"
    static TStr GetClass(const v8::Handle<v8::Object> Obj);

    /// Check if argument ArgN exists
    static bool IsArg(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN) { return (Args.Length() > ArgN); }

    /// Checks if the class name of the underlying glib object matches the
    /// given string. the name is stored in an hidden variable "class"
    static bool IsClass(const v8::Handle<v8::Object> Obj, const TStr& ClassNm) ;
    /// Check if argument ArgN belongs to a given class
    static bool IsArgClass(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN, const TStr& ClassNm);
    /// Check if is argument ArgN of type v8::Function
    static bool IsArgFun(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN);
    /// Check if is argument ArgN of type v8::Object
    static bool IsArgObj(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN);
    /// Check if is argument ArgN of type v8::Bool
    static bool IsArgBool(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN);
    /// Check if is argument ArgN of type v8::Number
    static bool IsArgFlt(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN);
    /// Check if is argument ArgN of type v8::String
    static bool IsArgStr(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN);
    /// Check if is argument ArgN is a JSON object
    static bool IsArgJson(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN);

    /// Extract argument ArgN property as bool
    static bool GetArgBool(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN);
    /// Extract argument ArgN property as bool, and use DefVal in case when not present
    static bool GetArgBool(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN, const bool& DefVal);
    /// Extract argument ArgN property as bool
    static bool GetArgBool(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN, const TStr& Property, const bool& DefVal);
    /// Extract argument ArgN as int
    static int GetArgInt32(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN);
    /// Extract argument ArgN as int, and use DefVal in case when not present
    static int GetArgInt32(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN, const int& DefVal);
    /// Extract argument ArgN property as int
    static int GetArgInt32(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN, const TStr& Property, const int& DefVal);

    /// Extract argument ArgN as double
    static double GetArgFlt(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN);
    /// Extract argument ArgN as int, and use DefVal in case when not present
    static double GetArgFlt(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN, const double& DefVal);
    /// Extract argument ArgN property as int
    static double GetArgFlt(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN, const TStr& Property, const double& DefVal);

    /// Extract argument ArgN as TStr
    static TStr GetArgStr(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN);
    /// Extract argument ArgN as TStr, and use DefVal in case when not present
    static TStr GetArgStr(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN, const TStr& DefVal);
    /// Extract argument ArgN property as string
    static TStr GetArgStr(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN, const TStr& Property, const TStr& DefVal);
    /// Extract argument ArgN as GLib Json (PJsonVal)
    static PJsonVal GetArgJson(const v8::FunctionCallbackInfo<v8::Value>& Args, const int& ArgN);
    /// Executes the function with the specified argument and returns a double result.
    static double ExecuteFlt(const v8::Handle<v8::Function>& Fun, const v8::Local<v8::Object>& Arg);

    /// Create a new Javascript instance wrapped by the wrapper class
    template <class TWrap>
    static v8::Local<v8::Object> NewJsInstance(TWrap* Wrapper, v8::Persistent<v8::Function>& ConsFun, v8::Isolate* Isolate, v8::EscapableHandleScope& HandleScope);
    /// Convenience method, creates a handle scope. If a handle scope is already present, the method
    /// which takes it as an argument should be used.
    template <class TWrap>
    static v8::Local<v8::Object> NewJsInstance(TWrap* Wrapper, v8::Persistent<v8::Function>& ConsFun, v8::Isolate* Isolate);
};

template <class TWrap>
v8::Local<v8::Object> TNodeJsUtil::NewJsInstance(TWrap* Wrapper, v8::Persistent<v8::Function>& ConsFun, v8::Isolate* Isolate, v8::EscapableHandleScope& HandleScope) {
	v8::Local<v8::Function> Cons = v8::Local<v8::Function>::New(Isolate, ConsFun);
	v8::Local<v8::Object> Instance = Cons->NewInstance();
	Wrapper->Wrap(Instance);
	return HandleScope.Escape(Instance);
}

template <class TWrap>
v8::Local<v8::Object> TNodeJsUtil::NewJsInstance(TWrap* Wrapper, v8::Persistent<v8::Function>& ConsFun, v8::Isolate* Isolate) {
	v8::EscapableHandleScope HandleScope(Isolate);
	return NewJsInstance(Wrapper, ConsFun, Isolate, HandleScope);
}

#endif

