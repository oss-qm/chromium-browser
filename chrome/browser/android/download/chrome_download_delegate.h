// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_DOWNLOAD_CHROME_DOWNLOAD_DELEGATE_H_
#define CHROME_BROWSER_ANDROID_DOWNLOAD_CHROME_DOWNLOAD_DELEGATE_H_

#include "base/android/jni_android.h"
#include "content/public/browser/web_contents_user_data.h"

class ChromeDownloadDelegate
    : public content::WebContentsUserData<ChromeDownloadDelegate> {
 public:
  // Returns true iff this request resulted in the tab creating the download
  // to close.
  static bool EnqueueDownloadManagerRequest(jobject chrome_download_delegate,
                                            bool overwrite,
                                            jobject download_info);

  void SetJavaRef(JNIEnv*, jobject obj);

  void RequestHTTPGetDownload(const std::string& url,
                              const std::string& user_agent,
                              const std::string& content_disposition,
                              const std::string& mime_type,
                              const std::string& cookie,
                              const std::string& referer,
                              const base::string16& file_name,
                              int64_t content_length,
                              bool has_user_geature,
                              bool must_download);
  void OnDownloadStarted(const std::string& filename,
                         const std::string& mime_type);
  void OnDangerousDownload(const std::string& filename,
                           const std::string& guid);
  void RequestFileAccess(intptr_t callback_id);

 private:
  explicit ChromeDownloadDelegate(content::WebContents* contents);
  friend class content::WebContentsUserData<ChromeDownloadDelegate>;
  ~ChromeDownloadDelegate() override;

  // A reference to the Java ChromeDownloadDelegate object.
  jobject java_ref_;
};

bool RegisterChromeDownloadDelegate(JNIEnv* env);

#endif  // CHROME_BROWSER_ANDROID_DOWNLOAD_CHROME_DOWNLOAD_DELEGATE_H_
