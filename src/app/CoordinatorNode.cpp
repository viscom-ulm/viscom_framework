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
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/random.hpp>

namespace viscom {

    CoordinatorNode::CoordinatorNode(ApplicationNodeInternal* appNode) :
        ApplicationNodeImplementation{ appNode }
    {
        initVr_ = InitialiseVR();
        InitialiseDisplayVR();
    }

    CoordinatorNode::~CoordinatorNode() = default;

    //TODO cleanup and fix
    void CoordinatorNode::UpdateFrame(double currenttime, double elapsedtime) {
        connectedDevices_ = GetConnectedDevices();
        std::uint32_t controllerindex = 0;
        for (ovr::DeviceInfo d : connectedDevices_) {
            if (useLeftHandController&& d.deviceRole_ == ovr::TrackedDeviceRole::CONTROLLER_LEFT_HAND) {
                controllerindex = d.deviceId_;
            }
            if (!useLeftHandController && d.deviceRole_ == ovr::TrackedDeviceRole::CONTROLLER_RIGHT_HAND) {
                controllerindex = d.deviceId_;
            }
        }
        glm::vec2 displayPos = GetDisplayPointerPosition(controllerindex);
        demoSyncInfoLocal_.displayPos0_.x = displayPos[0] * 2 - 1;
        demoSyncInfoLocal_.displayPos0_.y = displayPos[1] * 2 - 1;


        demoSyncInfoLocal_.circleData_.z = (static_cast<float>(currenttime) - circleMoveStartTime_) * 0.1f;

        glm::vec2 mouseCircleDistance = glm::vec2(GetConfig().nearPlaneSize_.x*(demoSyncInfoLocal_.displayPos0_.x - demoSyncInfoLocal_.circleData_.x), demoSyncInfoLocal_.displayPos0_.y - demoSyncInfoLocal_.circleData_.y);

        if (mouseCircleDistance.x*mouseCircleDistance.x + mouseCircleDistance.y*mouseCircleDistance.y < demoSyncInfoLocal_.circleData_.z*demoSyncInfoLocal_.circleData_.z) {
            demoSyncInfoLocal_.circleHit_ = true;
        }
        else {
            demoSyncInfoLocal_.circleHit_ = false;
        }

        glm::vec2 axisValues;
        viscom::ovr::ButtonState triggerButtonState;
        GetControllerButtonState(controllerindex, 33, axisValues, triggerButtonState);

        if (triggerButtonState == ovr::ButtonState::PRESSED && demoSyncInfoLocal_.circleHit_ || demoSyncInfoLocal_.circleData_.z > 0.5f) {
            demoSyncInfoLocal_.circleData_.x = glm::linearRand(-1.0f, 1.0f);
            demoSyncInfoLocal_.circleData_.y = glm::linearRand(-1.0f, 1.0f);

            circleMoveStartTime_ = static_cast<float>(currenttime);
            demoSyncInfoLocal_.circleData_.z = 0.0f;
            demoSyncInfoLocal_.circleHit_ = false;
        }

        ApplicationNodeImplementation::UpdateFrame(currenttime, elapsedtime);
    }


#ifdef VISCOM_USE_SGCT
    void CoordinatorNode::PreSync()
    {
        demoSyncInfoSynced_.setVal(demoSyncInfoLocal_);
    }
    void CoordinatorNode::UpdateSyncedInfo()
    {
        demoSyncInfoLocal_ = demoSyncInfoSynced_.getVal();
    }
    //TODO Check if encode decode needed
#endif

