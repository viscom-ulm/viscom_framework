/**
 * @file   CoordinatorNode.cpp
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.25
 *
 * @brief  Implementation of the coordinator application node.
 */

#include "CoordinatorNode.h"
#include <iostream>
#include <fstream>
#include <imgui.h>
#include <openvr.h>
#include <glm\gtc\matrix_transform.hpp>

namespace viscom {

    CoordinatorNode::CoordinatorNode(ApplicationNodeInternal* appNode) :
        ApplicationNodeImplementation{ appNode }
    {
    }

    CoordinatorNode::~CoordinatorNode() = default;

    void CoordinatorNode::InitOpenGL() {
        /*vr::EVRInitError peError;
        // VRApplication_Scene (starts SteamVR no proper data) VRApplication_Overlay (starts SteamVR no SteamVRHome)  VRApplication_Background (doesn't start SteamVR uses SteamVRHome)
        m_pHMD = vr::VR_Init(&peError, vr::EVRApplicationType::VRApplication_Background);
        if (peError == vr::VRInitError_None) {
            vrInitSucc = true;
            OutputDevices();
        }*/
        ApplicationNodeImplementation::InitOpenGL();
    }

    void CoordinatorNode::CleanUp() {
        //vr::VR_Shutdown();
        ApplicationNodeBase::CleanUp();
    }

