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
        ApplicationNodeImplementation::InitOpenGL();
    }

    void CoordinatorNode::CleanUp() {
        ApplicationNodeBase::CleanUp();
    }

    //TODO cleanup and fix
    void CoordinatorNode::UpdateFrame(double currenttime, double elapsedtime) {
        glm::vec2 displayPos = GetDisplayPosition(TrackedDeviceIdentifier::CONTROLLER_LEFT_HAND);
        posdx = displayPos[0] * 2 - 1;
        posdy = displayPos[1] * 2 - 1;
        
        //tracks the Controller pointing position
        mousepointModelMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3((float)posdx*(GetConfig().nearPlaneSize_.x), (float)posdy, 0.0f));
        ApplicationNodeImplementation::UpdateFrame(currenttime, elapsedtime);
    }
    /*
    bool CoordinatorNode::ControllerButtonPressedCallback(TrackedDeviceIdentifier trackedDevice, ControllerButtonIdentifier buttonid, glm::vec2 axisvalues)
    {
        if(ApplicationNodeBase::ControllerButtonPressedCallback(trackedDevice, buttonid, axisvalues)) return true;

        //TODO handle values
        return false;
    }*/

    /*bool CoordinatorNode::ControllerButtonTouchedCallback(TrackedDeviceIdentifier trackedDevice, ControllerButtonIdentifier buttonid, glm::vec2 axisvalues)
    {
        if (ApplicationNodeBase::ControllerButtonTouchedCallback(trackedDevice, buttonid, axisvalues)) return true;
        //TODO handle values
        return false;
    }
    */

    //TODO use values from Master Node internal
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
                if (InitialiseVR()) {
                    ImGui::Text("Vr Init succesful");
                }
                ImGui::Checkbox("Use left controller as main controller", &useLeftHandController);
                ImGui::NewLine();
                glm::vec2 displayPos = GetDisplayPosition(useLeftHandController?TrackedDeviceIdentifier::CONTROLLER_LEFT_HAND:TrackedDeviceIdentifier::CONTROLLER_RIGHT_HAND);
                
                glm::vec3 position = GetControllerPosition(TrackedDeviceIdentifier::CONTROLLER_LEFT_HAND);
                glm::vec3 zvector = GetControllerZVector(TrackedDeviceIdentifier::CONTROLLER_LEFT_HAND);
                glm::quat rotation = GetControllerRotation(TrackedDeviceIdentifier::CONTROLLER_LEFT_HAND);
                ImGui::Text("Controller 0: position x: %.2f, y: %.2f, z: %.2f", position[0], position[1], position[2]);
                ImGui::Text("              zVector x: %.2f, y: %.2f, z: %.2f", zvector[0], zvector[1], zvector[2]);
                ImGui::Text("              rotation w: %.2f, x: %.2f, y: %.2f, z: %.2f", rotation[0], rotation[1], rotation[2], rotation[3]);
                //ImGui::Text("              buttons pressed: ");
                

                position = GetControllerPosition(TrackedDeviceIdentifier::CONTROLLER_RIGHT_HAND);
                zvector = GetControllerZVector(TrackedDeviceIdentifier::CONTROLLER_RIGHT_HAND);
                rotation = GetControllerRotation(TrackedDeviceIdentifier::CONTROLLER_RIGHT_HAND);
                ImGui::NewLine();
                ImGui::Text("Controller 1: position x: %.2f, y: %.2f, z: %.2f", position[0], position[1], position[2]);
                ImGui::Text("              zVector x: %.2f, y: %.2f, z: %.2f", zvector[0], zvector[1], zvector[2]);
                ImGui::Text("              rotation w: %.2f, x: %.2f, y: %.2f, z: %.2f", rotation[0], rotation[1], rotation[2], rotation[3]);
                //ImGui::Text("              buttons pressed: ");
                
                
                ImGui::NewLine();
                ImGui::Text("Display Pointer Position x: %.2f y %.2f", displayPos[0], displayPos[1]);
                //ImGui::Text("midDisplaypos x: %.2f y: %.2f z: %.2f", midDisplayPos.v[0], midDisplayPos.v[1], midDisplayPos.v[2]);
                position = GetControllerPosition(TrackedDeviceIdentifier::GENERIC_TRACKER);
                ImGui::Text("Tracker    position x: %.2f, y: %.2f, z: %.2f", position[0], position[1], position[2]);
                ImGui::Text("           rotation w: %.2f, x: %.2f, y: %.2f, z: %.2f", rotation[0], rotation[1], rotation[2], rotation[3]);
                //ImGui::Text("SGCT Tracker Position x: %.2f, y: %.2f, z: %.2f", sgctTrackerPos.v[0], sgctTrackerPos.v[1], sgctTrackerPos.v[2]);
                ImGui::NewLine();
                if (ImGui::Button("Calibrate by Touching")) {
                    CalibrateVR(CalibrateMethod::CALIBRATE_BY_TOUCHING, useLeftHandController ? TrackedDeviceIdentifier::CONTROLLER_LEFT_HAND : TrackedDeviceIdentifier::CONTROLLER_RIGHT_HAND);
                }
                if (ImGui::Button("Calibrate by Pointing")) {
                    CalibrateVR(CalibrateMethod::CALIBRATE_BY_POINTING, useLeftHandController ? TrackedDeviceIdentifier::CONTROLLER_LEFT_HAND : TrackedDeviceIdentifier::CONTROLLER_RIGHT_HAND);
                }                   
                ImGui::NewLine();
                ImGui::Text("Display Edges");
               // ImGui::Text("lower left x: %.2f y: %.2f z: %.2f ", displayEdges[0][0], displayEdges[0][1], displayEdges[0][2]);
               // ImGui::Text("upper left x: %.2f y: %.2f z: %.2f ", displayEdges[1][0], displayEdges[1][1], displayEdges[1][2]);
                //ImGui::Text("lower left x: %.2f y: %.2f z: %.2f ", displayEdges[2][0], displayEdges[2][1], displayEdges[2][2]);
            }
            ImGui::End();
        });
        ApplicationNodeImplementation::Draw2D(fbo);
    }
}
