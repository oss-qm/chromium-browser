/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
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

#include "modules/filesystem/EntryBase.h"

#include "modules/filesystem/DOMFilePath.h"
#include "modules/filesystem/DOMFileSystemBase.h"
#include "platform/weborigin/SecurityOrigin.h"

namespace blink {

EntryBase::EntryBase(DOMFileSystemBase* fileSystem, const String& fullPath)
    : m_fileSystem(fileSystem)
    , m_fullPath(fullPath)
    , m_name(DOMFilePath::getName(fullPath))
{
}

EntryBase::~EntryBase()
{
}

String EntryBase::toURL() const
{
    if (!m_cachedURL.isNull())
        return m_cachedURL;

    // Some filesystem type may not support toURL.
    if (!m_fileSystem->supportsToURL())
        m_cachedURL = emptyString();
    else
        m_cachedURL = m_fileSystem->createFileSystemURL(this).getString();
    return m_cachedURL;
}

DEFINE_TRACE(EntryBase)
{
    visitor->trace(m_fileSystem);
}

} // namespace blink
