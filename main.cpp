#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <dglib/DgGeoSphRF.h>
#include <dglib/DgIDGGSBase.h>
#include <dglib/DgRFNetwork.h>
#include <dglib/DgZ7RF.h>
#include <dglib/DgZ7StringRF.h>

int main(int argc, char *argv[]) {
    DgRFNetwork network;

    // Geographic reference frame parameters
    const std::string datum("WGS84_AUTHALIC_SPHERE");
    double earthRadius = 6371.0071809184747;

    auto geographicRF = std::unique_ptr<const DgGeoSphRF>(DgGeoSphRF::makeRF(network, datum, earthRadius));
    auto geographicDegRF = std::unique_ptr<const DgGeoSphDegRF>(DgGeoSphDegRF::makeRF(*geographicRF));

    // IGEO7 parameters
    DgDVec2D vert0{0.19634954084936207, 1.0172219679233507};
    double azimuthDegs = 0;
    int aperture = 7;
    int actualRes = 9;
    DgGridTopology gridTopo = DgGridTopology::Hexagon;
    DgGridMetric gridMetric = DgGridMetric::D6;
    std::string projType = "ISEA";

    auto dggs = std::unique_ptr<const DgIDGGSBase>(DgIDGGSBase::makeRF(network, *geographicRF, vert0, azimuthDegs,
                                                                       aperture, actualRes + 2, gridTopo, gridMetric,
                                                                       "IDGGS", projType));

    auto &dgg = dggs->idggBase(actualRes);
    auto *z7RF = dgg.z7RF();

    // Test point in degrees
    DgDVec2D testPoint{5.1214106, 52.0906107};
    auto locationGeo = std::unique_ptr<const DgLocation>(geographicDegRF->makeLocation(testPoint));

    // Convert to the DGG's RF
    DgLocation locationDgg(*locationGeo);
    dgg.convert(&locationDgg);

    //< Convert to Z7 RF
    DgLocation locationZ7(locationDgg);
    z7RF->convert(&locationZ7);

    // Show representations
    std::cout << locationGeo->asString() << std::endl;
    std::cout << locationDgg.asString() << std::endl;
    std::cout << locationZ7.asString() << std::endl;
    std::cout << locationZ7.asAddressString() << std::endl;

    // Get the Z7 integer address
    auto addressZ7 = static_cast<const DgAddress<DgZ7Coord> *>(locationZ7.address());
    std::cout << addressZ7->address().value() << std::endl;

    return 0;
}
