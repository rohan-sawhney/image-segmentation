#ifndef SEGMENT_MANAGER_H
#define SEGMENT_MANAGER_H

#include <Magick++.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "DataStructures.h"

using namespace Magick;

class SegmentManager {
public:
    // constructor
    SegmentManager();
    
    // segment
    int segment(Image& output, const Image& input, const double& sigma, const double& k);
};

#endif
