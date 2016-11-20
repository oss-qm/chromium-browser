/*
 * Copyright (C) 2005, 2006, 2008 Apple Inc. All rights reserved.
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
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MoveSelectionCommand_h
#define MoveSelectionCommand_h

#include "core/editing/commands/CompositeEditCommand.h"

namespace blink {

class DocumentFragment;

class MoveSelectionCommand final : public CompositeEditCommand {
public:
    static MoveSelectionCommand* create(DocumentFragment* fragment, const Position& position, bool smartInsert = false, bool smartDelete = false)
    {
        return new MoveSelectionCommand(fragment, position, smartInsert, smartDelete);
    }

    DECLARE_VIRTUAL_TRACE();

private:
    MoveSelectionCommand(DocumentFragment*, const Position&, bool smartInsert, bool smartDelete);

    void doApply(EditingState*) override;
    EditAction editingAction() const override;

    Member<DocumentFragment> m_fragment;
    Position m_position;
    bool m_smartInsert;
    bool m_smartDelete;
};

} // namespace blink

#endif // MoveSelectionCommand_h
