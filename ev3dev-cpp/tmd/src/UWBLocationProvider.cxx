#include "UWBLocationProvider.hxx"

UWBLocationProvider::UWBLocationProvider(const std::string& devname, vec3d anchor1, vec3d anchor2) {
    uwb = new UWBIPS(anchor1, anchor2);
    int status = uwb->Init(devname);
}

UWBLocationProvider::~UWBLocationProvider() {
    delete uwb;
    uwb = NULL;
}

Location UWBLocationProvider::getLocation(int id) {
    vec3d location;
    try {
        location = uwb->getLocation(id);
        return Location {
            vec3d.x,
            vec3d.y
        };
    }
    catch {
        // oops
    }
}