    void CoordinatorNode::UpdateFrame(double currenttime, double elapsedtime) {
        /*if (!initDisplay) {
            vr::VREvent_t event;
            vr::TrackedDevicePose_t trackedDevicePose;
            vr::TrackedDevicePose_t *devicePose = &trackedDevicePose;

            if (!initfloor) {
                while (m_pHMD->PollNextEvent( &event, sizeof(event))) {
                    if (event.eventType == vr::VREvent_ButtonPress && event.data.controller.button == vr::k_EButton_SteamVR_Trigger) {
                        vr::VRControllerState_t controllerState;
                        vr::VRControllerState_t *controllerState_ptr = &controllerState;

                        vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding,event.trackedDeviceIndex,&controllerState,sizeof(controllerState),&trackedDevicePose);
                        InitDisplay(VrGetPosition(devicePose->mDeviceToAbsoluteTracking.m));
                    }
                }
            }
            else {
                while (m_pHMD->PollNextEvent( &event, sizeof(event))) {
                    if (event.eventType == vr::VREvent_ButtonPress && event.data.controller.button == vr::k_EButton_SteamVR_Trigger) {
                        vr::VRControllerState_t controllerState;
                        vr::VRControllerState_t *controllerState_ptr = &controllerState;

                        vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, event.trackedDeviceIndex, &controllerState, sizeof(controllerState), &trackedDevicePose);
                        InitDisplayFloor(VrGetPosition(trackedDevicePose.mDeviceToAbsoluteTracking.m), GetZVector(trackedDevicePose.mDeviceToAbsoluteTracking.m));
                    }
                }
            }
        }*/
        /*if (bAcquireTrackingDataByWaitingForVREvents) {
            vr::VREvent_t event;
            while (m_pHMD->PollNextEvent(&event, sizeof(event))) {
                if (!ProcessVREvent(event)) {
                    //smth went wronng with the event
                }
                //ParseTrackingFrame();
            }
        }
        else {
           // ParseTrackingFrame();
        }

        float *displayPosTemp = GetDisplayPosVector(position.v, zvector.v, displayEdges[0],displayEdges[1],displayEdges[2]);
        displayPos.v[0] = displayPosTemp[0];
        displayPos.v[1] = displayPosTemp[1];
        posdx = displayPos.v[0] * 2 - 1;
        posdy = displayPos.v[1] * 2 - 1;
        */
        //tracks the Controller pointing position
        mousepointModelMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3((float)posdx*(GetConfig().nearPlaneSize_.x), (float)posdy, 0.0f));
        ApplicationNodeImplementation::UpdateFrame(currenttime, elapsedtime);
    }

    bool CoordinatorNode::ProcessVREvent(const vr::VREvent_t & event) {
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

    /*void CoordinatorNode::ParseTrackingFrame() {
        // Process SteamVR device states
        for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
        {
            if (GetIVRSystem() == NULL)
                continue;
            // if not connected just skip the rest of the routine
            if (!GetIVRSystem()->IsTrackedDeviceConnected(unDevice))
                continue;

            vr::TrackedDevicePose_t trackedDevicePose;
            vr::TrackedDevicePose_t *devicePose = &trackedDevicePose;

            vr::VRControllerState_t controllerState;
            vr::VRControllerState_t *controllerState_ptr = &controllerState;

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
                vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0, &trackedDevicePose, 1);

                // get the position and rotation
                position.v[0] = VrGetPosition(devicePose->mDeviceToAbsoluteTracking.m)[0];
                position.v[1] = VrGetPosition(devicePose->mDeviceToAbsoluteTracking.m)[1];
                position.v[2] = VrGetPosition(devicePose->mDeviceToAbsoluteTracking.m)[2];
                zvector.v[0] = GetZVector(devicePose->mDeviceToAbsoluteTracking.m)[0];
                zvector.v[1] = GetZVector(devicePose->mDeviceToAbsoluteTracking.m)[1];
                zvector.v[2] = GetZVector(devicePose->mDeviceToAbsoluteTracking.m)[2];
                quaternion.w = VrGetRotation(devicePose->mDeviceToAbsoluteTracking.m)[0];
                quaternion.x = VrGetRotation(devicePose->mDeviceToAbsoluteTracking.m)[1];
                quaternion.y = VrGetRotation(devicePose->mDeviceToAbsoluteTracking.m)[2];
                quaternion.z = VrGetRotation(devicePose->mDeviceToAbsoluteTracking.m)[3];

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

                vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, unDevice, &controllerState, sizeof(controllerState), &trackedDevicePose);

                position.v[0] = VrGetPosition(devicePose->mDeviceToAbsoluteTracking.m)[0];
                position.v[1] = VrGetPosition(devicePose->mDeviceToAbsoluteTracking.m)[1];
                position.v[2] = VrGetPosition(devicePose->mDeviceToAbsoluteTracking.m)[2];
                zvector.v[0] = GetZVector(devicePose->mDeviceToAbsoluteTracking.m)[0];
                zvector.v[1] = GetZVector(devicePose->mDeviceToAbsoluteTracking.m)[1];
                zvector.v[2] = GetZVector(devicePose->mDeviceToAbsoluteTracking.m)[2];
                quaternion.w = VrGetRotation(devicePose->mDeviceToAbsoluteTracking.m)[0];
                quaternion.x = VrGetRotation(devicePose->mDeviceToAbsoluteTracking.m)[1];
                quaternion.y = VrGetRotation(devicePose->mDeviceToAbsoluteTracking.m)[2];
                quaternion.z = VrGetRotation(devicePose->mDeviceToAbsoluteTracking.m)[3];

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

                    /*char buf[1024];

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
                    *//*
                    break;

                case vr::TrackedControllerRole_RightHand:
                    char buf[1024];
                    /*
                    sprintf_s(buf, sizeof(buf), "\nRight Controller\nx: %.2f y: %.2f z: %.2f\n", position.v[0], position.v[1], position.v[2]);
                    printf_s(buf);

                    sprintf_s(buf, sizeof(buf), "z Vector: x: %.2f y: %.2f z: %.2f\n", zvector.v[0], zvector.v[1], zvector.v[2]);
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
                    *//*
                    break;

                case vr::TrackedDeviceClass_TrackingReference:
                    // incomplete code, only here for switch reference
                    sprintf_s(buf, sizeof(buf), "Camera / Base Station");
                    printf_s(buf);
                    break;
                }
                break;

            case vr::ETrackedDeviceClass::TrackedDeviceClass_GenericTracker:
                vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, unDevice, &controllerState, sizeof(controllerState), &trackedDevicePose);

                trackerPos.v[0] = VrGetPosition(devicePose->mDeviceToAbsoluteTracking.m)[0];
                trackerPos.v[1] = VrGetPosition(devicePose->mDeviceToAbsoluteTracking.m)[1];
                trackerPos.v[2] = VrGetPosition(devicePose->mDeviceToAbsoluteTracking.m)[2];
                zvector.v[0] = GetZVector(devicePose->mDeviceToAbsoluteTracking.m)[0];
                zvector.v[1] = GetZVector(devicePose->mDeviceToAbsoluteTracking.m)[1];
                zvector.v[2] = GetZVector(devicePose->mDeviceToAbsoluteTracking.m)[2];
                quaternion.w = VrGetRotation(devicePose->mDeviceToAbsoluteTracking.m)[0];
                quaternion.x = VrGetRotation(devicePose->mDeviceToAbsoluteTracking.m)[1];
                quaternion.y = VrGetRotation(devicePose->mDeviceToAbsoluteTracking.m)[2];
                quaternion.z = VrGetRotation(devicePose->mDeviceToAbsoluteTracking.m)[3];

                vVel = trackedDevicePose.vVelocity;
                vAngVel = trackedDevicePose.vAngularVelocity;
                eTrackingResult = trackedDevicePose.eTrackingResult;
                bPoseValid = trackedDevicePose.bPoseIsValid;
                if (bPoseValid) {
                    HandleSCGT(glm::vec3(trackerPos.v[0], trackerPos.v[1],trackerPos.v[2]), glm::dquat(quaternion.w, quaternion.x, quaternion.y, quaternion.z));
                }
                break;
            }
        }
    }*/

    //Get the vector representing the position
    /*vr::HmdVector3_t CoordinatorNode::GetPosition(vr::HmdMatrix34_t matrix) {
        vr::HmdVector3_t vector;
        vector.v[0] = matrix.m[0][3];
        vector.v[1] = matrix.m[1][3];
        vector.v[2] = matrix.m[2][3];
        return vector;
    }*/

    //Get the z-vector 
    /*vr::HmdVector3_t CoordinatorNode::GetZVector(vr::HmdMatrix34_t matrix) {
        vr::HmdVector3_t vector;
        vector.v[0] = matrix.m[0][2];
        vector.v[1] = matrix.m[1][2];
        vector.v[2] = matrix.m[2][2];
        return vector;
    }*/

    // Get the quaternion representing the rotation
    /*vr::HmdQuaternion_t CoordinatorNode::GetRotation(vr::HmdMatrix34_t matrix) {
        vr::HmdQuaternion_t q;

        q.w = sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2;
        q.x = sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2;
        q.y = sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2;
        q.z = sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2;
        q.x = copysign(q.x, matrix.m[2][1] - matrix.m[1][2]);
        q.y = copysign(q.y, matrix.m[0][2] - matrix.m[2][0]);
        q.z = copysign(q.z, matrix.m[1][0] - matrix.m[0][1]);
        return q;
    }*/

    // Get the vector representing the Display position (x,y) by passing 3 display edges position and zvector
    /*vr::HmdVector2_t CoordinatorNode::GetDisplayPosVector(vr::HmdVector3_t position, vr::HmdVector3_t zvector, vr::HmdVector3_t display_lowerLeftCorner, vr::HmdVector3_t display_upperLeftCorner, vr::HmdVector3_t display_lowerRightCorner) {
        vr::HmdVector3_t d1 = display_lowerLeftCorner;
        vr::HmdVector3_t d2 = {display_upperLeftCorner.v[0] - display_lowerLeftCorner.v[0], display_upperLeftCorner.v[1] - display_lowerLeftCorner.v[1], display_upperLeftCorner.v[2] - display_lowerLeftCorner.v[2]};
        vr::HmdVector3_t d3 = {display_lowerRightCorner.v[0]-display_lowerLeftCorner.v[0], display_lowerRightCorner.v[1] - display_lowerLeftCorner.v[1], display_lowerRightCorner.v[2] - display_lowerLeftCorner.v[2]};
        vr::HmdVector2_t result;

        result.v[1] = (position.v[0] * zvector.v[1] * d3.v[0] * zvector.v[2] - position.v[0] * zvector.v[1] * d3.v[2] * zvector.v[0] - position.v[1] * zvector.v[0] * d3.v[0] * zvector.v[2] + position.v[1] * zvector.v[0] * d3.v[2] * zvector.v[0] - d1.v[0] * zvector.v[1] * d3.v[0] * zvector.v[2] + d1.v[0] * zvector.v[1] * d3.v[2] * zvector.v[0] + d1.v[1] * zvector.v[0] * d3.v[0] * zvector.v[2] - d1.v[1] * zvector.v[0] * d3.v[2] * zvector.v[0] - position.v[0] * zvector.v[2] * d3.v[0] * zvector.v[1] + position.v[0] * zvector.v[2] * d3.v[1] * zvector.v[0] + position.v[2] * zvector.v[0] * d3.v[0] * zvector.v[1] - position.v[2] * zvector.v[0] * d3.v[1] * zvector.v[0] + d1.v[0] * zvector.v[2] * d3.v[0] * zvector.v[1] - d1.v[0] * zvector.v[2] * d3.v[1] * zvector.v[0] - d1.v[2] * zvector.v[0] * d3.v[0] * zvector.v[1] + d1.v[2] * zvector.v[0] * d3.v[1] * zvector.v[0]) / (d2.v[0] * zvector.v[1] * d3.v[0] * zvector.v[2] - d2.v[0] * zvector.v[1] * d3.v[2] * zvector.v[0] - d2.v[1] * zvector.v[0] * d3.v[0] * zvector.v[2] + d2.v[1] * zvector.v[0] * d3.v[2] * zvector.v[0] - d2.v[0] * zvector.v[2] * d3.v[0] * zvector.v[1] + d2.v[0] * zvector.v[2] * d3.v[1] * zvector.v[0] + d2.v[2] * zvector.v[0] * d3.v[0] * zvector.v[1] - d2.v[2] * zvector.v[0] * d3.v[1] * zvector.v[0]);
        result.v[0] = (position.v[0] * zvector.v[1] * d2.v[0] * zvector.v[2] - position.v[0] * zvector.v[1] * d2.v[2] * zvector.v[0] - position.v[1] * zvector.v[0] * d2.v[0] * zvector.v[2] + position.v[1] * zvector.v[0] * d2.v[2] * zvector.v[0] - d1.v[0] * zvector.v[1] * d2.v[0] * zvector.v[2] + d1.v[0] * zvector.v[1] * d2.v[2] * zvector.v[0] + d1.v[1] * zvector.v[0] * d2.v[0] * zvector.v[2] - d1.v[1] * zvector.v[0] * d2.v[2] * zvector.v[0] - position.v[0] * zvector.v[2] * d2.v[0] * zvector.v[1] + position.v[0] * zvector.v[2] * d2.v[1] * zvector.v[0] + position.v[2] * zvector.v[0] * d2.v[0] * zvector.v[1] - position.v[2] * zvector.v[0] * d2.v[1] * zvector.v[0] + d1.v[0] * zvector.v[2] * d2.v[0] * zvector.v[1] - d1.v[0] * zvector.v[2] * d2.v[1] * zvector.v[0] - d1.v[2] * zvector.v[0] * d2.v[0] * zvector.v[1] + d1.v[2] * zvector.v[0] * d2.v[1] * zvector.v[0]) / (d3.v[0] * zvector.v[1] * d2.v[0] * zvector.v[2] - d3.v[0] * zvector.v[1] * d2.v[2] * zvector.v[0] - d3.v[1] * zvector.v[0] * d2.v[0] * zvector.v[2] + d3.v[1] * zvector.v[0] * d2.v[2] * zvector.v[0] - d3.v[0] * zvector.v[2] * d2.v[0] * zvector.v[1] + d3.v[0] * zvector.v[2] * d2.v[1] * zvector.v[0] + d3.v[2] * zvector.v[0] * d2.v[0] * zvector.v[1] - d3.v[2] * zvector.v[0] * d2.v[1] * zvector.v[0]);
        midDisplayPos.v[0] = d1.v[0] + 0.5f * d2.v[0] + 0.5f * d3.v[0];
        midDisplayPos.v[1] = d1.v[1] + 0.5f * d2.v[1] + 0.5f * d3.v[1];
        midDisplayPos.v[2] = d1.v[2] + 0.5f * d2.v[2] + 0.5f * d3.v[2];
        return result;
    }*/

    void CoordinatorNode::Draw2D(FrameBuffer& fbo)
    {
        fbo.DrawToFBO([this]() {
            ImGui::ShowTestWindow();
            ImGui::SetNextWindowPos(ImVec2(60, 60), ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);
            ImGui::StyleColorsClassic();
            if (ImGui::Begin("MasterTestWindow", nullptr))
            {
                ImGui::Text("Hello World on Master!");
                if (vrInitSucc) {
                    ImGui::Text("Vr Init succesful");
                }
                ImGui::NewLine();
                ImGui::Text("Controller Position x: %.2f, y: %.2f, z: %.2f", position.v[0], position.v[1], position.v[2]);
                ImGui::Text("Controller zVector x: %.2f, y: %.2f, z: %.2f", zvector.v[0], zvector.v[1], zvector.v[2]);
                ImGui::Text("Display Pointer Position x: %.2f y %.2f", displayPos.v[0], displayPos.v[1]);
                ImGui::Text("midDisplaypos x: %.2f y: %.2f z: %.2f", midDisplayPos.v[0], midDisplayPos.v[1], midDisplayPos.v[2]);
                ImGui::Text("Tracker Position x: %.2f, y: %.2f, z: %.2f", trackerPos.v[0], trackerPos.v[1], trackerPos.v[2]);
                ImGui::Text("SGCT Tracker Position x: %.2f, y: %.2f, z: %.2f", sgctTrackerPos.v[0], sgctTrackerPos.v[1], sgctTrackerPos.v[2]);

                ImGui::NewLine();
                if (ImGui::Button("Init Display")) {
                    initDisplay = false;
                    displayllset = false;
                    displayulset = false;
                    displaylrset = false;
                }
                ImGui::SameLine();
                if (!initDisplay) {
                    if (initfloor) {
                        if (!displayllset) {
                            ImGui::Text("Touch the lower left Display corner and press the Trigger");
                        }
                        if (displayllset & !displaylrset) {
                            ImGui::Text("Touch the lower right Display corner and press the Trigger");
                        }
                        if (displayllset && !displayulset && displaylrset) {
                            ImGui::Text("Touch the upper left Display corner and press the Trigger");
                        }
                    }
                    else {
                        if (!displayllset) {
                            ImGui::Text("Touch the lower left Display corner and press the Trigger");
                        }
                        if (displayllset & !displayulset) {
                            ImGui::Text("Touch the upper left Display corner and press the Trigger");
                        }
                        if (displayllset && displayulset && !displaylrset) {
                            ImGui::Text("Touch the lower right Display corner and press the Trigger");
                        }
                    }
                }
                if (ImGui::Button("Load Display")) {
                    // InitDisplayFromFile();
                }
                ImGui::SameLine();
                if (ImGui::Button("Save Display")) {
                    //WriteInitDisplayToFile();
                }
                ImGui::Checkbox("init with Floor", &initfloor);
                ImGui::NewLine();
                ImGui::Text("Connected devices:", "");
                for (auto &device : devices) {
                    ImGui::Text(device.c_str());
                }
                ImGui::NewLine();
                ImGui::Text("Display Edges");
                ImGui::Text("lower left x: %.2f y: %.2f z: %.2f ", displayEdges[0][0], displayEdges[0][1], displayEdges[0][2]);
                ImGui::Text("upper left x: %.2f y: %.2f z: %.2f ", displayEdges[1][0], displayEdges[1][1], displayEdges[1][2]);
                ImGui::Text("lower left x: %.2f y: %.2f z: %.2f ", displayEdges[2][0], displayEdges[2][1], displayEdges[2][2]);
            }
            ImGui::End();
        });

        ApplicationNodeImplementation::Draw2D(fbo);
    }

    /*void CoordinatorNode::InitDisplay(vr::HmdVector3_t dpos) {
        if (!displayllset) {
            displayEdges[0][0] = dpos.v[0];
            displayEdges[0][1] = dpos.v[1];
            displayEdges[0][2] = dpos.v[1];
            displayllset = true;
            return;
        }
        if (!displayulset) {
            displayEdges[1][0] = dpos.v[0];
            displayEdges[1][1] = dpos.v[1];
            displayEdges[1][2] = dpos.v[1];
            displayulset = true;
            return;
        }
        if (!displaylrset) {
            displayEdges[2][0] = dpos.v[0];
            displayEdges[2][1] = dpos.v[1];
            displayEdges[2][2] = dpos.v[1];
            displaylrset = true;
        }
        initDisplay = true;
    }*/

    /*void CoordinatorNode::InitDisplayFloor(vr::HmdVector3_t cpos, vr::HmdVector3_t cz) {
        float t = (-cpos.v[1]) / cz.v[1];
        if (!displayllset) {
            displayEdges[0][0] = cpos.v[0] + t * cz.v[0];
            displayEdges[0][1] = 0.0f;
            displayEdges[0][2] = cpos.v[2] + t * cz.v[2];
            displayllset = true;
            return;
        }
        if (!displaylrset) {
            displayEdges[2][0] = cpos.v[0] + t * cz.v[0];
            displayEdges[2][1] = 0.0f;
            displayEdges[2][2] = cpos.v[2] + t * cz.v[2];
            displaylrset = true;
            return;
        }
        if (!displayulset) {
            float f = (displayEdges[0][0] * (displayEdges[2][2] - displayEdges[0][2]) - displayEdges[0][2] * (displayEdges[2][0] - displayEdges[0][0]) - cpos.v[0] * (displayEdges[2][2] - displayEdges[0][2]) + cpos.v[2] * (displayEdges[2][0] - displayEdges[0][0])) / (cz.v[0] * (displayEdges[2][2] - displayEdges[0][2]) - cz.v[2] * (displayEdges[2][0] - displayEdges[0][0]));
            displayEdges[1][0] = displayEdges[0][0]; //cpos.v[0] + f * cz.v[0];
            displayEdges[1][1] = cpos.v[1] + f * cz.v[1];
            displayEdges[1][2] = displayEdges[0][2]; //cpos.v[2] + f * cz.v[2];
            displayulset = true;
        }
        initDisplay = true;
    }*/

    /*void CoordinatorNode::InitDisplayFromFile() {
        std::ifstream myfile("displayEdges.txt");
        if (myfile.is_open()) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    myfile >> displayEdges[i][j];
                }
            }
        }
    }*/

    /*void CoordinatorNode::WriteInitDisplayToFile() {
        std::ofstream myfile;
        myfile.open("displayEdges.txt");
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                myfile << displayEdges[i][j] << " ";
            }
        }
        myfile.close();
    }*/

    void CoordinatorNode::OutputDevices() {
        for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++) {
            if (!m_pHMD->IsTrackedDeviceConnected(unDevice))
                continue;
            vr::ETrackedDeviceClass trackedDeviceClass = vr::VRSystem()->GetTrackedDeviceClass(unDevice);
            switch (trackedDeviceClass) {
            case(vr::ETrackedDeviceClass::TrackedDeviceClass_Controller):
                devices.push_back("Controller");
                break;
            case(vr::ETrackedDeviceClass::TrackedDeviceClass_GenericTracker):
                devices.push_back("GenericTracker");
                break;
            case(vr::ETrackedDeviceClass::TrackedDeviceClass_HMD):
                devices.push_back("HMD");
                break;
            case(vr::ETrackedDeviceClass::TrackedDeviceClass_TrackingReference):
                devices.push_back("TrackingRef");
                break;
            case(vr::ETrackedDeviceClass::TrackedDeviceClass_DisplayRedirect):
                devices.push_back("DisplayRedirect");
                break;
            case(vr::ETrackedDeviceClass::TrackedDeviceClass_Invalid):
                devices.push_back("Invalid");
                break;
            }
        }
    }

    //TODO fix wrong values
    void CoordinatorNode::HandleSCGT(glm::vec3 pos, glm::quat q) {
        pos.x -= midDisplayPos.v[0];
        pos.y -= midDisplayPos.v[1];
        pos.z -= midDisplayPos.v[2];
        sgctTrackerPos = { pos.x, pos.y, pos.z };
        // TODO: fix this. [8/10/2018 Sebastian Maisch]
        // GetApplication()->GetEngine()->getDefaultUserPtr()->setPos(pos);
        // GetApplication()->GetEngine()->getDefaultUserPtr()->setOrientation(q);
    }
}