/* Generated by wayland-scanner 1.11.0 */

#ifndef REMOTE_SHELL_UNSTABLE_V1_SERVER_PROTOCOL_H
#define REMOTE_SHELL_UNSTABLE_V1_SERVER_PROTOCOL_H

#include <stdint.h>
#include <stddef.h>
#include "wayland-server.h"

#ifdef  __cplusplus
extern "C" {
#endif

struct wl_client;
struct wl_resource;

/**
 * @page page_remote_shell_unstable_v1 The remote_shell_unstable_v1 protocol
 * Create remote desktop-style surfaces
 *
 * @section page_desc_remote_shell_unstable_v1 Description
 *
 * remote_shell allows clients to turn a wl_surface into a "real window"
 * which can be stacked and activated by the user.
 *
 * Warning! The protocol described in this file is experimental and backward
 * incompatible changes may be made. Backward compatible changes may be added
 * together with the corresponding interface version bump. Backward
 * incompatible changes are done by bumping the version number in the protocol
 * and interface names and resetting the interface version. Once the protocol
 * is to be declared stable, the 'z' prefix and the version number in the
 * protocol and interface names are removed and the interface version number is
 * reset.
 *
 * @section page_ifaces_remote_shell_unstable_v1 Interfaces
 * - @subpage page_iface_zwp_remote_shell_v1 - remote_shell
 * - @subpage page_iface_zwp_remote_surface_v1 - A desktop window
 * - @subpage page_iface_zwp_notification_surface_v1 - A notification window
 * @section page_copyright_remote_shell_unstable_v1 Copyright
 * <pre>
 *
 * Copyright 2016 The Chromium Authors.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * </pre>
 */
struct wl_surface;
struct zwp_notification_surface_v1;
struct zwp_remote_shell_v1;
struct zwp_remote_surface_v1;

/**
 * @page page_iface_zwp_remote_shell_v1 zwp_remote_shell_v1
 * @section page_iface_zwp_remote_shell_v1_desc Description
 *
 * The global interface that allows clients to turn a wl_surface into a
 * "real window" which is remotely managed but can be stacked, activated
 * and made fullscreen by the user.
 * @section page_iface_zwp_remote_shell_v1_api API
 * See @ref iface_zwp_remote_shell_v1.
 */
/**
 * @defgroup iface_zwp_remote_shell_v1 The zwp_remote_shell_v1 interface
 *
 * The global interface that allows clients to turn a wl_surface into a
 * "real window" which is remotely managed but can be stacked, activated
 * and made fullscreen by the user.
 */
extern const struct wl_interface zwp_remote_shell_v1_interface;
/**
 * @page page_iface_zwp_remote_surface_v1 zwp_remote_surface_v1
 * @section page_iface_zwp_remote_surface_v1_desc Description
 *
 * An interface that may be implemented by a wl_surface, for
 * implementations that provide a desktop-style user interface
 * and allows for remotely managed windows.
 *
 * It provides requests to treat surfaces like windows, allowing to set
 * properties like app id and geometry.
 *
 * The client must call wl_surface.commit on the corresponding wl_surface
 * for the remote_surface state to take effect.
 *
 * For a surface to be mapped by the compositor the client must have
 * committed both an remote_surface state and a buffer.
 * @section page_iface_zwp_remote_surface_v1_api API
 * See @ref iface_zwp_remote_surface_v1.
 */
/**
 * @defgroup iface_zwp_remote_surface_v1 The zwp_remote_surface_v1 interface
 *
 * An interface that may be implemented by a wl_surface, for
 * implementations that provide a desktop-style user interface
 * and allows for remotely managed windows.
 *
 * It provides requests to treat surfaces like windows, allowing to set
 * properties like app id and geometry.
 *
 * The client must call wl_surface.commit on the corresponding wl_surface
 * for the remote_surface state to take effect.
 *
 * For a surface to be mapped by the compositor the client must have
 * committed both an remote_surface state and a buffer.
 */
extern const struct wl_interface zwp_remote_surface_v1_interface;
/**
 * @page page_iface_zwp_notification_surface_v1 zwp_notification_surface_v1
 * @section page_iface_zwp_notification_surface_v1_desc Description
 *
 * An interface that may be implemented by a wl_surface to host
 * notification contents.
 * @section page_iface_zwp_notification_surface_v1_api API
 * See @ref iface_zwp_notification_surface_v1.
 */
/**
 * @defgroup iface_zwp_notification_surface_v1 The zwp_notification_surface_v1 interface
 *
 * An interface that may be implemented by a wl_surface to host
 * notification contents.
 */
extern const struct wl_interface zwp_notification_surface_v1_interface;

#ifndef ZWP_REMOTE_SHELL_V1_CONTAINER_ENUM
#define ZWP_REMOTE_SHELL_V1_CONTAINER_ENUM
/**
 * @ingroup iface_zwp_remote_shell_v1
 * containers for remote surfaces
 *
 * Determine how a remote surface should be stacked relative to other
 * shell surfaces.
 */
enum zwp_remote_shell_v1_container {
	/**
	 * default container
	 */
	ZWP_REMOTE_SHELL_V1_CONTAINER_DEFAULT = 1,
	/**
	 * system modal container
	 */
	ZWP_REMOTE_SHELL_V1_CONTAINER_OVERLAY = 2,
};
#endif /* ZWP_REMOTE_SHELL_V1_CONTAINER_ENUM */

#ifndef ZWP_REMOTE_SHELL_V1_STATE_TYPE_ENUM
#define ZWP_REMOTE_SHELL_V1_STATE_TYPE_ENUM
/**
 * @ingroup iface_zwp_remote_shell_v1
 * state types for remote surfaces
 *
 * Defines common show states for shell surfaces.
 */
enum zwp_remote_shell_v1_state_type {
	/**
	 * normal window state
	 */
	ZWP_REMOTE_SHELL_V1_STATE_TYPE_NORMAL = 1,
	/**
	 * minimized window state
	 */
	ZWP_REMOTE_SHELL_V1_STATE_TYPE_MINIMIZED = 2,
	/**
	 * maximized window state
	 */
	ZWP_REMOTE_SHELL_V1_STATE_TYPE_MAXIMIZED = 3,
	/**
	 * fullscreen window state
	 */
	ZWP_REMOTE_SHELL_V1_STATE_TYPE_FULLSCREEN = 4,
	/**
	 * pinned window state
	 */
	ZWP_REMOTE_SHELL_V1_STATE_TYPE_PINNED = 5,
};
#endif /* ZWP_REMOTE_SHELL_V1_STATE_TYPE_ENUM */

#ifndef ZWP_REMOTE_SHELL_V1_ERROR_ENUM
#define ZWP_REMOTE_SHELL_V1_ERROR_ENUM
enum zwp_remote_shell_v1_error {
	/**
	 * given wl_surface has another role
	 */
	ZWP_REMOTE_SHELL_V1_ERROR_ROLE = 0,
	/**
	 * invalid notification id
	 */
	ZWP_REMOTE_SHELL_V1_ERROR_INVALID_NOTIFICATION_ID = 1,
};
#endif /* ZWP_REMOTE_SHELL_V1_ERROR_ENUM */

#ifndef ZWP_REMOTE_SHELL_V1_LAYOUT_MODE_ENUM
#define ZWP_REMOTE_SHELL_V1_LAYOUT_MODE_ENUM
/**
 * @ingroup iface_zwp_remote_shell_v1
 * the layout mode
 *
 * Determine how a client should layout surfaces.
 */
enum zwp_remote_shell_v1_layout_mode {
	/**
	 * multiple windows
	 */
	ZWP_REMOTE_SHELL_V1_LAYOUT_MODE_WINDOWED = 1,
	/**
	 * restricted mode for tablet
	 */
	ZWP_REMOTE_SHELL_V1_LAYOUT_MODE_TABLET = 2,
};
#endif /* ZWP_REMOTE_SHELL_V1_LAYOUT_MODE_ENUM */

/**
 * @ingroup iface_zwp_remote_shell_v1
 * @struct zwp_remote_shell_v1_interface
 */
struct zwp_remote_shell_v1_interface {
	/**
	 * destroy remote_shell
	 *
	 * Destroy this remote_shell object.
	 *
	 * Destroying a bound remote_shell object while there are surfaces
	 * still alive created by this remote_shell object instance is
	 * illegal and will result in a protocol error.
	 */
	void (*destroy)(struct wl_client *client,
			struct wl_resource *resource);
	/**
	 * create a remote shell surface from a surface
	 *
	 * This creates an remote_surface for the given surface and gives
	 * it the remote_surface role. A wl_surface can only be given a
	 * remote_surface role once. If get_remote_surface is called with a
	 * wl_surface that already has an active remote_surface associated
	 * with it, or if it had any other role, an error is raised.
	 *
	 * See the documentation of remote_surface for more details about
	 * what an remote_surface is and how it is used.
	 */
	void (*get_remote_surface)(struct wl_client *client,
				   struct wl_resource *resource,
				   uint32_t id,
				   struct wl_resource *surface,
				   uint32_t container);
	/**
	 * create a notification surface from a surface
	 *
	 * Creates a notification_surface for the given surface, gives it
	 * the notification_surface role and associated it with a
	 * notification id.
	 * @since 6
	 */
	void (*get_notification_surface)(struct wl_client *client,
					 struct wl_resource *resource,
					 uint32_t id,
					 struct wl_resource *surface,
					 const char *notification_id);
};

#define ZWP_REMOTE_SHELL_V1_CONFIGURE	0
#define ZWP_REMOTE_SHELL_V1_ACTIVATED	1
#define ZWP_REMOTE_SHELL_V1_LAYOUT_MODE_CHANGED	2
#define ZWP_REMOTE_SHELL_V1_CONFIGURATION_CHANGED	3

/**
 * @ingroup iface_zwp_remote_shell_v1
 */
#define ZWP_REMOTE_SHELL_V1_CONFIGURE_SINCE_VERSION	1
/**
 * @ingroup iface_zwp_remote_shell_v1
 */
#define ZWP_REMOTE_SHELL_V1_ACTIVATED_SINCE_VERSION	1
/**
 * @ingroup iface_zwp_remote_shell_v1
 */
#define ZWP_REMOTE_SHELL_V1_LAYOUT_MODE_CHANGED_SINCE_VERSION	8
/**
 * @ingroup iface_zwp_remote_shell_v1
 */
#define ZWP_REMOTE_SHELL_V1_CONFIGURATION_CHANGED_SINCE_VERSION	9

/**
 * @ingroup iface_zwp_remote_shell_v1
 * Sends an configure event to the client owning the resource.
 * @param resource_ The client's resource
 */
static inline void
zwp_remote_shell_v1_send_configure(struct wl_resource *resource_, int32_t width, int32_t height, int32_t work_area_inset_left, int32_t work_area_inset_top, int32_t work_area_inset_right, int32_t work_area_inset_bottom)
{
	wl_resource_post_event(resource_, ZWP_REMOTE_SHELL_V1_CONFIGURE, width, height, work_area_inset_left, work_area_inset_top, work_area_inset_right, work_area_inset_bottom);
}

/**
 * @ingroup iface_zwp_remote_shell_v1
 * Sends an activated event to the client owning the resource.
 * @param resource_ The client's resource
 */
static inline void
zwp_remote_shell_v1_send_activated(struct wl_resource *resource_, struct wl_resource *gained_active, struct wl_resource *lost_active)
{
	wl_resource_post_event(resource_, ZWP_REMOTE_SHELL_V1_ACTIVATED, gained_active, lost_active);
}

/**
 * @ingroup iface_zwp_remote_shell_v1
 * Sends an layout_mode_changed event to the client owning the resource.
 * @param resource_ The client's resource
 */
static inline void
zwp_remote_shell_v1_send_layout_mode_changed(struct wl_resource *resource_, uint32_t layout_mode)
{
	wl_resource_post_event(resource_, ZWP_REMOTE_SHELL_V1_LAYOUT_MODE_CHANGED, layout_mode);
}

/**
 * @ingroup iface_zwp_remote_shell_v1
 * Sends an configuration_changed event to the client owning the resource.
 * @param resource_ The client's resource
 */
static inline void
zwp_remote_shell_v1_send_configuration_changed(struct wl_resource *resource_, int32_t width, int32_t height, int32_t transform, wl_fixed_t scale_factor, int32_t work_area_inset_left, int32_t work_area_inset_top, int32_t work_area_inset_right, int32_t work_area_inset_bottom, uint32_t layout_mode)
{
	wl_resource_post_event(resource_, ZWP_REMOTE_SHELL_V1_CONFIGURATION_CHANGED, width, height, transform, scale_factor, work_area_inset_left, work_area_inset_top, work_area_inset_right, work_area_inset_bottom, layout_mode);
}

/**
 * @ingroup iface_zwp_remote_surface_v1
 * @struct zwp_remote_surface_v1_interface
 */
struct zwp_remote_surface_v1_interface {
	/**
	 * Destroy the remote_surface
	 *
	 * Unmap and destroy the window. The window will be effectively
	 * hidden from the user's point of view, and all state will be
	 * lost.
	 */
	void (*destroy)(struct wl_client *client,
			struct wl_resource *resource);
	/**
	 * set application ID
	 *
	 * Set an application identifier for the surface.
	 */
	void (*set_app_id)(struct wl_client *client,
			   struct wl_resource *resource,
			   const char *app_id);
	/**
	 * set the new window geometry
	 *
	 * The window geometry of a window is its "visible bounds" from
	 * the user's perspective. Client-side decorations often have
	 * invisible portions like drop-shadows which should be ignored for
	 * the purposes of aligning, placing and constraining windows.
	 *
	 * The window geometry is double buffered, and will be applied at
	 * the time wl_surface.commit of the corresponding wl_surface is
	 * called.
	 *
	 * Once the window geometry of the surface is set once, it is not
	 * possible to unset it, and it will remain the same until
	 * set_window_geometry is called again, even if a new subsurface or
	 * buffer is attached.
	 *
	 * If never set, the value is the full bounds of the output. This
	 * updates dynamically on every commit.
	 *
	 * The arguments are given in the output coordinate space.
	 *
	 * The width and height must be greater than zero.
	 */
	void (*set_window_geometry)(struct wl_client *client,
				    struct wl_resource *resource,
				    int32_t x,
				    int32_t y,
				    int32_t width,
				    int32_t height);
	/**
	 * set scale
	 *
	 * Set a scale factor that will be applied to surface and all
	 * descendants.
	 */
	void (*set_scale)(struct wl_client *client,
			  struct wl_resource *resource,
			  wl_fixed_t scale);
	/**
	 * fullscreen
	 *
	 * Request that surface is made fullscreen.
	 *
	 * This is only a request that the window should be made
	 * fullscreen. The compositor may choose to ignore this request.
	 * The client should listen to set_fullscreen events to determine
	 * if the window was made fullscreen or not.
	 * @since 2
	 */
	void (*fullscreen)(struct wl_client *client,
			   struct wl_resource *resource);
	/**
	 * maximize
	 *
	 * Request that surface is maximized. The window geometry will be
	 * updated to whatever the compositor finds appropriate for a
	 * maximized window.
	 *
	 * This is only a request that the window should be maximized. The
	 * compositor may choose to ignore this request. The client should
	 * listen to set_maximized events to determine if the window was
	 * maximized or not.
	 * @since 2
	 */
	void (*maximize)(struct wl_client *client,
			 struct wl_resource *resource);
	/**
	 * minimize
	 *
	 * Request that surface is minimized.
	 *
	 * This is only a request that the window should be minimized. The
	 * compositor may choose to ignore this request. The client should
	 * listen to set_minimized events to determine if the window was
	 * minimized or not.
	 * @since 2
	 */
	void (*minimize)(struct wl_client *client,
			 struct wl_resource *resource);
	/**
	 * restore
	 *
	 * Request that surface is restored. This restores the window
	 * geometry to what it was before the window was minimized,
	 * maximized or made fullscreen.
	 *
	 * This is only a request that the window should be restored. The
	 * compositor may choose to ignore this request. The client should
	 * listen to unset_maximized, unset_minimize and unset_fullscreen
	 * events to determine if the window was restored or not.
	 * @since 2
	 */
	void (*restore)(struct wl_client *client,
			struct wl_resource *resource);
	/**
	 * pin
	 *
	 * Request that surface is pinned.
	 *
	 * This is only a request that the window should be pinned. The
	 * compositor may choose to ignore this request. The client should
	 * listen to set_pinned events to determine if the window was
	 * pinned or not.
	 * @since 3
	 */
	void (*pin)(struct wl_client *client,
		    struct wl_resource *resource);
	/**
	 * unpin
	 *
	 * Request that surface is unpinned.
	 *
	 * This is only a request that the window should be unpinned. The
	 * compositor may choose to ignore this request. The client should
	 * listen to unset_pinned events to determine if the window was
	 * unpinned or not.
	 * @since 3
	 */
	void (*unpin)(struct wl_client *client,
		      struct wl_resource *resource);
	/**
	 * unfullscreen
	 *
	 * Request that surface is made unfullscreen.
	 *
	 * This is only a request that the window should be made
	 * unfullscreen. The compositor may choose to ignore this request.
	 * The client should listen to unset_fullscreen events to determine
	 * if the window was made unfullscreen or not.
	 * @since 3
	 */
	void (*unfullscreen)(struct wl_client *client,
			     struct wl_resource *resource);
	/**
	 * set a rectangular shadow
	 *
	 * Request that surface needs a rectangular shadow.
	 *
	 * This is only a request that the surface should have a
	 * rectangular shadow. The compositor may choose to ignore this
	 * request.
	 *
	 * The arguments are given in the output coordinate space and
	 * specifies the inner bounds of the shadow.
	 *
	 * The arguments are given in the output coordinate space.
	 * Specifying zero width and height will disable the shadow.
	 * @since 4
	 */
	void (*set_rectangular_shadow)(struct wl_client *client,
				       struct wl_resource *resource,
				       int32_t x,
				       int32_t y,
				       int32_t width,
				       int32_t height);
	/**
	 * set surface title
	 *
	 * Set a short title for the surface.
	 *
	 * This string may be used to identify the surface in a task bar,
	 * window list, or other user interface elements provided by the
	 * compositor.
	 *
	 * The string must be encoded in UTF-8.
	 * @since 5
	 */
	void (*set_title)(struct wl_client *client,
			  struct wl_resource *resource,
			  const char *title);
	/**
	 * set top inset for surface
	 *
	 * Set distance from the top of the surface to the contents.
	 *
	 * This distance typically represents the size of the window
	 * caption.
	 * @since 5
	 */
	void (*set_top_inset)(struct wl_client *client,
			      struct wl_resource *resource,
			      int32_t height);
	/**
	 * suggests a re-layout of remote shell input area
	 *
	 * Suggests a surface should become system modal.
	 * @since 8
	 */
	void (*set_system_modal)(struct wl_client *client,
				 struct wl_resource *resource);
	/**
	 * suggests a re-layout of remote shell input area
	 *
	 * Suggests a surface should become non system modal.
	 * @since 8
	 */
	void (*unset_system_modal)(struct wl_client *client,
				   struct wl_resource *resource);
	/**
	 * suggests the window's background opacity
	 *
	 * Suggests the window's background opacity when the shadow is
	 * requested.
	 * @since 9
	 */
	void (*set_rectangular_shadow_background_opacity)(struct wl_client *client,
							  struct wl_resource *resource,
							  wl_fixed_t opacity);
	/**
	 * activate surface
	 *
	 * Make surface active.
	 *
	 * @since 10
	 */
	void (*activate)(struct wl_client *client,
				struct wl_resource *resource,
				uint32_t serial);
};

#define ZWP_REMOTE_SURFACE_V1_SET_FULLSCREEN	0
#define ZWP_REMOTE_SURFACE_V1_UNSET_FULLSCREEN	1
#define ZWP_REMOTE_SURFACE_V1_CLOSE	2
#define ZWP_REMOTE_SURFACE_V1_SET_MAXIMIZED	3
#define ZWP_REMOTE_SURFACE_V1_UNSET_MAXIMIZED	4
#define ZWP_REMOTE_SURFACE_V1_SET_MINIMIZED	5
#define ZWP_REMOTE_SURFACE_V1_UNSET_MINIMIZED	6
#define ZWP_REMOTE_SURFACE_V1_SET_PINNED	7
#define ZWP_REMOTE_SURFACE_V1_UNSET_PINNED	8
#define ZWP_REMOTE_SURFACE_V1_STATE_TYPE_CHANGED	9

/**
 * @ingroup iface_zwp_remote_surface_v1
 */
#define ZWP_REMOTE_SURFACE_V1_SET_FULLSCREEN_SINCE_VERSION	1
/**
 * @ingroup iface_zwp_remote_surface_v1
 */
#define ZWP_REMOTE_SURFACE_V1_UNSET_FULLSCREEN_SINCE_VERSION	1
/**
 * @ingroup iface_zwp_remote_surface_v1
 */
#define ZWP_REMOTE_SURFACE_V1_CLOSE_SINCE_VERSION	1
/**
 * @ingroup iface_zwp_remote_surface_v1
 */
#define ZWP_REMOTE_SURFACE_V1_SET_MAXIMIZED_SINCE_VERSION	2
/**
 * @ingroup iface_zwp_remote_surface_v1
 */
#define ZWP_REMOTE_SURFACE_V1_UNSET_MAXIMIZED_SINCE_VERSION	2
/**
 * @ingroup iface_zwp_remote_surface_v1
 */
#define ZWP_REMOTE_SURFACE_V1_SET_MINIMIZED_SINCE_VERSION	2
/**
 * @ingroup iface_zwp_remote_surface_v1
 */
#define ZWP_REMOTE_SURFACE_V1_UNSET_MINIMIZED_SINCE_VERSION	2
/**
 * @ingroup iface_zwp_remote_surface_v1
 */
#define ZWP_REMOTE_SURFACE_V1_SET_PINNED_SINCE_VERSION	3
/**
 * @ingroup iface_zwp_remote_surface_v1
 */
#define ZWP_REMOTE_SURFACE_V1_UNSET_PINNED_SINCE_VERSION	3
/**
 * @ingroup iface_zwp_remote_surface_v1
 */
#define ZWP_REMOTE_SURFACE_V1_STATE_TYPE_CHANGED_SINCE_VERSION	7

/**
 * @ingroup iface_zwp_remote_surface_v1
 * Sends an set_fullscreen event to the client owning the resource.
 * @param resource_ The client's resource
 */
static inline void
zwp_remote_surface_v1_send_set_fullscreen(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, ZWP_REMOTE_SURFACE_V1_SET_FULLSCREEN);
}

