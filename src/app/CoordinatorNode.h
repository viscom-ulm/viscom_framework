/**
 * @file   CoordinatorNode.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.25
 *
 * @brief  Declaration of the ApplicationNodeImplementation for the coordinator node.
 */

#pragma once

#include "app/ApplicationNodeImplementation.h"
#include <openvr.h>

namespace viscom {

    class CoordinatorNode final : public ApplicationNodeImplementation
    {
    public:
        explicit CoordinatorNode(ApplicationNodeInternal* appNode);
        virtual ~CoordinatorNode() override;

        void Draw2D(FrameBuffer& fbo) override;
        void InitOpenGL() override;
        void CleanUp() override;

        void UpdateFrame(double currenttime, double) override;

        bool vrInitSucc = false;
        bool bAcquireTrackingDataByWaitingForVREvents = false;

    private:
        bool ProcessVREvent(const vr::VREvent_t & event);
        void ParseTrackingFrame();
        void InitDisplay(vr::HmdVector3_t);

        void InitDisplayFloor(vr::HmdVector3_t cpos, vr::HmdVector3_t cz);

        void InitDisplayFromFile();

        void WriteInitDisplayToFile();

        void OutputDevices();

        void HandleSCGT(glm::vec3 pos, glm::quat q);

        vr::HmdVector3_t GetPosition(vr::HmdMatrix34_t matrix);
        vr::HmdVector3_t GetZVector(vr::HmdMatrix34_t matrix);
        vr::HmdQuaternion_t GetRotation(vr::HmdMatrix34_t matrix);

        vr::HmdVector2_t GetDisplayPosVector(vr::HmdVector3_t position, vr::HmdVector3_t zvector, vr::HmdVector3_t display_lowerLeftCorner, vr::HmdVector3_t display_upperLeftCorner, vr::HmdVector3_t display_lowerRightCorner);

        vr::IVRSystem *m_pHMD;
        vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];

        vr::HmdVector3_t position;
        vr::HmdVector3_t zvector;
        vr::HmdVector2_t displayPos;
        vr::HmdVector3_t trackerPos;
        vr::HmdVector3_t midDisplayPos;
        vr::HmdVector3_t sgctTrackerPos;


        bool initDisplay = true;
        bool displayllset = false;
        bool displayulset = false;
        bool displaylrset = false;
        bool initfloor = true;

        /** first row lowerLeftCorner, second row upperLeftCorner, third row lowerRightCorner **/
        float displayEdges[3][3] = { { -1.7f, -0.2f, -3.0f },
        { -1.7f, 1.5f, -3.0f },
        { 1.8f, -0.28f, -3.0f } };
        float posdx, posdy;
        std::vector <std::string> devices;

    };
}
