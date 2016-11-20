/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "bindings/core/v8/V8ErrorHandler.h"

#include "bindings/core/v8/ScriptController.h"
#include "bindings/core/v8/V8Binding.h"
#include "bindings/core/v8/V8ErrorEvent.h"
#include "bindings/core/v8/V8PrivateProperty.h"
#include "bindings/core/v8/V8ScriptRunner.h"
#include "core/dom/ExecutionContext.h"

namespace blink {

V8ErrorHandler::V8ErrorHandler(bool isInline, ScriptState* scriptState)
    : V8EventListener(isInline, scriptState)
{
}

v8::Local<v8::Value> V8ErrorHandler::callListenerFunction(ScriptState* scriptState, v8::Local<v8::Value> jsEvent, Event* event)
{
    ASSERT(!jsEvent.IsEmpty());
    if (!event->hasInterface(EventNames::ErrorEvent))
        return V8EventListener::callListenerFunction(scriptState, jsEvent, event);

    ErrorEvent* errorEvent = static_cast<ErrorEvent*>(event);
    if (errorEvent->world() && errorEvent->world() != &world())
        return v8::Null(isolate());

    v8::Local<v8::Object> listener = getListenerObject(scriptState->getExecutionContext());
    if (listener.IsEmpty() || !listener->IsFunction())
        return v8::Null(isolate());

    v8::Local<v8::Function> callFunction = v8::Local<v8::Function>::Cast(listener);
    v8::Local<v8::Object> thisValue = scriptState->context()->Global();

    v8::Local<v8::Object> eventObject;
    if (!jsEvent->ToObject(scriptState->context()).ToLocal(&eventObject))
        return v8::Null(isolate());
    auto privateError = V8PrivateProperty::getErrorEventError(isolate());
    v8::Local<v8::Value> error = privateError.getOrUndefined(scriptState->context(), eventObject);
    if (error->IsUndefined())
        error = v8::Null(isolate());

    v8::Local<v8::Value> parameters[5] = { v8String(isolate(), errorEvent->message()), v8String(isolate(), errorEvent->filename()), v8::Integer::New(isolate(), errorEvent->lineno()), v8::Integer::New(isolate(), errorEvent->colno()), error };
    v8::TryCatch tryCatch(isolate());
    tryCatch.SetVerbose(true);
    v8::MaybeLocal<v8::Value> result;
    if (scriptState->getExecutionContext()->isWorkerGlobalScope()) {
        result = V8ScriptRunner::callFunction(callFunction, scriptState->getExecutionContext(), thisValue, WTF_ARRAY_LENGTH(parameters), parameters, isolate());
    } else {
        result = ScriptController::callFunction(scriptState->getExecutionContext(), callFunction, thisValue, WTF_ARRAY_LENGTH(parameters), parameters, isolate());
    }
    v8::Local<v8::Value> returnValue;
    if (!result.ToLocal(&returnValue))
        return v8::Null(isolate());

    return returnValue;
}

// static
void V8ErrorHandler::storeExceptionOnErrorEventWrapper(ScriptState* scriptState, ErrorEvent* event, v8::Local<v8::Value> data, v8::Local<v8::Object> creationContext)
{
    v8::Local<v8::Value> wrappedEvent = toV8(event, creationContext, scriptState->isolate());
    if (wrappedEvent.IsEmpty())
        return;

    DCHECK(wrappedEvent->IsObject());
    auto privateError = V8PrivateProperty::getErrorEventError(scriptState->isolate());
    privateError.set(scriptState->context(), wrappedEvent.As<v8::Object>(), data);
}

bool V8ErrorHandler::shouldPreventDefault(v8::Local<v8::Value> returnValue)
{
    return returnValue->IsBoolean() && returnValue.As<v8::Boolean>()->Value();
}

} // namespace blink