    //TODO use all values from Master Node internal
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
                if (initVr_) {
                    ImGui::Text("VR Init successful");
                    ImGui::Checkbox("Use left controller as main controller", &useLeftHandController);
                    ImGui::NewLine();

                    std::uint32_t trackerid = 65;
                    glm::vec3 position = glm::vec3();
                    glm::vec3 zvector = glm::vec3();
                    glm::quat rotation = glm::quat();
                    for (ovr::DeviceInfo d : connectedDevices_) {
                        if (d.deviceClass_ == ovr::TrackedDeviceClass::CONTROLLER) {
                            glm::vec3 position = GetControllerPosition(d.deviceId_);
                            glm::vec3 zvector = GetControllerDirection(d.deviceId_);
                            glm::quat rotation = GetControllerOrientation(d.deviceId_);
                            ImGui::Text("Controller %i: position x: %.2f, y: %.2f, z: %.2f", d.deviceId_, position[0], position[1], position[2]);
                            ImGui::Text("              zVector x: %.2f, y: %.2f, z: %.2f", zvector[0], zvector[1], zvector[2]);
                            ImGui::Text("              rotation w: %.2f, x: %.2f, y: %.2f, z: %.2f", rotation[0], rotation[1], rotation[2], rotation[3]);

                            ImGui::Text("              hand: %i", d.deviceRole_);


                            // Optionally display all the button states
                            for (int buttonId = 0; buttonId < 64; buttonId++) {
                                glm::vec2 axis;
                                ovr::ButtonState buttonState;
                                GetControllerButtonState(d.deviceId_, buttonId, axis, buttonState);
                                ImGui::Text("              Button %i state: %i (%.2f, %.2f)", buttonId, buttonState, axis.x, axis.y);
                            }

                        }
                        if (d.deviceClass_ == ovr::TrackedDeviceClass::GENERIC_TRACKER) trackerid = d.deviceId_;
                    }

                    ImGui::NewLine();
                    ImGui::Text("Display Pointer Position x: %.2f y %.2f", displayPos[0], displayPos[1]);
                    //ImGui::Text("midDisplaypos x: %.2f y: %.2f z: %.2f", midDisplayPos.v[0], midDisplayPos.v[1], midDisplayPos.v[2]);
                    if (trackerid != 65) {
                        position = GetControllerPosition(trackerid);
                        rotation = GetControllerOrientation(trackerid);
                        ImGui::Text("Tracker    position x: %.2f, y: %.2f, z: %.2f", position[0], position[1], position[2]);
                        ImGui::Text("           rotation w: %.2f, x: %.2f, y: %.2f, z: %.2f", rotation[0], rotation[1], rotation[2], rotation[3]);
                    }
                    //ImGui::Text("SGCT Tracker Position x: %.2f, y: %.2f, z: %.2f", sgctTrackerPos.v[0], sgctTrackerPos.v[1], sgctTrackerPos.v[2]);
                    ImGui::NewLine();
                    if (ImGui::Button("Calibrate by Touching")) {
                        CalibrateVR(ovr::CalibrateMethod::CALIBRATE_BY_TOUCHING);
                    }
                    if (ImGui::Button("Calibrate by Pointing")) {
                        CalibrateVR(ovr::CalibrateMethod::CALIBRATE_BY_POINTING);
                    }
                    ImGui::NewLine();
                    ImGui::Text("Connected devices:");
                    for (ovr::DeviceInfo d : connectedDevices_) {
                        switch (d.deviceClass_)
                        {
                        case ovr::TrackedDeviceClass::CONTROLLER:
                            switch (d.deviceRole_)
                            {
                            case ovr::TrackedDeviceRole::CONTROLLER_LEFT_HAND:
                                ImGui::Text("Controller %i Role: Left Hand Controller", d.deviceId_);
                                break;
                            case ovr::TrackedDeviceRole::CONTROLLER_RIGHT_HAND:
                                ImGui::Text("Controller %i Role: Right Hand Controller", d.deviceId_);
                                break;
                            default:
                                ImGui::Text("Controller %i Role: No Role", d.deviceId_);
                                break;
                            }
                            break;
                        case ovr::TrackedDeviceClass::GENERIC_TRACKER:
                            ImGui::Text("Tracker %i Role: Generic Tracker", d.deviceId_);
                            break;
                        case ovr::TrackedDeviceClass::HMD:
                            ImGui::Text("HMD %i ", d.deviceId_);
                            break;
                        case ovr::TrackedDeviceClass::DISPLAY_REDIRECT:
                            ImGui::Text("Display Redirect %i", d.deviceId_);
                            break;
                        case ovr::TrackedDeviceClass::TRACKING_REFERENCE:
                            ImGui::Text("Tracking Reference %i", d.deviceId_);
                            break;
                        default:
                            break;
                        }
                    }
                    
                }
                if (!initVr_) {
                    ImGui::Text("Open VR not initialized! Please start SteamVR.");
                }
                ImGui::Text("Circle Pos x: %f, y: %f, radius: %f", demoSyncInfoLocal_.circleData_.x, demoSyncInfoLocal_.circleData_.y, demoSyncInfoLocal_.circleData_.z);
                //ImGui::Text("Demo Score: %i", demoPoints);
            }
            ImGui::End();
        });
        ApplicationNodeImplementation::Draw2D(fbo);
    }

    /*bool CoordinatorNode::ControllerButtonPressedCallback(std::uint32_t trackedDeviceId, std::size_t buttonid, const glm::vec2& axisvalues) {
        if (ApplicationNodeBase::ControllerButtonPressedCallback(trackedDeviceId, buttonid, axisvalues)) return true;
        glm::vec2 displayPos = GetDisplayPointerPosition(trackedDeviceId);
#ifdef VISCOM_USE_SGCT
        if (buttonid == 33 &&
            demoSyncInfoLocal_.displayPos0_.x >= (demoSyncInfoLocal_.circleData_.x - (demoSyncInfoLocal_.circleData_.z / 12)) &&
            demoSyncInfoLocal_.displayPos0_.x <= (demoSyncInfoLocal_.circleData_.x + (demoSyncInfoLocal_.circleData_.z / 12)) &&
            demoSyncInfoLocal_.displayPos0_.y >= (demoSyncInfoLocal_.circleData_.y - demoSyncInfoLocal_.circleData_.z / 10) &&
            demoSyncInfoLocal_.displayPos0_.y <= (demoSyncInfoLocal_.circleData_.y + demoSyncInfoLocal_.circleData_.z / 10)) {
            if (demoSyncInfoLocal_.circleData_.z < 2) demoPoints += 50;
            if (demoSyncInfoLocal_.circleData_.z < 2) demoPoints += 25;
            if (demoSyncInfoLocal_.circleData_.z < 2) demoPoints += 15;
            if (demoSyncInfoLocal_.circleData_.z < 2) demoPoints += 10;
            demoCirclesMoved = false;
            return true;
        }
#endif // VISCOM_USE_SGCT
#ifndef VISCOM_USE_SGCT
        if (buttonid == 33 &&
            displayPos.x >= (circlex_ - circler_ / 12) &&
            displayPos.x <= (circlex_ + circler_ / 12) &&
            displayPos.y >= (circley_ - circler_ / 10) &&
            displayPos.y <= (circley_ + circler_ / 10)) {
            if (circler_ < 2) demoPoints += 50;
            if (circler_ < 4) demoPoints += 25;
            if (circler_ < 6) demoPoints += 15;
            if (circler_ < 8) demoPoints += 10;
            demoCirclesMoved = false;
            return true;
        }
#endif // !VISCOM_USE_SGCT

            return false;
        }*/
}