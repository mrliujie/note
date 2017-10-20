// This file is part of OpenMVG, an Open Multiple View Geometry C++ library.

// Copyright (c) 2015 Pierre MOULON.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// The <cereal/archives> headers are special and must be included first.
#include <cereal/archives/json.hpp>

#include <openMVG/sfm/sfm.hpp>
#include <openMVG/features/feature.hpp>
#include <openMVG/features/image_describer.hpp>
#include <openMVG/image/image_io.hpp>
#include <software/SfM/SfMPlyHelper.hpp>

#include <openMVG/system/timer.hpp>
#include "openMVG/stl/stl.hpp"

#include "nonFree/sift/SIFT_describer_io.hpp"
#include "openMVG/features/image_describer_akaze_io.hpp"

#include "third_party/cmdLine/cmdLine.h"
#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"

#include <cstdlib>

#ifdef OPENMVG_USE_OPENMP
#include <omp.h>
#endif

#include "SfMInit_ImageListing.h"
#include "SfM_ComputeFeatures.hpp"
#include "SfM_AddPose.hpp"
#include <iomanip>

// ----------------------------------------------------
// OpenCV include
// ----------------------------------------------------
#include "opencv2/opencv.hpp"

// ----------------------------------------------------
// Multiple Images localization from an existing reconstruction
// ----------------------------------------------------
int main(int argc, char **argv)
{
    using namespace openMVG;
    using namespace openMVG::sfm;
//    SfM_List sfm_list;
//    sfm_list.init(argv[1], argv[2], 2457.6);
//    SfM_Data sfm_data = sfm_list.getSfMData();
//    SfM_ComputeFeatures computeFeatures;
//    computeFeatures.set_sfm_data(sfm_data);
//    computeFeatures.compute_features(argv[2], "SIFT");
//     AddPose add;
//     add.set_sfm_data(argv[1]);
//     add.compute_with_OpenCV(argv[2], argv[3]);
    
  //---------------------------------------
  // Read SfM Scene (image view & intrinsics data)
  //---------------------------------------
    std::string sSfM_Data_Filename = argv[1];
    
    SfM_Data sfm_data;
    if (!Load(sfm_data, sSfM_Data_Filename, ESfM_Data(ALL))) {
        std::cerr << std::endl
        << "The input SfM_Data file \""<< sSfM_Data_Filename << "\" cannot be read." << std::endl;
        return EXIT_FAILURE;
    }
    
    std::string sOutDir = argv[2];
    Save(sfm_data,
      stlplus::create_filespec(sOutDir, "cloud_and_poses", ".ply"),
         ESfM_Data(ALL));
    
    
    // Create the stream and check its status
    std::ofstream stream("test", std::ios::out | std::ios::binary);
    std::ofstream stream_err("err", std::ios::out | std::ios::binary);
    stream << std::fixed << std::setprecision (std::numeric_limits<double>::digits10 + 1);
    stream_err << std::fixed << std::setprecision (std::numeric_limits<double>::digits10 + 1);
    if (!stream.is_open())
        return false;
    if (!stream_err.is_open())
        return false;
      stream << "ply"
      << '\n' << "property double x"
      << '\n' << "property double y"
      << '\n' << "property double z"
      << '\n' << "property uchar red"
      << '\n' << "property uchar green"
      << '\n' << "property uchar blue"
      << '\n' << "end_header" << std::endl;

    
    Poses & poses = sfm_data.poses;
    for ( const auto & iterV : sfm_data.GetViews() )
    {
        Vec3 reconstruct_pose = Vec3::Zero();
        Vec3 prior_pose = Vec3::Zero();  
        const View * view = iterV.second.get();
        if (sfm_data.IsPoseAndIntrinsicDefined(view))
        {
            const geometry::Pose3 pose = sfm_data.GetPoseOrDie(view.second.get());

            Vec3 pose = poses.at(view->id_pose).center();
            stream << "pose : "
                << pose(0) << ' '
                << pose(1) << ' '
                << pose(2) << ' '
                << "0 255 0\n";
            reconstruct_pose = pose;
            std::cout << "pose" << std::endl;
            
            // Export pose priors as Blue points
            if (const sfm::ViewPriors *prior = dynamic_cast<sfm::ViewPriors*>(iterV.second.get()))
            {
                stream << "prior :"
                    << prior->pose_center_(0) << ' '
                    << prior->pose_center_(1) << ' '
                    << prior->pose_center_(2) << ' '
                    << "0 0 255\n";
                prior_pose = prior->pose_center_;
                std::cout << "11 : \n" << std::fixed << std::setprecision (std::numeric_limits<double>::digits10 + 1)
                << prior->pose_center_(0) << ' ' << prior->pose_center_(1) << "\n";
            }
            
        double dis = sqrt(pow(prior_pose(0) - reconstruct_pose(0), 2)
    + pow(prior_pose(1) - reconstruct_pose(1), 2));
       // std::cout << "pose : \n" <<  reconstruct_pose(0) << ' ' << reconstruct_pose(1) << "\n";
        std::cout << "prior : \n" << std::fixed << std::setprecision (std::numeric_limits<double>::digits10 + 1)
            << prior_pose(0) << ' ' << prior_pose(1) << "\n";
        stream_err << dis << "\n";
        }
        
  
    }
    return 0;
}

