// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

#include "V8TestInterfaceConstructor4.h"

#include "bindings/core/v8/ExceptionState.h"
#include "bindings/core/v8/V8DOMConfiguration.h"
#include "bindings/core/v8/V8ObjectConstructor.h"
#include "bindings/core/v8/V8TestInterfaceConstructor4.h"
#include "core/dom/Document.h"
#include "core/frame/LocalDOMWindow.h"
#include "wtf/GetPtr.h"
#include "wtf/RefPtr.h"

namespace blink {

// Suppress warning: global constructors, because struct WrapperTypeInfo is trivial
// and does not depend on another global objects.
#if defined(COMPONENT_BUILD) && defined(WIN32) && COMPILER(CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif
const WrapperTypeInfo V8TestInterfaceConstructor4::wrapperTypeInfo = { gin::kEmbedderBlink, V8TestInterfaceConstructor4::domTemplate, V8TestInterfaceConstructor4::trace, V8TestInterfaceConstructor4::traceWrappers, 0, 0, V8TestInterfaceConstructor4::preparePrototypeAndInterfaceObject, nullptr, "TestInterfaceConstructor4", 0, WrapperTypeInfo::WrapperTypeObjectPrototype, WrapperTypeInfo::ObjectClassId, WrapperTypeInfo::NotInheritFromEventTarget, WrapperTypeInfo::Independent };
#if defined(COMPONENT_BUILD) && defined(WIN32) && COMPILER(CLANG)
#pragma clang diagnostic pop
#endif

// This static member must be declared by DEFINE_WRAPPERTYPEINFO in TestInterfaceConstructor4.h.
// For details, see the comment of DEFINE_WRAPPERTYPEINFO in
// bindings/core/v8/ScriptWrappable.h.
const WrapperTypeInfo& TestInterfaceConstructor4::s_wrapperTypeInfo = V8TestInterfaceConstructor4::wrapperTypeInfo;

namespace TestInterfaceConstructor4V8Internal {

static void constructor1(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestInterfaceConstructor4* testInterface4Arg;
    {
        testInterface4Arg = V8TestInterfaceConstructor4::toImplWithTypeCheck(info.GetIsolate(), info[0]);
        if (!testInterface4Arg) {
            V8ThrowException::throwTypeError(info.GetIsolate(), ExceptionMessages::failedToConstruct("TestInterfaceConstructor4", "parameter 1 is not of type 'TestInterfaceConstructor4'."));
            return;
        }
    }
    TestInterfaceConstructor4* impl = TestInterfaceConstructor4::create(testInterface4Arg);
    v8::Local<v8::Object> wrapper = info.Holder();
    wrapper = impl->associateWithWrapper(info.GetIsolate(), &V8TestInterfaceConstructor4::wrapperTypeInfo, wrapper);
    v8SetReturnValue(info, wrapper);
}

static void constructor2(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ConstructionContext, "TestInterfaceConstructor4", info.Holder(), info.GetIsolate());
    V8StringResource<> usvStringArg;
    {
        usvStringArg = toUSVString(info.GetIsolate(), info[0], exceptionState);
        if (exceptionState.throwIfNeeded())
            return;
    }
    TestInterfaceConstructor4* impl = TestInterfaceConstructor4::create(usvStringArg);
    v8::Local<v8::Object> wrapper = info.Holder();
    wrapper = impl->associateWithWrapper(info.GetIsolate(), &V8TestInterfaceConstructor4::wrapperTypeInfo, wrapper);
    v8SetReturnValue(info, wrapper);
}

static void constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ConstructionContext, "TestInterfaceConstructor4", info.Holder(), info.GetIsolate());
    switch (std::min(1, info.Length())) {
    case 1:
        if (V8TestInterfaceConstructor4::hasInstance(info[0], info.GetIsolate())) {
            TestInterfaceConstructor4V8Internal::constructor1(info);
            return;
        }
        if (true) {
            TestInterfaceConstructor4V8Internal::constructor2(info);
            return;
        }
        break;
    default:
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    exceptionState.throwTypeError("No matching constructor signature.");
    exceptionState.throwIfNeeded();
}

} // namespace TestInterfaceConstructor4V8Internal

void V8TestInterfaceConstructor4::constructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (!info.IsConstructCall()) {
        V8ThrowException::throwTypeError(info.GetIsolate(), ExceptionMessages::constructorNotCallableAsFunction("TestInterfaceConstructor4"));
        return;
    }

    if (ConstructorMode::current(info.GetIsolate()) == ConstructorMode::WrapExistingObject) {
        v8SetReturnValue(info, info.Holder());
        return;
    }

    TestInterfaceConstructor4V8Internal::constructor(info);
}

static void installV8TestInterfaceConstructor4Template(v8::Isolate* isolate, const DOMWrapperWorld& world, v8::Local<v8::FunctionTemplate> interfaceTemplate)
{
    // Initialize the interface object's template.
    V8DOMConfiguration::initializeDOMInterfaceTemplate(isolate, interfaceTemplate, V8TestInterfaceConstructor4::wrapperTypeInfo.interfaceName, v8::Local<v8::FunctionTemplate>(), V8TestInterfaceConstructor4::internalFieldCount);
    interfaceTemplate->SetCallHandler(V8TestInterfaceConstructor4::constructorCallback);
    interfaceTemplate->SetLength(1);
    v8::Local<v8::Signature> signature = v8::Signature::New(isolate, interfaceTemplate);
    ALLOW_UNUSED_LOCAL(signature);
    v8::Local<v8::ObjectTemplate> instanceTemplate = interfaceTemplate->InstanceTemplate();
    ALLOW_UNUSED_LOCAL(instanceTemplate);
    v8::Local<v8::ObjectTemplate> prototypeTemplate = interfaceTemplate->PrototypeTemplate();
    ALLOW_UNUSED_LOCAL(prototypeTemplate);
    // Register DOM constants, attributes and operations.
}

v8::Local<v8::FunctionTemplate> V8TestInterfaceConstructor4::domTemplate(v8::Isolate* isolate, const DOMWrapperWorld& world)
{
    return V8DOMConfiguration::domClassTemplate(isolate, world, const_cast<WrapperTypeInfo*>(&wrapperTypeInfo), installV8TestInterfaceConstructor4Template);
}


bool V8TestInterfaceConstructor4::hasInstance(v8::Local<v8::Value> v8Value, v8::Isolate* isolate)
{
    return V8PerIsolateData::from(isolate)->hasInstance(&wrapperTypeInfo, v8Value);
}

v8::Local<v8::Object> V8TestInterfaceConstructor4::findInstanceInPrototypeChain(v8::Local<v8::Value> v8Value, v8::Isolate* isolate)
{
    return V8PerIsolateData::from(isolate)->findInstanceInPrototypeChain(&wrapperTypeInfo, v8Value);
}

TestInterfaceConstructor4* V8TestInterfaceConstructor4::toImplWithTypeCheck(v8::Isolate* isolate, v8::Local<v8::Value> value)
{
    return hasInstance(value, isolate) ? toImpl(v8::Local<v8::Object>::Cast(value)) : nullptr;
}

} // namespace blink
