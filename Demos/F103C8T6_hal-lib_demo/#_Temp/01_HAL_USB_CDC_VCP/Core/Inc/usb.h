#ifndef __USB_H
#define __USB_H

#include "usb_device.h"
#include "usbd_cdc_if.h"

void USB_Device_DeInit(void);
void USB_Main_Loop(void);

#endif /* __USB_H */