/**
 * @file   MasterNode.cpp
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.25
 *
 * @brief  Implementation of the master application node.
 */

#include "MasterNode.h"
#include <imgui.h>
#include <openvr.h>

namespace viscom {

    MasterNode::MasterNode(ApplicationNodeInternal* appNode) :
        ApplicationNodeImplementation{ appNode }
    {
    }

    MasterNode::~MasterNode() = default;

    void MasterNode::InitOpenGL() {
        vr::EVRInitError peError;
        m_pHMD = vr::VR_Init(&peError, vr::EVRApplicationType::VRApplication_Background);
        if (peError == vr::VRInitError_None) {
            vrInitSucc = true;
        }

        ApplicationNodeImplementation::InitOpenGL();
    }

    void MasterNode::CleanUp() {
        vr::VR_Shutdown();
        ApplicationNodeBase::CleanUp();
    }
    void MasterNode::UpdateFrame(double currenttime, double elapsedtime) {
        if (bAcquireTrackingDataByWaitingForVREvents) {
            vr::VREvent_t event;
            while (m_pHMD->PollNextEvent(&event, sizeof(event))) {
                if (!ProcessVREvent(event)) {
                    //smth went wronng with the event
                }
                ParseTrackingFrame();
            }
        }
        else {
            ParseTrackingFrame();
        }
        ApplicationNodeImplementation::UpdateFrame(currenttime, elapsedtime);
    }

    bool MasterNode::ProcessVREvent(const vr::VREvent_t & event) {
        switch (event.eventType)
        {
        case vr::VREvent_TrackedDeviceActivated:
        {
            //process Event
        }
        break;

        case vr::VREvent_TrackedDeviceDeactivated:
        {
            //process Event
        }
        break;

        case vr::VREvent_TrackedDeviceUpdated:
        {
            //process Event
        }
        break;

        case (vr::VREvent_DashboardActivated):
        {
            //process Event
        }
        break;

        case (vr::VREvent_DashboardDeactivated):
        {
            
            //process Event
        }
        break;

        case (vr::VREvent_ChaperoneDataHasChanged):
        {
            //process Event
        }
        break;

        case (vr::VREvent_ChaperoneSettingsHaveChanged):
        {
            //process Event
        }
        break;

        case (vr::VREvent_ChaperoneUniverseHasChanged):
        {
            //process Event
        }
        break;

        case (vr::VREvent_ApplicationTransitionStarted):
        {
            //process Event
        }
        break;

        case (vr::VREvent_ApplicationTransitionNewAppStarted):
        {
            //process Event
        }
        break;

        case (vr::VREvent_Quit):
        {
            //process Event
            return false;
        }
        break;

        case (vr::VREvent_ProcessQuit):
        {
            //process Event
            return false;
        }
        break;

        case (vr::VREvent_QuitAborted_UserPrompt):
        {
            //process Event
            return false;
        }
        break;

        case (vr::VREvent_QuitAcknowledged):
        {
            //process Event
            return false;
        }
        break;

        case (vr::VREvent_TrackedDeviceRoleChanged):
        {
            //process Event
            break;
        }

        case (vr::VREvent_TrackedDeviceUserInteractionStarted):
        {
            //process Event
            break;
        }

        default:
            //process Event
            break;
        }

        return true;
    }