/**
 * @ingroup iface_zwp_remote_surface_v1
 * Sends an unset_fullscreen event to the client owning the resource.
 * @param resource_ The client's resource
 */
static inline void
zwp_remote_surface_v1_send_unset_fullscreen(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, ZWP_REMOTE_SURFACE_V1_UNSET_FULLSCREEN);
}

/**
 * @ingroup iface_zwp_remote_surface_v1
 * Sends an close event to the client owning the resource.
 * @param resource_ The client's resource
 */
static inline void
zwp_remote_surface_v1_send_close(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, ZWP_REMOTE_SURFACE_V1_CLOSE);
}

/**
 * @ingroup iface_zwp_remote_surface_v1
 * Sends an set_maximized event to the client owning the resource.
 * @param resource_ The client's resource
 */
static inline void
zwp_remote_surface_v1_send_set_maximized(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, ZWP_REMOTE_SURFACE_V1_SET_MAXIMIZED);
}

/**
 * @ingroup iface_zwp_remote_surface_v1
 * Sends an unset_maximized event to the client owning the resource.
 * @param resource_ The client's resource
 */
static inline void
zwp_remote_surface_v1_send_unset_maximized(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, ZWP_REMOTE_SURFACE_V1_UNSET_MAXIMIZED);
}

/**
 * @ingroup iface_zwp_remote_surface_v1
 * Sends an set_minimized event to the client owning the resource.
 * @param resource_ The client's resource
 */
