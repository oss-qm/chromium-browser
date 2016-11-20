// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_TEST_RUNNER_WEB_TEST_DELEGATE_H_
#define COMPONENTS_TEST_RUNNER_WEB_TEST_DELEGATE_H_

#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "third_party/WebKit/public/platform/WebString.h"
#include "third_party/WebKit/public/platform/WebTaskRunner.h"
#include "third_party/WebKit/public/platform/WebURL.h"
#include "third_party/WebKit/public/platform/WebVector.h"
#include "third_party/WebKit/public/platform/modules/screen_orientation/WebScreenOrientationType.h"

#define WEBTESTRUNNER_NEW_HISTORY_CAPTURE

namespace base {
class DictionaryValue;
}

namespace blink {
class WebDeviceMotionData;
class WebDeviceOrientationData;
class WebFrame;
class WebGamepad;
class WebGamepads;
class WebHistoryItem;
class WebLayer;
class WebLocalFrame;
class WebMediaStream;
class WebPlugin;
struct WebPluginParams;
struct WebPoint;
struct WebRect;
struct WebSize;
struct WebURLError;
class WebURLResponse;
class WebView;
}

namespace cc {
class TextureLayer;
class TextureLayerClient;
class SharedBitmapManager;
}

namespace test_runner {

class DeviceLightData;
class GamepadController;
class WebTask;
class WebTestProxyBase;
struct TestPreferences;

class WebTestDelegate {
 public:
  // Set and clear the edit command to execute on the next call to
  // WebViewClient::handleCurrentKeyboardEvent().
  virtual void ClearEditCommand() = 0;
  virtual void SetEditCommand(const std::string& name,
                              const std::string& value) = 0;

  // Sets gamepad provider to be used for tests.
  virtual void SetGamepadProvider(GamepadController* controller) = 0;

  // Set data to return when registering via
  // Platform::setDeviceLightListener().
  virtual void SetDeviceLightData(const double data) = 0;
  // Set data to return when registering via
  // Platform::setDeviceMotionListener().
  virtual void SetDeviceMotionData(const blink::WebDeviceMotionData& data) = 0;
  // Set data to return when registering via
  // Platform::setDeviceOrientationListener().
  virtual void SetDeviceOrientationData(
      const blink::WebDeviceOrientationData& data) = 0;

  // Add a message to the text dump for the layout test.
  virtual void PrintMessage(const std::string& message) = 0;

  // The delegate takes ownership of the WebTask objects and is responsible
  // for deleting them.
  virtual void PostTask(blink::WebTaskRunner::Task* task) = 0;
  virtual void PostDelayedTask(blink::WebTaskRunner::Task* task,
                               long long ms) = 0;

  // Register a new isolated filesystem with the given files, and return the
  // new filesystem id.
  virtual blink::WebString RegisterIsolatedFileSystem(
      const blink::WebVector<blink::WebString>& absolute_filenames) = 0;

  // Gets the current time in milliseconds since the UNIX epoch.
  virtual long long GetCurrentTimeInMillisecond() = 0;

  // Convert the provided relative path into an absolute path.
  virtual blink::WebString GetAbsoluteWebStringFromUTF8Path(
      const std::string& path) = 0;

  // Reads in the given file and returns its contents as data URL.
  virtual blink::WebURL LocalFileToDataURL(const blink::WebURL& file_url) = 0;

  // Replaces file:///tmp/LayoutTests/ with the actual path to the
  // LayoutTests directory, or rewrite URLs generated from absolute
  // path links in web-platform-tests.
  virtual blink::WebURL RewriteLayoutTestsURL(const std::string& utf8_url,
                                              bool is_wpt_mode) = 0;

  // Manages the settings to used for layout tests.
  virtual TestPreferences* Preferences() = 0;
  virtual void ApplyPreferences() = 0;

  // Enables or disables synchronous resize mode. When enabled, all
  // window-sizing machinery is
  // short-circuited inside the renderer. This mode is necessary for some tests
  // that were written
  // before browsers had multi-process architecture and rely on window resizes
  // to happen synchronously.
  // The function has "unfortunate" it its name because we must strive to remove
  // all tests
  // that rely on this... well, unfortunate behavior. See
  // http://crbug.com/309760 for the plan.
  virtual void UseUnfortunateSynchronousResizeMode(bool enable) = 0;

  // Controls auto resize mode.
  virtual void EnableAutoResizeMode(const blink::WebSize& min_size,
                                    const blink::WebSize& max_size) = 0;
  virtual void DisableAutoResizeMode(const blink::WebSize& new_size) = 0;

  // Clears DevTools' localStorage when an inspector test is started.
  virtual void ClearDevToolsLocalStorage() = 0;

  // Opens and closes the inspector.
  virtual void ShowDevTools(const std::string& settings,
                            const std::string& frontend_url) = 0;
  virtual void CloseDevTools() = 0;

  // Evaluate the given script in the DevTools agent.
  virtual void EvaluateInWebInspector(int call_id,
                                      const std::string& script) = 0;

  // Evaluate the given script in the inspector overlay page.
  virtual std::string EvaluateInWebInspectorOverlay(
      const std::string& script) = 0;

  // Controls WebSQL databases.
  virtual void ClearAllDatabases() = 0;
  virtual void SetDatabaseQuota(int quota) = 0;