    void MasterNode::ParseTrackingFrame() {
        // Process SteamVR device states
        for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
        {
            // if not connected just skip the rest of the routine
            if (!m_pHMD->IsTrackedDeviceConnected(unDevice))
                continue;

            vr::TrackedDevicePose_t trackedDevicePose;
            vr::TrackedDevicePose_t *devicePose = &trackedDevicePose;

            vr::VRControllerState_t controllerState;
            vr::VRControllerState_t *ontrollerState_ptr = &controllerState;

            vr::HmdVector3_t position;
            vr::HmdQuaternion_t quaternion;

            if (!vr::VRSystem()->IsInputAvailable()) {
                //TODO Error Message no Input available
            }

            bool bPoseValid = trackedDevicePose.bPoseIsValid;
            vr::HmdVector3_t vVel;
            vr::HmdVector3_t vAngVel;
            vr::ETrackingResult eTrackingResult;

            // Get what type of device it is and work with its data
            vr::ETrackedDeviceClass trackedDeviceClass = vr::VRSystem()->GetTrackedDeviceClass(unDevice);
            switch (trackedDeviceClass) {
            case vr::ETrackedDeviceClass::TrackedDeviceClass_HMD:
                // print stuff for the HMD here, see controller stuff in next case block

                // get pose relative to the safe bounds defined by the user (only if using TrackingUniverseStanding)
                vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseSeated, 0, &trackedDevicePose, 1);

                // get the position and rotation
                position = GetPosition(devicePose->mDeviceToAbsoluteTracking);
                quaternion = GetRotation(devicePose->mDeviceToAbsoluteTracking);

                // get some data
                vVel = trackedDevicePose.vVelocity;
                vAngVel = trackedDevicePose.vAngularVelocity;
                eTrackingResult = trackedDevicePose.eTrackingResult;
                bPoseValid = trackedDevicePose.bPoseIsValid;

                //TODO print the tracking data
                

                // and print some more info to the user about the state of the device/pose
                switch (eTrackingResult) {
                case vr::ETrackingResult::TrackingResult_Uninitialized:
                    //TODO Handle
                    break;
                case vr::ETrackingResult::TrackingResult_Calibrating_InProgress:
                    //TODO Handle
                    break;
                case vr::ETrackingResult::TrackingResult_Calibrating_OutOfRange:
                    //TODO Handle
                    break;
                case vr::ETrackingResult::TrackingResult_Running_OK:
                    //TODO Handle
                    break;
                case vr::ETrackingResult::TrackingResult_Running_OutOfRange:
                    //TODO Handle

                    break;
                default:
                    //TODO Handle
                    break;
                }

                //TODO check if pose is Valid
                


            case vr::ETrackedDeviceClass::TrackedDeviceClass_Controller:
                // Simliar to the HMD case block above, please adapt as you like
                // to get away with code duplication and general confusion

                vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseSeated, unDevice, &controllerState, sizeof(controllerState), &trackedDevicePose);

                position = GetPosition(devicePose->mDeviceToAbsoluteTracking);
                quaternion = GetRotation(devicePose->mDeviceToAbsoluteTracking);

                vVel = trackedDevicePose.vVelocity;
                vAngVel = trackedDevicePose.vAngularVelocity;
                eTrackingResult = trackedDevicePose.eTrackingResult;
                bPoseValid = trackedDevicePose.bPoseIsValid;

                switch (vr::VRSystem()->GetControllerRoleForTrackedDeviceIndex(unDevice)) {
                case vr::TrackedControllerRole_Invalid:
                    // invalid hand...
                    break;

                    //
                case vr::TrackedControllerRole_LeftHand:

                    char buf[1024];

                    sprintf_s(buf, sizeof(buf), "\nLeft Controller\nx: %.2f y: %.2f z: %.2f\n", position.v[0], position.v[1], position.v[2]);
                    printf_s(buf);

                    sprintf_s(buf, sizeof(buf), "qw: %.2f qx: %.2f qy: %.2f qz: %.2f\n", quaternion.w, quaternion.x, quaternion.y, quaternion.z);
                    printf_s(buf);

                    switch (eTrackingResult) {
                    case vr::ETrackingResult::TrackingResult_Uninitialized:
                        sprintf_s(buf, sizeof(buf), "Invalid tracking result\n");
                        printf_s(buf);
                        break;
                    case vr::ETrackingResult::TrackingResult_Calibrating_InProgress:
                        sprintf_s(buf, sizeof(buf), "Calibrating in progress\n");
                        printf_s(buf);
                        break;
                    case vr::ETrackingResult::TrackingResult_Calibrating_OutOfRange:
                        sprintf_s(buf, sizeof(buf), "Calibrating Out of range\n");
                        printf_s(buf);
                        break;
                    case vr::ETrackingResult::TrackingResult_Running_OK:
                        sprintf_s(buf, sizeof(buf), "Running OK\n");
                        printf_s(buf);
                        break;
                    case vr::ETrackingResult::TrackingResult_Running_OutOfRange:
                        sprintf_s(buf, sizeof(buf), "WARNING: Running Out of Range\n");
                        printf_s(buf);

                        break;
                    default:
                        sprintf_s(buf, sizeof(buf), "Default\n");
                        printf_s(buf);
                        break;
                    }

                    if (bPoseValid)
                        sprintf_s(buf, sizeof(buf), "Valid pose\n");
                    else
                        sprintf_s(buf, sizeof(buf), "Invalid pose\n");
                    printf_s(buf);

                    break;

                case vr::TrackedControllerRole_RightHand:
                    // incomplete code, look at left hand for reference
                    break;

                case vr::TrackedDeviceClass_TrackingReference:
                    // incomplete code, only here for switch reference
                    sprintf_s(buf, sizeof(buf), "Camera / Base Station");
                    printf_s(buf);
                    break;
                }

                break;
            }
        }
    }

    //Get the vector representing the position
    vr::HmdVector3_t MasterNode::GetPosition(vr::HmdMatrix34_t matrix) {
        vr::HmdVector3_t vector;

        vector.v[0] = matrix.m[0][3];
        vector.v[1] = matrix.m[1][3];
        vector.v[2] = matrix.m[2][3];

        return vector;
    }

    // Get the quaternion representing the rotation
    vr::HmdQuaternion_t MasterNode::GetRotation(vr::HmdMatrix34_t matrix) {
        vr::HmdQuaternion_t q;

        q.w = sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2;
        q.x = sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2;
        q.y = sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2;
        q.z = sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2;
        q.x = copysign(q.x, matrix.m[2][1] - matrix.m[1][2]);
        q.y = copysign(q.y, matrix.m[0][2] - matrix.m[2][0]);
        q.z = copysign(q.z, matrix.m[1][0] - matrix.m[0][1]);
        return q;
    }

    void MasterNode::Draw2D(FrameBuffer& fbo)
    {
        if (vrInitSucc) {
             
            fbo.DrawToFBO([]() {
                ImGui::ShowTestWindow();

                ImGui::SetNextWindowPos(ImVec2(700, 60), ImGuiSetCond_FirstUseEver);
                ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);
                if (ImGui::Begin("MasterTestWindow", nullptr, ImGuiWindowFlags_ShowBorders))
                {
                    ImGui::Text("Hello World on Master!");
                    ImGui::Text("Vr Init succesful");                
                }
                ImGui::End();
            });
        }
        else {
            fbo.DrawToFBO([]() {
                ImGui::ShowTestWindow();

                ImGui::SetNextWindowPos(ImVec2(700, 60), ImGuiSetCond_FirstUseEver);
                ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);
                if (ImGui::Begin("MasterTestWindow", nullptr, ImGuiWindowFlags_ShowBorders))
                {
                    ImGui::Text("Hello World on Master!");
                }
                ImGui::End();
            });
        }
        ApplicationNodeImplementation::Draw2D(fbo);
    }

}
