/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CSSSVGDocumentValue_h
#define CSSSVGDocumentValue_h

#include "core/css/CSSValue.h"
#include "core/fetch/DocumentResource.h"

namespace blink {

class Document;

class CSSSVGDocumentValue : public CSSValue {
public:
    static CSSSVGDocumentValue* create(const String& url)
    {
        return new CSSSVGDocumentValue(url);
    }
    ~CSSSVGDocumentValue();

    DocumentResource* cachedSVGDocument() const { return m_document.get(); }
    DocumentResource* load(Document*) const;

    String customCSSText() const;
    const String& url() const { return m_url; }
    bool loadRequested() const { return m_loadRequested; }
    bool equals(const CSSSVGDocumentValue&) const;

    DEFINE_INLINE_TRACE_AFTER_DISPATCH()
    {
        visitor->trace(m_document);
        CSSValue::traceAfterDispatch(visitor);
    }

private:
    CSSSVGDocumentValue(const String& url);

    String m_url;

    // Document cache.
    mutable Member<DocumentResource> m_document;
    mutable bool m_loadRequested;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSSVGDocumentValue, isSVGDocumentValue());

} // namespace blink

#endif // CSSSVGDocumentValue_h
