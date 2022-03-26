#include <linux/device.h>
#include <linux/input.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hid.h>

//#include "hid-ids.h"

MODULE_AUTHOR("Raleigh Littles <raleighlittles@gmail.com>");
MODULE_DESCRIPTION("Olympus MAJ 1428 driver");
MODULE_LICENSE("GPL");

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
#define USB_DEVICE_ID_OLYMPUS_MAJ1428_SPECIAL_KEYS 0x009b
#define USB_VENDOR_ID_SUN 0x0430 // this is normally available in hid-ids.h, I just added it here for local debugging


/**
 * @brief Receives and interprets raw HID data from the keyboard, and then generates the appropriate key press event.
 * 
 * @param data - A pointer to the start of an 8-byte array. This 8 byte array contains the HID data. This matches what you'll
 *               see in the URB_INTERRUPT packets from the keyboard on wireshark.
 */
static int olympus_maj1428_raw_event(struct hid_device* hdev, struct hid_report*, u8* data, int size) {

	// The 3rd byte in the array contains the actual keyboard scan code, this is what we need to identify which key was pressed.
	// Look up the HID packet interface for details
	u8 key_scancode = data[2];

    dev_printk(KERN_INFO, &hdev->dev, "key event detected! scan code = %d", data[2]);

	// You'll need to get a pointer to the actual input_dev object to be able to produce a key event from it
	struct input_dev* input;
	struct hid_input* hidinput;
	hidinput = list_entry(hdev->inputs.next, struct hid_input, list);
	input = hidinput->input;

	switch (key_scancode) {
		// See the keymap in the README for more information
		case 166: // 0xA6
			input_report_key(input, KEY_F13, 1);
			input_report_key(input, KEY_F13, 0);
			break;

		case 164:
			input_report_key(input, KEY_F14, 1);
			input_report_key(input, KEY_F14, 0);
			break;
	}

	return 0;
}

static void olympus_maj1428_remove(struct hid_device* hdev) {
    hid_hw_stop(hdev);
	dev_printk(KERN_INFO, &hdev->dev, "Olympus MAJ-1428 removed");
}

/// Device strings
static const struct hid_device_id olympus_maj1428_id_table[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_SUN,
			 USB_DEVICE_ID_OLYMPUS_MAJ1428_SPECIAL_KEYS) },
	{}
};
MODULE_DEVICE_TABLE(hid, olympus_maj1428_id_table);

static struct hid_driver olympus_maj1428_driver = {
	.name			= "Olympus MAJ-1428",
	.id_table		= olympus_maj1428_id_table,
/// This component is only for tweaking the report descriptor on events. We don't need to do this and can just use the scancodes
/// from the device directly
//  .report_fixup		= nullptr,
/// This callback runs after the kernel has positively identified our device. You can use this to check if the device is working
/// by sending out a sample command, etc, but we don't need it here.
//	.probe			= cougar_probe,
/// This callback runs after the device has been removed
	.remove			= olympus_maj1428_remove,
/// This callback is the event handler for a keyboard event. 
	.raw_event		= olympus_maj1428_raw_event,
};

module_hid_driver(olympus_maj1428_driver);