static inline void
zwp_remote_surface_v1_send_set_minimized(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, ZWP_REMOTE_SURFACE_V1_SET_MINIMIZED);
}

/**
 * @ingroup iface_zwp_remote_surface_v1
 * Sends an unset_minimized event to the client owning the resource.
 * @param resource_ The client's resource
 */
static inline void
zwp_remote_surface_v1_send_unset_minimized(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, ZWP_REMOTE_SURFACE_V1_UNSET_MINIMIZED);
}

/**
 * @ingroup iface_zwp_remote_surface_v1
 * Sends an set_pinned event to the client owning the resource.
 * @param resource_ The client's resource
 */
static inline void
zwp_remote_surface_v1_send_set_pinned(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, ZWP_REMOTE_SURFACE_V1_SET_PINNED);
}

/**
 * @ingroup iface_zwp_remote_surface_v1
 * Sends an unset_pinned event to the client owning the resource.
 * @param resource_ The client's resource
 */
static inline void
zwp_remote_surface_v1_send_unset_pinned(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, ZWP_REMOTE_SURFACE_V1_UNSET_PINNED);
}

/**
 * @ingroup iface_zwp_remote_surface_v1
 * Sends an state_type_changed event to the client owning the resource.
 * @param resource_ The client's resource
 */
static inline void
zwp_remote_surface_v1_send_state_type_changed(struct wl_resource *resource_, uint32_t state_type)
{
	wl_resource_post_event(resource_, ZWP_REMOTE_SURFACE_V1_STATE_TYPE_CHANGED, state_type);
}

/**
 * @ingroup iface_zwp_notification_surface_v1
 * @struct zwp_notification_surface_v1_interface
 */
struct zwp_notification_surface_v1_interface {
	/**
	 * Destroy the notification_surface
	 *
	 * Unmap and destroy the notification surface.
	 */
	void (*destroy)(struct wl_client *client,
			struct wl_resource *resource);
};


#ifdef  __cplusplus
}
#endif

#endif
