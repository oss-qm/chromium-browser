// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_SYNC_ENCRYPTION_HANDLER_H_
#define SYNC_INTERNAL_API_PUBLIC_SYNC_ENCRYPTION_HANDLER_H_

#include <string>

#include "base/time/time.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/protocol/sync.pb.h"

namespace syncer {

class Cryptographer;

// Reasons due to which Cryptographer might require a passphrase.
enum PassphraseRequiredReason {
  REASON_PASSPHRASE_NOT_REQUIRED = 0,  // Initial value.
  REASON_ENCRYPTION = 1,               // The cryptographer requires a
                                       // passphrase for its first attempt at
                                       // encryption. Happens only during
                                       // migration or upgrade.
  REASON_DECRYPTION = 2,               // The cryptographer requires a
                                       // passphrase for its first attempt at
                                       // decryption.
};

// The different states for the encryption passphrase. These control if and how
// the user should be prompted for a decryption passphrase.
// Do not re-order or delete these entries; they are used in a UMA histogram.
// Please edit SyncPassphraseType in histograms.xml if a value is added.
enum PassphraseType {
  IMPLICIT_PASSPHRASE = 0,             // GAIA-based passphrase (deprecated).
  KEYSTORE_PASSPHRASE = 1,             // Keystore passphrase.
  FROZEN_IMPLICIT_PASSPHRASE = 2,      // Frozen GAIA passphrase.
  CUSTOM_PASSPHRASE = 3,               // User-provided passphrase.
  PASSPHRASE_TYPE_SIZE,                // The size of this enum; keep last.
};

// Enum used to distinguish which bootstrap encryption token is being updated.
enum BootstrapTokenType {
  PASSPHRASE_BOOTSTRAP_TOKEN,
  KEYSTORE_BOOTSTRAP_TOKEN
};

// Sync's encryption handler. Handles tracking encrypted types, ensuring the
// cryptographer encrypts with the proper key and has the most recent keybag,
// and keeps the nigori node up to date.
// Implementations of this class must be assumed to be non-thread-safe. All
// methods must be invoked on the sync thread.
class SYNC_EXPORT SyncEncryptionHandler {
 public:
  class NigoriState;

  // All Observer methods are done synchronously from within a transaction and
  // on the sync thread.
  class SYNC_EXPORT Observer {
   public:
    Observer();

    // Called when user interaction is required to obtain a valid passphrase.
    // - If the passphrase is required for encryption, |reason| will be
    //   REASON_ENCRYPTION.
    // - If the passphrase is required for the decryption of data that has
    //   already been encrypted, |reason| will be REASON_DECRYPTION.
    // - If the passphrase is required because decryption failed, and a new
    //   passphrase is required, |reason| will be REASON_SET_PASSPHRASE_FAILED.
    //
    // |pending_keys| is a copy of the cryptographer's pending keys, that may be
    // cached by the frontend for subsequent use by the UI.
    virtual void OnPassphraseRequired(
        PassphraseRequiredReason reason,
        const sync_pb::EncryptedData& pending_keys) = 0;
    // Called when the passphrase provided by the user has been accepted and is
    // now used to encrypt sync data.

    virtual void OnPassphraseAccepted() = 0;
    // |bootstrap_token| is an opaque base64 encoded representation of the key
    // generated by the current passphrase, and is provided to the observer for
    // persistence purposes and use in a future initialization of sync (e.g.
    // after restart). The boostrap token will always be derived from the most
    // recent GAIA password (for accounts with implicit passphrases), even if
    // the data is still encrypted with an older GAIA password. For accounts
    // with explicit passphrases, it will be the most recently seen custom
    // passphrase.
    virtual void OnBootstrapTokenUpdated(
        const std::string& bootstrap_token,
        BootstrapTokenType type) = 0;

