/*
 *
 *  Created on: Sep 14, 2015
 *      Author: tiagotrocoli
 */

#ifndef SRC_DEVICEUTILS_H_
#define SRC_DEVICEUTILS_H_

#include <string>
#include <vector>

typedef struct deviceAttributes {
    std::string devnode;
    std::string devpath;
    std::string syspath;
    std::string sysname;
    std::string sysnum;
    std::string idVendor;
    std::string idProduct;

} deviceAttributes;

enum deviceAttributesSearch {
    SUBSYSTEM,
    SYSNAME,
    IDVENDOR,
    IDPRODUCT
};

class DeviceUtils {
public:

    std::vector<deviceAttributes> getDeviceAtributesFromSubsystem(std::string subsystem);
    std::vector<deviceAttributes> getDeviceAtributesFromSysname(std::string sysname);
    std::vector<deviceAttributes> getDeviceAtributesFromIdVendorAndIdProductor(std::string idVendor, std::string idProduct);
    std::vector<deviceAttributes> getDevicesFrom(std::vector<deviceAttributesSearch> attributes, std::vector<std::string> values);
};

#endif /* SRC_DEVICEUTILS_H_ */
