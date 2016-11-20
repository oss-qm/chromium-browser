// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chromoting.base;

import android.app.Activity;
import android.content.Context;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Looper;

import com.google.android.gms.auth.GoogleAuthException;
import com.google.android.gms.auth.GoogleAuthUtil;
import com.google.android.gms.auth.UserRecoverableAuthException;

import java.io.IOException;

/**
 * This helper class fetches an OAuth token on a separate thread, and properly handles the various
 * error-conditions that can occur (such as, starting an Activity to prompt user for input).
 */
public class OAuthTokenFetcher {
    /**
     * Callback interface to receive the token, or an error notification. These will be called
     * on the application's main thread. Note that if a user-recoverable error occurs and the
     * passed-in context is an activity, neither of these callback will be triggered. Instead, a new
     * Activity will be launched, and the calling Activity must override
     * {@link android.app.Activity#onActivityResult} and handle the result code
     * {@link REQUEST_CODE_RECOVER_FROM_OAUTH_ERROR} to re-attempt or cancel fetching the token.
     * If the context is not an activity, onError will be called with Error.UI.
     */
    public interface Callback {
        /** Called when a token is obtained. */
        void onTokenFetched(String token);

        /**
         * Called if an unrecoverable error prevents fetching a token.
         */
        void onError(Error error);
    }

    /** Error types that can be returned from the token-fetcher. */
    public enum Error {
        NETWORK,
        UI,  // When a user-recoverable exception occurs and |mContext| is not an activity.

        // When a user-recoverable exception occurs and a new activity is launched prompting user
        // for input.
        INTERRUPTED,
        UNEXPECTED
    }

    /** Request code used for starting the OAuth recovery activity. */
    public static final int REQUEST_CODE_RECOVER_FROM_OAUTH_ERROR = 100;

    /**
     * Reference to the context to fetch token. It will be used for starting other activities to
     * handle user-recoverable errors if it is an activity, otherwise user-recoverable errors will
     * not be handled.
     */
    private Context mContext;

    /** Account name (e-mail) for which the token will be fetched. */
    private String mAccountName;

    /** OAuth scope used for the token request. */
    private String mTokenScope;

    private Callback mCallback;

    public OAuthTokenFetcher(Context context, String accountName, String tokenScope,
            Callback callback) {
        mContext = context;
        mAccountName = accountName;
        mTokenScope = tokenScope;
        mCallback = callback;
    }

    /** Begins fetching a token. Should be called on the main thread. */
    public void fetch() {
        fetchImpl(null);
    }

    /**
     * Begins fetching a token, clearing an existing token from the cache. Should be called on the
     * main thread.
     * @param expiredToken A previously-fetched token which has expired.
     */
    public void clearAndFetch(String expiredToken) {
        fetchImpl(expiredToken);
    }

    private void fetchImpl(final String expiredToken) {
        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {
                try {
                    if (expiredToken != null) {
                        GoogleAuthUtil.clearToken(mContext, expiredToken);
                    }

                    // This method is deprecated but its replacement is not yet available.
                    // TODO(lambroslambrou): Fix this by replacing |mAccountName| with an instance
                    // of android.accounts.Account.
                    String token = GoogleAuthUtil.getToken(mContext, mAccountName, mTokenScope);
                    handleTokenReceived(token);
                } catch (IOException ioException) {
                    handleError(Error.NETWORK);
                } catch (UserRecoverableAuthException recoverableException) {
                    handleRecoverableException(recoverableException);
                } catch (GoogleAuthException fatalException) {
                    handleError(Error.UNEXPECTED);
                }
                return null;
            }
        }.execute();
    }

    private void handleTokenReceived(final String token) {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                mCallback.onTokenFetched(token);
            }
        });
    }

    private void handleError(final Error error) {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                mCallback.onError(error);
            }
        });
    }

    private void handleRecoverableException(final UserRecoverableAuthException exception) {
        if (!(mContext instanceof Activity)) {
            handleError(Error.UI);
            return;
        }
        handleError(Error.INTERRUPTED);
        final Activity activity = (Activity) mContext;
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                activity.startActivityForResult(exception.getIntent(),
                        REQUEST_CODE_RECOVER_FROM_OAUTH_ERROR);
            }
        });
    }
}