    // Called when the set of encrypted types or the encrypt
    // everything flag has been changed.  Note that encryption isn't
    // complete until the OnEncryptionComplete() notification has been
    // sent (see below).
    //
    // |encrypted_types| will always be a superset of
    // Cryptographer::SensitiveTypes().  If |encrypt_everything| is
    // true, |encrypted_types| will be the set of all known types.
    //
    // Until this function is called, observers can assume that the
    // set of encrypted types is Cryptographer::SensitiveTypes() and
    // that the encrypt everything flag is false.
    virtual void OnEncryptedTypesChanged(
        ModelTypeSet encrypted_types,
        bool encrypt_everything) = 0;

    // Called after we finish encrypting the current set of encrypted
    // types.
    virtual void OnEncryptionComplete() = 0;

    // The cryptographer has been updated. Listeners should check that their
    // own state matches the cryptographer.
    // Used primarily for debugging.
    virtual void OnCryptographerStateChanged(Cryptographer* cryptographer) = 0;

    // The passphrase type has changed. |type| is the new type,
    // |passphrase_time| is the time the passphrase was set (unset if |type|
    // is KEYSTORE_PASSPHRASE or the passphrase was set before we started
    // recording the time).
    virtual void OnPassphraseTypeChanged(PassphraseType type,
                                         base::Time passphrase_time) = 0;

    // The user has set a passphrase using this device.
    //
    // |nigori_state| can be used to restore nigori state across
    // SyncEncryptionHandlerImpl lifetimes. See also SyncEncryptionHandlerImpl's
    // RestoredNigori method.
    virtual void OnLocalSetPassphraseEncryption(
        const NigoriState& nigori_state) = 0;

   protected:
    virtual ~Observer();
  };

  class SYNC_EXPORT NigoriState {
   public:
    NigoriState() {}
    sync_pb::NigoriSpecifics nigori_specifics;
  };

  SyncEncryptionHandler();
  virtual ~SyncEncryptionHandler();

  // Add/Remove SyncEncryptionHandler::Observers.
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  // Reads the nigori node, updates internal state as needed, and, if an
  // empty/stale nigori node is detected, overwrites the existing
  // nigori node. Upon completion, if the cryptographer is still ready
  // attempts to re-encrypt all sync data.
  // Note: This method is expensive (it iterates through all encrypted types),
  // so should only be used sparingly (e.g. on startup).
  virtual void Init() = 0;

  // Attempts to re-encrypt encrypted data types using the passphrase provided.
  // Notifies observers of the result of the operation via OnPassphraseAccepted
  // or OnPassphraseRequired, updates the nigori node, and does re-encryption as
  // appropriate. If an explicit password has been set previously, we drop
  // subsequent requests to set a passphrase. If the cryptographer has pending
  // keys, and a new implicit passphrase is provided, we try decrypting the
  // pending keys with it, and if that fails, we cache the passphrase for
  // re-encryption once the pending keys are decrypted.
  virtual void SetEncryptionPassphrase(const std::string& passphrase,
                                       bool is_explicit) = 0;

  // Provides a passphrase for decrypting the user's existing sync data.
  // Notifies observers of the result of the operation via OnPassphraseAccepted
  // or OnPassphraseRequired, updates the nigori node, and does re-encryption as
  // appropriate if there is a previously cached encryption passphrase. It is an
  // error to call this when we don't have pending keys.
  virtual void SetDecryptionPassphrase(const std::string& passphrase) = 0;

  // Enables encryption of all datatypes.
  virtual void EnableEncryptEverything() = 0;

  // Whether encryption of all datatypes is enabled. If false, only sensitive
  // types are encrypted.
  virtual bool IsEncryptEverythingEnabled() const = 0;

  // Returns the current state of the passphrase needed to decrypt the
  // bag of encryption keys in the nigori node.
  virtual PassphraseType GetPassphraseType() const = 0;

  // The set of types that are always encrypted.
  static ModelTypeSet SensitiveTypes();
};

}  // namespace syncer

#endif  // SYNC_INTERNAL_API_PUBLIC_SYNC_ENCRYPTION_HANDLER_H_
