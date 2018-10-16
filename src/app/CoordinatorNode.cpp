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
//#include <openvr.h>
#include <glm\gtc\matrix_transform.hpp>

namespace viscom {

    CoordinatorNode::CoordinatorNode(ApplicationNodeInternal* appNode) :
        ApplicationNodeImplementation{ appNode }
    {

    }

    CoordinatorNode::~CoordinatorNode() = default;

    void CoordinatorNode::InitOpenGL() 
    {
        initVr_ = InitialiseVR();
        ApplicationNodeImplementation::InitOpenGL();
    }

    void CoordinatorNode::CleanUp() {
        ApplicationNodeBase::CleanUp();
    }

    //TODO cleanup and fix
    void CoordinatorNode::UpdateFrame(double currenttime, double elapsedtime) {
        connectedDevices_ = GetConnectedDevices();
        size_t controllerindex=0;
        for (DeviceInfo d : connectedDevices_) {
            if (useLeftHandController&& d.deviceRole == TrackedDeviceRole::CONTROLLER_LEFT_HAND) {
                controllerindex = d.deviceId;
            }
            if (!useLeftHandController && d.deviceRole == TrackedDeviceRole::CONTROLLER_RIGHT_HAND) {
                controllerindex = d.deviceId;
            }
        }
        displayPos = GetDisplayPointerPosition(controllerindex);
        posdx = displayPos[0] * 2 - 1;
        posdy = displayPos[1] * 2 - 1;
        
        //tracks the Controller pointing position
        mousepointModelMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3((float)posdx*(GetConfig().nearPlaneSize_.x), (float)posdy, 0.0f));
        ApplicationNodeImplementation::UpdateFrame(currenttime, elapsedtime);
    }
    //TODO Demo with Callbacks

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
                    ImGui::Text("Vr Init succesful");
                    ImGui::Checkbox("Use left controller as main controller", &useLeftHandController);
                    ImGui::NewLine();

                    size_t trackerid = 65;
                    glm::vec3 position = glm::vec3();
                    glm::vec3 zvector = glm::vec3();
                    glm::quat rotation = glm::quat();
                    for (DeviceInfo d : connectedDevices_) {
                        if (d.deviceClass == TrackedDeviceClass::CONTROLLER) {
                            glm::vec3 position = GetControllerPosition(d.deviceId);
                            glm::vec3 zvector = GetControllerZVector(d.deviceId);
                            glm::quat rotation = GetControllerRotation(d.deviceId);
                            ImGui::Text("Controller %i: position x: %.2f, y: %.2f, z: %.2f", d.deviceId, position[0], position[1], position[2]);
                            ImGui::Text("              zVector x: %.2f, y: %.2f, z: %.2f", zvector[0], zvector[1], zvector[2]);
                            ImGui::Text("              rotation w: %.2f, x: %.2f, y: %.2f, z: %.2f", rotation[0], rotation[1], rotation[2], rotation[3]);
                        }
                        if (d.deviceClass == TrackedDeviceClass::GENERIC_TRACKER) trackerid = d.deviceId;
                    }

                    ImGui::NewLine();
                    ImGui::Text("Display Pointer Position x: %.2f y %.2f", displayPos[0], displayPos[1]);
                    //ImGui::Text("midDisplaypos x: %.2f y: %.2f z: %.2f", midDisplayPos.v[0], midDisplayPos.v[1], midDisplayPos.v[2]);
                    if (trackerid != 65) {
                        position = GetControllerPosition(trackerid);
                        rotation = GetControllerRotation(trackerid);
                        ImGui::Text("Tracker    position x: %.2f, y: %.2f, z: %.2f", position[0], position[1], position[2]);
                        ImGui::Text("           rotation w: %.2f, x: %.2f, y: %.2f, z: %.2f", rotation[0], rotation[1], rotation[2], rotation[3]);
                    }
                    //ImGui::Text("SGCT Tracker Position x: %.2f, y: %.2f, z: %.2f", sgctTrackerPos.v[0], sgctTrackerPos.v[1], sgctTrackerPos.v[2]);
                    ImGui::NewLine();
                    if (ImGui::Button("Calibrate by Touching")) {
                        CalibrateVR(CalibrateMethod::CALIBRATE_BY_TOUCHING);
                    }
                    if (ImGui::Button("Calibrate by Pointing")) {
                        CalibrateVR(CalibrateMethod::CALIBRATE_BY_POINTING);
                    }
                    ImGui::NewLine();
                    ImGui::Text("Connected devices:");
                    for (DeviceInfo d : connectedDevices_) {
                        switch (d.deviceClass)
                        {
                        case TrackedDeviceClass::CONTROLLER:
                            switch (d.deviceRole)
                            {
                            case TrackedDeviceRole::CONTROLLER_LEFT_HAND:
                                ImGui::Text("Controller %i Role: Left Hand Controller", d.deviceId);
                                break;
                            case TrackedDeviceRole::CONTROLLER_RIGHT_HAND:
                                ImGui::Text("Controller %i Role: Right Hand Controller", d.deviceId);
                                break;
                            default:
                                ImGui::Text("Controller %i Role: No Role", d.deviceId);
                                break;
                            }
                            break;
                        case TrackedDeviceClass::GENERIC_TRACKER:
                            ImGui::Text("Tracker %i Role: Generic Tracker", d.deviceId);
                            break;
                        case TrackedDeviceClass::HMD:
                            ImGui::Text("HMD %i ", d.deviceId);
                            break;
                        case TrackedDeviceClass::DISPLAY_REDIRECT:
                            ImGui::Text("Display Redirect %i", d.deviceId);
                            break;
                        case TrackedDeviceClass::TRACKING_REFERENCE:
                            ImGui::Text("Tracking Referende %i", d.deviceId);
                            break;
                        default:
                            break;
                        }
                    }
                    
                }
                if (!initVr_) {
                    ImGui::Text("Open VR not initialised! Please start SteamVR.");
                }
                if (ImGui::Button("Reset Demo Circle")) {
                    demoCirclesMoved = false;
                }
                ImGui::Text("Circle Pos x: %f, y: %f, radius: %f", circlex_, circley_, circler_);
                ImGui::Text("Demo Score: %i", demoPoints);
            }
            ImGui::End();
        });
        ApplicationNodeImplementation::Draw2D(fbo);
    }


}
