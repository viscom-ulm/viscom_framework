/**
 * @file   MasterNode.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.25
 *
 * @brief  Declaration of the ApplicationNodeImplementation for the master node.
 */

#pragma once

#include "../app/ApplicationNodeImplementation.h"
#include <openvr.h>

namespace viscom {

    class MasterNode final : public ApplicationNodeImplementation
    {
    public:
        explicit MasterNode(ApplicationNodeInternal* appNode);
        virtual ~MasterNode() override;

        void Draw2D(FrameBuffer& fbo) override;
        void InitOpenGL() override;
        void CleanUp() override;

        void UpdateFrame(double currenttime,double) override;

        bool vrInitSucc = false;
        bool bAcquireTrackingDataByWaitingForVREvents = false;

    private:
        bool ProcessVREvent(const vr::VREvent_t & event);
        void ParseTrackingFrame();

        vr::HmdVector3_t GetPosition(vr::HmdMatrix34_t matrix);
        vr::HmdQuaternion_t GetRotation(vr::HmdMatrix34_t matrix);

        vr::IVRSystem *m_pHMD;
        vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
       
        
    };
    
}
