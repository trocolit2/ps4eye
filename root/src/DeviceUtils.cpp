/*
 *
 *  Created on: Sep 14, 2015
 *      Author: tiagotrocoli
 */

#include <DeviceUtils.h>
#include <libudev.h>
#include <string>
#include <iostream>

std::vector<deviceAttributes> DeviceUtils::getDeviceAtributesFromSubsystem(std::string subsystem) {
    std::vector<deviceAttributesSearch> attributes = { SUBSYSTEM };
    std::vector<std::string> values = { subsystem };
    return getDevicesFrom(attributes, values);
}

std::vector<deviceAttributes> DeviceUtils::getDeviceAtributesFromIdVendorAndIdProductor(std::string idVendor, std::string idProduct) {
    std::vector<deviceAttributesSearch> attributes = { IDVENDOR, IDPRODUCT };
    std::vector<std::string> values = { idVendor, idProduct };
    return getDevicesFrom(attributes, values);
}

std::vector<deviceAttributes> DeviceUtils::getDeviceAtributesFromSysname(std::string sysname) {
    std::vector<deviceAttributesSearch> attributes = { SYSNAME };
    std::vector<std::string> values = { sysname };
    return getDevicesFrom(attributes, values);
}

std::vector<deviceAttributes> DeviceUtils::getDevicesFrom(std::vector<deviceAttributesSearch> attributes, std::vector<std::string> values) {

    //code based on http://www.signal11.us/oss/udev/
    struct udev *udev;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;
    struct udev_device *dev;

    udev = udev_new();
    if (!udev) {
        printf("Can't create udev\n");
        exit(1);
    }

    enumerate = udev_enumerate_new(udev);
    for (int i = 0; i < attributes.size(); ++i) {
        switch (attributes[i]) {
        case SUBSYSTEM:
            udev_enumerate_add_match_subsystem(enumerate, values[i].c_str());
            break;
        case IDVENDOR:
            udev_enumerate_add_match_sysattr(enumerate, "idVendor", values[i].c_str());
            break;
        case IDPRODUCT:
            udev_enumerate_add_match_sysattr(enumerate, "idProduct", values[i].c_str());
            break;
        case SYSNAME:
            udev_enumerate_add_match_sysname(enumerate, values[i].c_str());
            break;
        }
    }

    std::vector<deviceAttributes> devicesAttributes;
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    udev_list_entry_foreach(dev_list_entry, devices)
    {
        const char *path;
        path = udev_list_entry_get_name(dev_list_entry);
        dev = udev_device_new_from_syspath(udev, path);

        deviceAttributes deviceInfo;
//        printf("Device Node Path: %s\n", udev_device_get_devnode(dev));
//        printf("devpath: %s\n", udev_device_get_devpath(dev));
//        printf("syspath: %s\n", udev_device_get_syspath(dev));
//        printf("sysname: %s\n", udev_device_get_sysname(dev));
//        printf("sysnum: %s\n", udev_device_get_sysnum(dev));
//        printf("VID/PID: %s %s\n", udev_device_get_sysattr_value(dev, "idVendor"), udev_device_get_sysattr_value(dev, "idProduct"));

        deviceInfo.devnode = udev_device_get_devnode(dev);
        deviceInfo.devpath = udev_device_get_devpath(dev);
        deviceInfo.syspath = udev_device_get_syspath(dev);
        deviceInfo.sysname = udev_device_get_sysname(dev);
        deviceInfo.sysnum = udev_device_get_sysnum(dev);
        const char* idVendor = udev_device_get_sysattr_value(dev, "idVendor");
        const char* idProduct = udev_device_get_sysattr_value(dev, "idProduct");
        idVendor != NULL ? deviceInfo.idVendor = idVendor : deviceInfo.idVendor = "";
        idProduct != NULL ? deviceInfo.idProduct = idProduct : deviceInfo.idProduct = "";

        devicesAttributes.push_back(deviceInfo);
        if (!dev) {
            printf("Unable to find parent usb device.");
            exit(1);
        }
        udev_device_unref(dev);
    }
    udev_enumerate_unref(enumerate);
    udev_unref(udev);
    return devicesAttributes;
}
