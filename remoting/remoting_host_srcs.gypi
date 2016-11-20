# Copyright 2014 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'remoting_host_sources': [
      'host/audio_capturer.cc',
      'host/audio_capturer.h',
      'host/audio_capturer_android.cc',
      'host/audio_capturer_linux.cc',
      'host/audio_capturer_linux.h',
      'host/audio_capturer_mac.cc',
      'host/audio_capturer_win.cc',
      'host/audio_capturer_win.h',
      'host/audio_pump.cc',
      'host/audio_pump.h',
      'host/audio_silence_detector.cc',
      'host/audio_silence_detector.h',
      'host/backoff_timer.cc',
      'host/backoff_timer.h',
      'host/basic_desktop_environment.cc',
      'host/basic_desktop_environment.h',
      'host/branding.cc',
      'host/branding.h',
      'host/chromeos/aura_desktop_capturer.cc',
      'host/chromeos/aura_desktop_capturer.h',
      'host/chromeos/clipboard_aura.cc',
      'host/chromeos/clipboard_aura.h',
      'host/chromeos/message_box.cc',
      'host/chromeos/message_box.h',
      'host/chromeos/mouse_cursor_monitor_aura.cc',
      'host/chromeos/mouse_cursor_monitor_aura.h',
      'host/chromeos/point_transformer.cc',
      'host/chromeos/point_transformer.h',
      'host/chromeos/skia_bitmap_desktop_frame.cc',
      'host/chromeos/skia_bitmap_desktop_frame.h',
      'host/chromoting_host.cc',
      'host/chromoting_host.h',
      'host/chromoting_host_context.cc',
      'host/chromoting_host_context.h',
      'host/chromoting_messages.cc',
      'host/chromoting_messages.h',
      'host/chromoting_param_traits.cc',
      'host/chromoting_param_traits.h',
      'host/client_session.cc',
      'host/client_session.h',
      'host/client_session_control.h',
      'host/client_session_details.h',
      'host/clipboard.h',
      'host/clipboard_mac.mm',
      'host/clipboard_win.cc',
      'host/clipboard_x11.cc',
      'host/config_file_watcher.cc',
      'host/config_file_watcher.h',
      'host/config_watcher.h',
      'host/constants_mac.cc',
      'host/constants_mac.h',
      'host/continue_window.cc',
      'host/continue_window.h',
      'host/continue_window_android.cc',
      'host/continue_window_chromeos.cc',
      'host/continue_window_linux.cc',
      'host/continue_window_mac.mm',
      'host/continue_window_win.cc',
      'host/curtain_mode.h',
      'host/curtain_mode_android.cc',
      'host/curtain_mode_linux.cc',
      'host/curtain_mode_mac.cc',
      'host/curtain_mode_win.cc',
      'host/daemon_process.cc',
      'host/daemon_process.h',
      'host/daemon_process_win.cc',
      'host/desktop_capturer_proxy.cc',
      'host/desktop_capturer_proxy.h',
      'host/desktop_environment.h',
      'host/desktop_process.cc',
      'host/desktop_process.h',
      'host/desktop_resizer.h',
      'host/desktop_resizer_android.cc',
      'host/desktop_resizer_mac.cc',
      'host/desktop_resizer_ozone.cc',
      'host/desktop_resizer_win.cc',
      'host/desktop_resizer_x11.cc',
      'host/desktop_session.cc',
      'host/desktop_session.h',
      'host/desktop_session_agent.cc',
      'host/desktop_session_agent.h',
      'host/desktop_session_connector.h',
      'host/desktop_session_proxy.cc',
      'host/desktop_session_proxy.h',
      'host/desktop_session_win.cc',
      'host/desktop_session_win.h',
      'host/disconnect_window_android.cc',
      'host/disconnect_window_chromeos.cc',
      'host/disconnect_window_linux.cc',
      'host/disconnect_window_mac.h',
      'host/disconnect_window_mac.mm',
      'host/disconnect_window_win.cc',
      'host/dns_blackhole_checker.cc',
      'host/dns_blackhole_checker.h',
      'host/gcd_rest_client.cc',
      'host/gcd_rest_client.h',
      'host/gcd_state_updater.cc',
      'host/gcd_state_updater.h',
      'host/heartbeat_sender.cc',
      'host/heartbeat_sender.h',
      'host/host_change_notification_listener.cc',
      'host/host_change_notification_listener.h',
      'host/host_config.cc',
      'host/host_config.h',
      'host/host_config_constants.cc',
      'host/host_details.cc',
      'host/host_details.h',
      'host/host_event_logger.h',
      'host/host_event_logger_posix.cc',
      'host/host_event_logger_win.cc',
      'host/host_exit_codes.cc',
      'host/host_exit_codes.h',
      'host/host_export.h',
      'host/host_extension.h',
      'host/host_extension_session.h',
      'host/host_extension_session_manager.cc',
      'host/host_extension_session_manager.h',
      'host/host_secret.cc',
      'host/host_secret.h',
      'host/host_status_logger.cc',
      'host/host_status_logger.h',
      'host/host_status_monitor.h',
      'host/host_status_observer.h',
      'host/host_window.h',
      'host/host_window_proxy.cc',
      'host/host_window_proxy.h',
      'host/input_injector.h',
      'host/input_injector_android.cc',
      'host/input_injector_chromeos.cc',
      'host/input_injector_chromeos.h',
      'host/input_injector_mac.cc',
      'host/input_injector_win.cc',
      'host/input_injector_x11.cc',
      'host/ipc_audio_capturer.cc',
      'host/ipc_audio_capturer.h',
      'host/ipc_constants.cc',
      'host/ipc_constants.h',
      'host/ipc_desktop_environment.cc',
      'host/ipc_desktop_environment.h',
      'host/ipc_host_event_logger.cc',
      'host/ipc_host_event_logger.h',
      'host/ipc_input_injector.cc',
      'host/ipc_input_injector.h',
      'host/ipc_mouse_cursor_monitor.cc',
      'host/ipc_mouse_cursor_monitor.h',
      'host/ipc_screen_controls.cc',
      'host/ipc_screen_controls.h',
      'host/ipc_util.h',
      'host/ipc_util_posix.cc',
      'host/ipc_util_win.cc',
      'host/ipc_video_frame_capturer.cc',
      'host/ipc_video_frame_capturer.h',
      'host/it2me_desktop_environment.cc',
      'host/it2me_desktop_environment.h',
      'host/linux/audio_pipe_reader.cc',
      'host/linux/audio_pipe_reader.h',
      'host/linux/certificate_watcher.cc',
      'host/linux/certificate_watcher.h',
      'host/linux/unicode_to_keysym.cc',
      'host/linux/unicode_to_keysym.h',
      'host/linux/x11_util.cc',
      'host/linux/x11_util.h',
      'host/linux/x_server_clipboard.cc',
      'host/linux/x_server_clipboard.h',
      'host/local_input_monitor.h',
      'host/local_input_monitor_android.cc',
      'host/local_input_monitor_chromeos.cc',
      'host/local_input_monitor_mac.mm',
      'host/local_input_monitor_win.cc',
      'host/local_input_monitor_x11.cc',
      'host/logging.h',
      'host/logging_linux.cc',
      'host/logging_mac.cc',
      'host/logging_win.cc',
      'host/me2me_desktop_environment.cc',
      'host/me2me_desktop_environment.h',
      'host/mouse_cursor_monitor_proxy.cc',
      'host/mouse_cursor_monitor_proxy.h',
      'host/mouse_shape_pump.cc',
      'host/mouse_shape_pump.h',
      'host/oauth_token_getter.cc',
      'host/oauth_token_getter.h',
      'host/oauth_token_getter_impl.cc',
      'host/oauth_token_getter_impl.h',
      'host/pairing_registry_delegate.cc',
      'host/pairing_registry_delegate.h',
      'host/pairing_registry_delegate_android.cc',
      'host/pairing_registry_delegate_linux.cc',
      'host/pairing_registry_delegate_linux.h',
      'host/pairing_registry_delegate_mac.cc',
      'host/pairing_registry_delegate_win.cc',
      'host/pairing_registry_delegate_win.h',
      'host/pin_hash.cc',
      'host/pin_hash.h',
      'host/policy_watcher.cc',
      'host/policy_watcher.h',
      'host/posix/signal_handler.cc',
      'host/posix/signal_handler.h',
      'host/register_support_host_request.cc',
      'host/register_support_host_request.h',
      'host/remote_input_filter.cc',
      'host/remote_input_filter.h',
      'host/resizing_host_observer.cc',
      'host/resizing_host_observer.h',
      'host/resources.h',
      'host/resources_android.cc',
      'host/resources_linux.cc',
      'host/resources_mac.cc',
      'host/resources_win.cc',
      'host/sas_injector.h',
      'host/sas_injector_win.cc',
      'host/screen_controls.h',
      'host/screen_resolution.cc',
      'host/screen_resolution.h',
      'host/security_key/gnubby_auth_handler.h',
      'host/security_key/gnubby_auth_handler_android.cc',
      'host/security_key/gnubby_auth_handler_linux.cc',
      'host/security_key/gnubby_auth_handler_mac.cc',
      'host/security_key/gnubby_auth_handler_win.cc',
      'host/security_key/gnubby_extension.cc',
      'host/security_key/gnubby_extension.h',
      'host/security_key/gnubby_extension_session.cc',
      'host/security_key/gnubby_extension_session.h',
      'host/security_key/gnubby_socket.cc',
      'host/security_key/gnubby_socket.h',
      'host/security_key/remote_security_key_ipc_client.cc',
      'host/security_key/remote_security_key_ipc_client.h',
      'host/security_key/remote_security_key_ipc_constants.cc',
      'host/security_key/remote_security_key_ipc_constants.h',
      'host/security_key/remote_security_key_ipc_server.cc',
      'host/security_key/remote_security_key_ipc_server.h',
      'host/security_key/remote_security_key_ipc_server_impl.cc',
      'host/security_key/remote_security_key_ipc_server_impl.h',
      'host/security_key/remote_security_key_message_handler.cc',
      'host/security_key/remote_security_key_message_handler.h',
      'host/security_key/remote_security_key_message_reader.h',
      'host/security_key/remote_security_key_message_reader_impl.cc',
      'host/security_key/remote_security_key_message_reader_impl.h',
      'host/security_key/remote_security_key_message_writer.h',
      'host/security_key/remote_security_key_message_writer_impl.cc',
      'host/security_key/remote_security_key_message_writer_impl.h',
      'host/security_key/security_key_message.cc',
      'host/security_key/security_key_message.h',
      'host/server_log_entry_host.cc',
      'host/server_log_entry_host.h',
      'host/service_urls.cc',
      'host/service_urls.h',
      'host/shutdown_watchdog.cc',
      'host/shutdown_watchdog.h',
      'host/signaling_connector.cc',
      'host/signaling_connector.h',
      'host/single_window_desktop_environment.cc',
      'host/single_window_desktop_environment.h',
      'host/single_window_input_injector.h',
      'host/single_window_input_injector_linux.cc',
      'host/single_window_input_injector_mac.cc',
      'host/single_window_input_injector_win.cc',
      'host/switches.cc',
      'host/switches.h',
      'host/third_party_auth_config.cc',
      'host/third_party_auth_config.h',
      'host/token_validator_base.cc',
      'host/token_validator_base.h',
      'host/token_validator_factory_impl.cc',
      'host/token_validator_factory_impl.h',
      'host/touch_injector_win.cc',
      'host/touch_injector_win.h',
      'host/usage_stats_consent.h',
      'host/usage_stats_consent_mac.cc',
      'host/usage_stats_consent_win.cc',
      'host/username.cc',
      'host/username.h',
      'host/win/com_imported_mstscax.tlh',
      'host/win/com_security.cc',
      'host/win/com_security.h',
      'host/win/elevation_helpers.cc',
      'host/win/elevation_helpers.h',
      'host/win/launch_process_with_token.cc',
      'host/win/launch_process_with_token.h',
      'host/win/omaha.cc',
      'host/win/omaha.h',
      'host/win/rdp_client.cc',
      'host/win/rdp_client.h',
      'host/win/rdp_client_window.cc',
      'host/win/rdp_client_window.h',
      'host/win/security_descriptor.cc',
      'host/win/security_descriptor.h',
      'host/win/session_desktop_environment.cc',
      'host/win/session_desktop_environment.h',
      'host/win/session_input_injector.cc',
      'host/win/session_input_injector.h',
      'host/win/window_station_and_desktop.cc',
      'host/win/window_station_and_desktop.h',
      'host/win/worker_process_launcher.cc',
      'host/win/worker_process_launcher.h',
      'host/win/wts_terminal_monitor.cc',
      'host/win/wts_terminal_monitor.h',
      'host/win/wts_terminal_observer.h',
    ],
    'remoting_host_setup_sources': [
      'host/setup/daemon_controller.cc',
      'host/setup/daemon_controller.h',
      'host/setup/daemon_controller_delegate_linux.cc',
      'host/setup/daemon_controller_delegate_linux.h',
      'host/setup/daemon_controller_delegate_mac.h',
      'host/setup/daemon_controller_delegate_mac.mm',
      'host/setup/daemon_controller_delegate_win.cc',
      'host/setup/daemon_controller_delegate_win.h',
      'host/setup/gaia_oauth_client.cc',
      'host/setup/gaia_oauth_client.h',
      'host/setup/me2me_native_messaging_host.cc',
      'host/setup/me2me_native_messaging_host.h',
      'host/setup/oauth_client.h',
      'host/setup/oauth_helper.cc',
      'host/setup/oauth_helper.h',
      'host/setup/pin_validator.cc',
      'host/setup/pin_validator.h',
      'host/setup/service_client.cc',
      'host/setup/service_client.h',
      'host/setup/test_util.cc',
      'host/setup/test_util.h',
      'host/setup/win/auth_code_getter.cc',
      'host/setup/win/auth_code_getter.h',
    ],
    'remoting_host_native_messaging_sources': [
      'host/native_messaging/log_message_handler.cc',
      'host/native_messaging/log_message_handler.h',
      'host/native_messaging/native_messaging_pipe.cc',
      'host/native_messaging/native_messaging_pipe.h',
      'host/native_messaging/native_messaging_reader.cc',
      'host/native_messaging/native_messaging_reader.h',
      'host/native_messaging/native_messaging_writer.cc',
      'host/native_messaging/native_messaging_writer.h',
      'host/native_messaging/pipe_messaging_channel.cc',
      'host/native_messaging/pipe_messaging_channel.h',
    ],
    'remoting_it2me_host_static_sources' : [
      'host/it2me/it2me_confirmation_dialog.cc',
      'host/it2me/it2me_confirmation_dialog.h',
      'host/it2me/it2me_confirmation_dialog_chromeos.cc',
      'host/it2me/it2me_confirmation_dialog_proxy.cc',
      'host/it2me/it2me_confirmation_dialog_proxy.h',
      'host/it2me/it2me_host.cc',
      'host/it2me/it2me_host.h',
      'host/it2me/it2me_native_messaging_host.cc',
      'host/it2me/it2me_native_messaging_host.h',
    ],
  }
}
