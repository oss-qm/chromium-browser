// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

#include "V8TestDictionaryDerived.h"

#include "bindings/core/v8/ExceptionState.h"
#include "bindings/core/v8/V8TestDictionary.h"

namespace blink {

void V8TestDictionaryDerivedImplementedAs::toImpl(v8::Isolate* isolate, v8::Local<v8::Value> v8Value, TestDictionaryDerivedImplementedAs& impl, ExceptionState& exceptionState)
{
    if (isUndefinedOrNull(v8Value)) {
        exceptionState.throwTypeError("Missing required member(s): requiredLongMember.");
        return;
    }
    if (!v8Value->IsObject()) {
        exceptionState.throwTypeError("cannot convert to dictionary.");
        return;
    }

    V8TestDictionary::toImpl(isolate, v8Value, impl, exceptionState);
    if (exceptionState.hadException())
        return;

    v8::TryCatch block(isolate);
    v8::Local<v8::Object> v8Object;
    if (!v8Call(v8Value->ToObject(isolate->GetCurrentContext()), v8Object, block)) {
        exceptionState.rethrowV8Exception(block.Exception());
        return;
    }
    v8::Local<v8::Value> derivedStringMemberValue;
    if (!v8Object->Get(isolate->GetCurrentContext(), v8String(isolate, "derivedStringMember")).ToLocal(&derivedStringMemberValue)) {
        exceptionState.rethrowV8Exception(block.Exception());
        return;
    }
    if (derivedStringMemberValue.IsEmpty() || derivedStringMemberValue->IsUndefined()) {
        // Do nothing.
    } else {
        V8StringResource<> derivedStringMember = derivedStringMemberValue;
        if (!derivedStringMember.prepare(exceptionState))
            return;
        impl.setDerivedStringMember(derivedStringMember);
    }

    v8::Local<v8::Value> derivedStringMemberWithDefaultValue;
    if (!v8Object->Get(isolate->GetCurrentContext(), v8String(isolate, "derivedStringMemberWithDefault")).ToLocal(&derivedStringMemberWithDefaultValue)) {
        exceptionState.rethrowV8Exception(block.Exception());
        return;
    }
    if (derivedStringMemberWithDefaultValue.IsEmpty() || derivedStringMemberWithDefaultValue->IsUndefined()) {
        // Do nothing.
    } else {
        V8StringResource<> derivedStringMemberWithDefault = derivedStringMemberWithDefaultValue;
        if (!derivedStringMemberWithDefault.prepare(exceptionState))
            return;
        impl.setDerivedStringMemberWithDefault(derivedStringMemberWithDefault);
    }

    v8::Local<v8::Value> requiredLongMemberValue;
    if (!v8Object->Get(isolate->GetCurrentContext(), v8String(isolate, "requiredLongMember")).ToLocal(&requiredLongMemberValue)) {
        exceptionState.rethrowV8Exception(block.Exception());
        return;
    }
    if (requiredLongMemberValue.IsEmpty() || requiredLongMemberValue->IsUndefined()) {
        exceptionState.throwTypeError("required member requiredLongMember is undefined.");
        return;
    } else {
        int requiredLongMember = toInt32(isolate, requiredLongMemberValue, NormalConversion, exceptionState);
        if (exceptionState.hadException())
            return;
        impl.setRequiredLongMember(requiredLongMember);
    }

    v8::Local<v8::Value> stringOrDoubleSequenceMemberValue;
    if (!v8Object->Get(isolate->GetCurrentContext(), v8String(isolate, "stringOrDoubleSequenceMember")).ToLocal(&stringOrDoubleSequenceMemberValue)) {
        exceptionState.rethrowV8Exception(block.Exception());
        return;
    }
    if (stringOrDoubleSequenceMemberValue.IsEmpty() || stringOrDoubleSequenceMemberValue->IsUndefined()) {
        // Do nothing.
    } else {
        HeapVector<StringOrDouble> stringOrDoubleSequenceMember = toImplArray<HeapVector<StringOrDouble>>(stringOrDoubleSequenceMemberValue, 0, isolate, exceptionState);
        if (exceptionState.hadException())
            return;
        impl.setStringOrDoubleSequenceMember(stringOrDoubleSequenceMember);
    }

}

v8::Local<v8::Value> toV8(const TestDictionaryDerivedImplementedAs& impl, v8::Local<v8::Object> creationContext, v8::Isolate* isolate)
{
    v8::Local<v8::Object> v8Object = v8::Object::New(isolate);
    if (!toV8TestDictionary(impl, v8Object, creationContext, isolate))
        return v8::Local<v8::Value>();
    if (!toV8TestDictionaryDerivedImplementedAs(impl, v8Object, creationContext, isolate))
        return v8::Local<v8::Value>();
    return v8Object;
}

bool toV8TestDictionaryDerivedImplementedAs(const TestDictionaryDerivedImplementedAs& impl, v8::Local<v8::Object> dictionary, v8::Local<v8::Object> creationContext, v8::Isolate* isolate)
{
    if (impl.hasDerivedStringMember()) {
        if (!v8CallBoolean(dictionary->CreateDataProperty(isolate->GetCurrentContext(), v8String(isolate, "derivedStringMember"), v8String(isolate, impl.derivedStringMember()))))
            return false;
    }

    if (impl.hasDerivedStringMemberWithDefault()) {
        if (!v8CallBoolean(dictionary->CreateDataProperty(isolate->GetCurrentContext(), v8String(isolate, "derivedStringMemberWithDefault"), v8String(isolate, impl.derivedStringMemberWithDefault()))))
            return false;
    } else {
        if (!v8CallBoolean(dictionary->CreateDataProperty(isolate->GetCurrentContext(), v8String(isolate, "derivedStringMemberWithDefault"), v8String(isolate, String("default string value")))))
            return false;
    }

    if (impl.hasRequiredLongMember()) {
        if (!v8CallBoolean(dictionary->CreateDataProperty(isolate->GetCurrentContext(), v8String(isolate, "requiredLongMember"), v8::Integer::New(isolate, impl.requiredLongMember()))))
            return false;
    } else {
        ASSERT_NOT_REACHED();
    }

    if (impl.hasStringOrDoubleSequenceMember()) {
        if (!v8CallBoolean(dictionary->CreateDataProperty(isolate->GetCurrentContext(), v8String(isolate, "stringOrDoubleSequenceMember"), toV8(impl.stringOrDoubleSequenceMember(), creationContext, isolate))))
            return false;
    }

    return true;
}

TestDictionaryDerivedImplementedAs NativeValueTraits<TestDictionaryDerivedImplementedAs>::nativeValue(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState)
{
    TestDictionaryDerivedImplementedAs impl;
    V8TestDictionaryDerivedImplementedAs::toImpl(isolate, value, impl, exceptionState);
    return impl;
}

} // namespace blink
