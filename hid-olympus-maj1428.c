#include <linux/device.h>
#include <linux/input.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hid.h>

//#include "hid-ids.h"

MODULE_AUTHOR("Raleigh Littles <raleighlittles>");
MODULE_DESCRIPTION("Olympus MAJ 1428 driver");
MODULE_LICENSE("MIT");

/* # Description

This keyboard actually has 3 different USB devices included in it. 
1. On the top right, there's a joystick-like controller (treated as a mouse), and a button [this is one device]
2. Starting at the top left, there are 9 keys:
- '#PER PAGE'
- 'CAPTURE
- 'DEL IMAGE'
- 'PRINT'
- 'PRINT QTY.'
- 'COLOR'
(This is referred to as the 'special keys')
3. Lastly, the main keyboard area (standard 87 keys) are treated as their own USB device

Each of the 3 devices has a different USB product ID.
*/
#define USB_DEVICE_ID_OLYMPUS_MAJ1428_SPECIAL_KEYS 009b
#define USB_DEVICE_ID_OLYMPUS 0430 // this is normally available in hid-ids.h, I just added it here for local debugging


static int olympus_maj_raw_event(struct hid_device* hdev, struct hid_report*, u8* data, int size) {
    dev_printk(KERN_ERR "raleigh");
}

static void olympus_maj_remove(struct hid_device* hdev) {
    hid_hw_stop(hdev);
}

/// Device strings
static const struct hid_device_id olympus_maj1428_id_table[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_SUN,
			 USB_DEVICE_ID_OLYMPUS_MAJ1428_SPECIAL_KEYS) },
	{}
};
MODULE_DEVICE_TABLE(hid, olympus_maj1428_id_table);

static struct hid_driver olympus_maj1428_driver = {
	.name			= "MAJ1428",
	.id_table		= olympus_maj1428_id_table,
/// This component is only for tweaking the report descriptor on events. We don't need to do this and can just use the scancodes
/// from the device directly
//  .report_fixup		= nullptr,
/// This callback runs after the kernel has positively identified our device. You can use this to check if the device is working
/// by sending out a sample command, etc, but we don't need it here.
//	.probe			= cougar_probe,
/// This callback runs after the device has been removed
	.remove			= olympus_maj_remove,
/// This callback is the event handler for a keyboard event. 
	.raw_event		= olympus_maj_raw_event,
};

module_hid_driver(olympus_maj1428_driver);