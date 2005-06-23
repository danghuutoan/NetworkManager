/* NetworkManager -- Network link manager
 *
 * Dan Williams <dcbw@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * (C) Copyright 2004 Red Hat, Inc.
 */

#ifndef NETWORK_MANAGER_MAIN_H
#define NETWORK_MANAGER_MAIN_H

#include <glib.h>
#include <glib/gthread.h>
#include <dbus/dbus.h>
#include <libhal.h>
#include "NetworkManager.h"
#include "NetworkManagerAP.h"
#include "nm-netlink-monitor.h"
#include "nm-named-manager.h"


typedef enum NMIntState
{
	NM_INT_STATE_UNKNOWN = 0,
	NM_INT_STATE_ASLEEP,
	NM_INT_STATE_CONFIGURE_AP,
	NM_INT_STATE_CONFIGURE_DEV,
	NM_INT_STATE_CONFIGURE_IP,
	NM_INT_STATE_CONNECTED,
	NM_INT_STATE_DISCONNECTED
} NMIntState;


typedef struct NMDbusMethodList NMDbusMethodList;
typedef struct NMActRequest NMActRequest;
typedef struct NMVPNManager NMVPNManager;
typedef struct NMDHCPManager NMDHCPManager;

#define DHCP_SERVICE_NAME	"com.redhat.dhcp"
#define DHCP_OBJECT_PATH		"/com/redhat/dhcp"

typedef struct NMData
{
	GIOChannel *			sigterm_iochannel;
	int					sigterm_pipe[2];

	LibHalContext *		hal_ctx;

	NmNetlinkMonitor *		netlink_monitor;

	NMNamedManager *		named_manager;
	NMVPNManager *			vpn_manager;
	NMDHCPManager *		dhcp_manager;

	DBusConnection *		dbus_connection;
	NMDbusMethodList *		nm_methods;
	NMDbusMethodList *		device_methods;
	NMDbusMethodList *		net_methods;
	NMDbusMethodList *		vpn_methods;

	GMainContext *			main_context;
	GMainLoop *			main_loop;
	gboolean				enable_test_devices;

	guint				dev_change_check_idle_id;

	GSList *				dev_list;
	GMutex *				dev_list_mutex;

	NMWirelessScanMethod	scanning_method;
	gboolean				wireless_enabled;
	gboolean				asleep;

	struct NMAccessPointList	*allowed_ap_list;
	struct NMAccessPointList	*invalid_ap_list;
} NMData;


struct NMDevice *	nm_get_active_device					(NMData *data);

struct NMDevice *	nm_create_device_and_add_to_list			(NMData *data, const char *udi, const char *iface,
														gboolean test_device, NMDeviceType test_device_type);

void				nm_remove_device_from_list				(NMData *data, const char *udi);

void				nm_schedule_state_change_signal_broadcast	(NMData *data);

void				nm_hal_init							(NMData *data);

void				nm_hal_deinit							(NMData *data);

#endif
