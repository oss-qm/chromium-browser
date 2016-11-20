// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

#include "V8ArrayBufferView.h"

#include "bindings/core/v8/ExceptionState.h"
#include "bindings/core/v8/V8ArrayBuffer.h"
#include "bindings/core/v8/V8DOMConfiguration.h"
#include "bindings/core/v8/V8DataView.h"
#include "bindings/core/v8/V8Float32Array.h"
#include "bindings/core/v8/V8Float64Array.h"
#include "bindings/core/v8/V8HiddenValue.h"
#include "bindings/core/v8/V8Int16Array.h"
#include "bindings/core/v8/V8Int32Array.h"
#include "bindings/core/v8/V8Int8Array.h"
#include "bindings/core/v8/V8ObjectConstructor.h"
#include "bindings/core/v8/V8SharedArrayBuffer.h"
#include "bindings/core/v8/V8Uint16Array.h"
#include "bindings/core/v8/V8Uint32Array.h"
#include "bindings/core/v8/V8Uint8Array.h"
#include "bindings/core/v8/V8Uint8ClampedArray.h"
#include "core/dom/Document.h"
#include "wtf/GetPtr.h"
#include "wtf/RefPtr.h"

namespace blink {

// Suppress warning: global constructors, because struct WrapperTypeInfo is trivial
// and does not depend on another global objects.
#if defined(COMPONENT_BUILD) && defined(WIN32) && COMPILER(CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif
const WrapperTypeInfo V8ArrayBufferView::wrapperTypeInfo = { gin::kEmbedderBlink, 0, V8ArrayBufferView::trace, V8ArrayBufferView::traceWrappers, 0, 0, V8ArrayBufferView::preparePrototypeAndInterfaceObject, nullptr, "ArrayBufferView", 0, WrapperTypeInfo::WrapperTypeObjectPrototype, WrapperTypeInfo::ObjectClassId, WrapperTypeInfo::NotInheritFromEventTarget, WrapperTypeInfo::Independent };
#if defined(COMPONENT_BUILD) && defined(WIN32) && COMPILER(CLANG)
#pragma clang diagnostic pop
#endif

// This static member must be declared by DEFINE_WRAPPERTYPEINFO in TestArrayBufferView.h.
// For details, see the comment of DEFINE_WRAPPERTYPEINFO in
// bindings/core/v8/ScriptWrappable.h.
const WrapperTypeInfo& TestArrayBufferView::s_wrapperTypeInfo = V8ArrayBufferView::wrapperTypeInfo;

TestArrayBufferView* V8ArrayBufferView::toImpl(v8::Local<v8::Object> object)
{
    ASSERT(object->IsArrayBufferView());
    ScriptWrappable* scriptWrappable = toScriptWrappable(object);
    if (scriptWrappable)
        return scriptWrappable->toImpl<TestArrayBufferView>();

    if (object->IsInt8Array())
        return V8Int8Array::toImpl(object);
    if (object->IsInt16Array())
        return V8Int16Array::toImpl(object);
    if (object->IsInt32Array())
        return V8Int32Array::toImpl(object);
    if (object->IsUint8Array())
        return V8Uint8Array::toImpl(object);
    if (object->IsUint8ClampedArray())
        return V8Uint8ClampedArray::toImpl(object);
    if (object->IsUint16Array())
        return V8Uint16Array::toImpl(object);
    if (object->IsUint32Array())
        return V8Uint32Array::toImpl(object);
    if (object->IsFloat32Array())
        return V8Float32Array::toImpl(object);
    if (object->IsFloat64Array())
        return V8Float64Array::toImpl(object);
    if (object->IsDataView())
        return V8DataView::toImpl(object);

    ASSERT_NOT_REACHED();
    return 0;
}

TestArrayBufferView* V8ArrayBufferView::toImplWithTypeCheck(v8::Isolate* isolate, v8::Local<v8::Value> value)
{
    return value->IsArrayBufferView() ? toImpl(v8::Local<v8::Object>::Cast(value)) : nullptr;
}

} // namespace blink