  // Controls Web Notifications.
  virtual void SimulateWebNotificationClick(const std::string& title,
                                            int action_index) = 0;
  virtual void SimulateWebNotificationClose(const std::string& title,
                                            bool by_user) = 0;

  // Controls the device scale factor of the main WebView for hidpi tests.
  virtual void SetDeviceScaleFactor(float factor) = 0;

  // When use-zoom-for-dsf mode is enabled, this returns the scale to
  // convert from window coordinates to viewport coordinates. When
  // use-zoom-for-dsf is disabled, this return always 1.0f.
  virtual float GetWindowToViewportScale() = 0;

  // Enable zoom-for-dsf option.
  virtual void EnableUseZoomForDSF() = 0;

  // Returns whether or not the use-zoom-for-dsf flag is enabled.
  virtual bool IsUseZoomForDSFEnabled() = 0;

  // Change the device color profile while running a layout test.
  virtual void SetDeviceColorProfile(const std::string& name) = 0;

  // Set the bluetooth adapter while running a layout test, uses Mojo to
  // communicate with the browser.
  virtual void SetBluetoothFakeAdapter(const std::string& adapter_name,
                                       const base::Closure& callback) = 0;

  // If |enable| is true makes the Bluetooth chooser record its input and wait
  // for instructions from the test program on how to proceed. Otherwise
  // fall backs to the browser's default chooser.
  virtual void SetBluetoothManualChooser(bool enable) = 0;

  // Returns the events recorded since the last call to this function.
  virtual void GetBluetoothManualChooserEvents(
      const base::Callback<void(const std::vector<std::string>& events)>&
          callback) = 0;

  // Calls the BluetoothChooser::EventHandler with the arguments here. Valid
  // event strings are:
  //  * "cancel" - simulates the user canceling the chooser.
  //  * "select" - simulates the user selecting a device whose device ID is in
  //               |argument|.
  virtual void SendBluetoothManualChooserEvent(const std::string& event,
                                               const std::string& argument) = 0;

  // Controls which WebView should be focused.
  virtual void SetFocus(blink::WebView* web_view, bool focus) = 0;

  // Controls whether all cookies should be accepted or writing cookies in a
  // third-party context is blocked.
  virtual void SetAcceptAllCookies(bool accept) = 0;

  // The same as RewriteLayoutTestsURL unless the resource is a path starting
  // with /tmp/, then return a file URL to a temporary file.
  virtual std::string PathToLocalResource(const std::string& resource) = 0;

  // Sets the POSIX locale of the current process.
  virtual void SetLocale(const std::string& locale) = 0;

  // Invoked when layout test runtime flags change.
  virtual void OnLayoutTestRuntimeFlagsChanged(
      const base::DictionaryValue& changed_values) = 0;

  // Invoked when the test finished.
  virtual void TestFinished() = 0;

  // Invoked when the embedder should close all but the main WebView.
  virtual void CloseRemainingWindows() = 0;

  virtual void DeleteAllCookies() = 0;

  // Returns the length of the back/forward history of the main WebView.
  virtual int NavigationEntryCount() = 0;

  // The following trigger navigations on the main WebView.
  virtual void GoToOffset(int offset) = 0;
  virtual void Reload() = 0;
  virtual void LoadURLForFrame(const blink::WebURL& url,
                               const std::string& frame_name) = 0;

  // Returns true if resource requests to external URLs should be permitted.
  virtual bool AllowExternalPages() = 0;

  // Fetch the manifest for a given WebView from the given url.
  virtual void FetchManifest(
      blink::WebView* view,
      const GURL& url,
      const base::Callback<void(const blink::WebURLResponse& response,
                                const std::string& data)>& callback) = 0;

  // Sends a message to the LayoutTestPermissionManager in order for it to
  // update its database.
  virtual void SetPermission(const std::string& permission_name,
                             const std::string& permission_value,
                             const GURL& origin,
                             const GURL& embedding_origin) = 0;

  // Clear all the permissions set via SetPermission().
  virtual void ResetPermissions() = 0;

  // Add content MediaStream classes to the Blink MediaStream ones.
  virtual bool AddMediaStreamVideoSourceAndTrack(
      blink::WebMediaStream* stream) = 0;
  virtual bool AddMediaStreamAudioSourceAndTrack(
      blink::WebMediaStream* stream) = 0;

  virtual cc::SharedBitmapManager* GetSharedBitmapManager() = 0;

  // Causes the beforeinstallprompt event to be sent to the renderer with a
  // request id of |request_id|. |event_platforms| are the platforms to be sent
  // with the event. Once the event listener completes, |callback| will be
  // called with a boolean argument. This argument will be true if the event is
  // canceled, and false otherwise.
  virtual void DispatchBeforeInstallPromptEvent(
      int request_id,
      const std::vector<std::string>& event_platforms,
      const base::Callback<void(bool)>& callback) = 0;

  virtual blink::WebPlugin* CreatePluginPlaceholder(
    blink::WebLocalFrame* frame,
    const blink::WebPluginParams& params) = 0;

  virtual float GetDeviceScaleFactorForTest() const = 0;

  // Run all pending idle tasks, and then run callback.
  virtual void RunIdleTasks(const base::Closure& callback) = 0;
};

}  // namespace test_runner

#endif  // COMPONENTS_TEST_RUNNER_WEB_TEST_DELEGATE_H_
