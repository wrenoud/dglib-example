#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <dglib/DgGeoSphRF.h>
#include <dglib/DgIDGGSBase.h>
#include <dglib/DgRFNetwork.h>
#include <dglib/DgZ7RF.h>
#include <dglib/DgZ7StringRF.h>

int main(int argc, char *argv[]) {
    // DgRFNetwork is responsible for memory management, so no need to manage the raw pointers here.
    DgRFNetwork network;

    // Geographic reference frame parameters
    const std::string datum("WGS84_AUTHALIC_SPHERE");
    double earthRadius = WGS84_AUTHALIC_RADIUS_KM;

    auto *geographicRF = DgGeoSphRF::makeRF(network, datum, earthRadius);
    auto *geographicDegRF = DgGeoSphDegRF::makeRF(*geographicRF);

    // IGEO7 parameters
    DgGeoCoord vert0(11.25L, 58.28252559L, false);
    double azimuthDegs = 0;
    int aperture = 7;
    int actualRes = 9;
    DgGridTopology gridTopo = DgGridTopology::Hexagon;
    DgGridMetric gridMetric = DgGridMetric::D6;
    std::string name = "IGEO7";
    std::string projType = "ISEA";

    auto *dggs = DgIDGGSBase::makeRF(network, *geographicRF, vert0, azimuthDegs, aperture, actualRes + 2, gridTopo,
                                     gridMetric, name, projType);

    auto &dgg = dggs->idggBase(actualRes);
    auto *z7RF = dgg.z7RF();

    // Test point in degrees
    DgDVec2D testPoint{5.1214106, 52.0906107};
    auto locationGeo = geographicDegRF->makeLocation(testPoint);

    // Convert to the DGG's RF
    DgLocation locationDgg(*locationGeo);
    dgg.convert(&locationDgg);

    // Get the geographic cell center
    DgLocation locationGeoCellCenter(locationDgg);
    geographicDegRF->convert(&locationGeoCellCenter);

    //< Convert to Z7 RF
    DgLocation locationZ7(locationDgg);
    z7RF->convert(&locationZ7);

    // Show representations
    std::cout << "       Test point: " << locationGeo->asAddressString() << "\n\n";
    std::cout << "      Zone center: " << locationGeoCellCenter.asAddressString() << "\n";
    std::cout << "      DGG Zone ID: " << locationDgg.asString() << "\n";
    std::cout << "Z7 index (base16): " << locationZ7.asAddressString() << "\n";

    // Get the Z7 integer address
    auto addressZ7 = static_cast<const DgAddress<DgZ7Coord> *>(locationZ7.address());
    std::cout << "Z7 index (base10): " << addressZ7->address().value() << "\n";

    return 0;
}
