/**
 * @file   ApplicationNodeImplementation.cpp
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.30
 *
 * @brief  Implementation of the application node class.
 */

#include "ApplicationNodeImplementation.h"
#include "Vertices.h"
#include <imgui.h>
#include "core/gfx/mesh/MeshRenderable.h"
#include "core/gfx/mesh/AnimMeshRenderable.h"
#include <iostream>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

namespace viscom {

    ApplicationNodeImplementation::ApplicationNodeImplementation(ApplicationNodeInternal* appNode) :
        ApplicationNodeBase{ appNode }
    {
        LOG(INFO) << "InitOpenGL called in AppNImpl";


        InitialiseVR();
        if (!InitialiseDisplayVR()) {
            CalibrateVR(ovr::CalibrateMethod::CALIBRATE_BY_POINTING);
        }


        backgroundProgram_ = GetGPUProgramManager().GetResource("backgroundGrid", std::vector<std::string>{ "backgroundGrid.vert", "backgroundGrid.frag" });
        backgroundMVPLoc_ = backgroundProgram_->getUniformLocation("MVP");

        triangleProgram_ = GetGPUProgramManager().GetResource("foregroundTriangle", std::vector<std::string>{ "foregroundTriangle.vert", "foregroundTriangle.frag" });
        triangleMVPLoc_ = triangleProgram_->getUniformLocation("MVP");

        mousepointProgram_ = GetGPUProgramManager().GetResource("mousePoint", std::vector<std::string>{"mousePoint.vert", "mousePoint.frag"});
        mousepointMVPLoc_ = mousepointProgram_->getUniformLocation("MVP");

        demoCirclesProgram_ = GetGPUProgramManager().GetResource("demoCircles", std::vector<std::string>{"demoCircles.vert", "demoCircles.frag"});
        demoCirclesMVPLoc_ = demoCirclesProgram_->getUniformLocation("MVP");
        demoCirclesHitLoc_ = demoCirclesProgram_->getUniformLocation("hitCircle");
        demoCirclesSizeLoc_ = demoCirclesProgram_->getUniformLocation("circleSize");

        teapotProgram_ = GetGPUProgramManager().GetResource("foregroundMesh", std::vector<std::string>{ "foregroundMesh.vert", "foregroundMesh.frag" });
        teapotVPLoc_ = teapotProgram_->getUniformLocation("viewProjectionMatrix");

        robotProgram_ = GetGPUProgramManager().GetResource("foregroundAnimatedMesh", std::vector<std::string>{ "foregroundAnimatedMesh.vert", "foregroundMesh.frag" });
        robotVPLoc_ = robotProgram_->getUniformLocation("viewProjectionMatrix");

        std::vector<GridVertex> gridVertices;
        std::vector<SimpleVertex> circleVertices;

        auto delta = 0.125f;
        for (auto x = -1.0f; x < 1.0f; x += delta) {
            auto green = (x + 1.0f) / 2.0f;

            for (float y = -1.0; y < 1.0; y += delta) {
                auto red = (y + 1.0f) / 2.0f;

                auto dx = 0.004f;
                auto dy = 0.004f;

                gridVertices.emplace_back(glm::vec3(x + dx, y + dy, -1.0f), glm::vec4(red, green, 0.0f, 1.0f));//right top
                gridVertices.emplace_back(glm::vec3(x - dx + delta, y + dy, -1.0f), glm::vec4(red, green, 0.0f, 1.0f));//left top
                gridVertices.emplace_back(glm::vec3(x - dx + delta, y - dy + delta, -1.0f), glm::vec4(red, green, 0.0f, 1.0f));//left bottom

                gridVertices.emplace_back(glm::vec3(x - dx + delta, y - dy + delta, -1.0f), glm::vec4(red, green, 0.0f, 1.0f));//left bottom
                gridVertices.emplace_back(glm::vec3(x + dx, y - dy + delta, -1.0f), glm::vec4(red, green, 0.0f, 1.0f));//right bottom
                gridVertices.emplace_back(glm::vec3(x + dx, y + dy, -1.0f), glm::vec4(red, green, 0.0f, 1.0f));//right top
            }
        }

        numBackgroundVertices_ = static_cast<unsigned int>(gridVertices.size());

        gridVertices.emplace_back(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        gridVertices.emplace_back(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        gridVertices.emplace_back(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

        gridVertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        circleVertices.emplace_back(glm::vec3(0.0f));
        //circleVertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

        numCirclesVertices_ = static_cast<unsigned int>(circleVertices.size());

        glGenBuffers(1, &vboBackgroundGrid_);
        glBindBuffer(GL_ARRAY_BUFFER, vboBackgroundGrid_);
        glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(GridVertex), gridVertices.data(), GL_STATIC_DRAW);

        glGenVertexArrays(1, &vaoBackgroundGrid_);
        glBindVertexArray(vaoBackgroundGrid_);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GridVertex), reinterpret_cast<GLvoid*>(offsetof(GridVertex, position_)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GridVertex), reinterpret_cast<GLvoid*>(offsetof(GridVertex, color_)));
        glBindVertexArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &vboCircles_);
        glBindBuffer(GL_ARRAY_BUFFER, vboCircles_);
        glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(SimpleVertex), circleVertices.data(), GL_STATIC_DRAW);

        glGenVertexArrays(1, &vaoCircles_);
        glBindVertexArray(vaoCircles_);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), reinterpret_cast<GLvoid*>(offsetof(SimpleVertex, position_)));
        glBindVertexArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        teapotMesh_ = GetMeshManager().GetResource("/models/teapot/teapot.obj");
        teapotRenderable_ = MeshRenderable::create<SimpleMeshVertex>(teapotMesh_.get(), teapotProgram_.get());

        robotMesh_ = GetMeshManager().GetResource("/models/robot2/robot2.fbx");
        robotRenderable_ = AnimMeshRenderable::create<AnimMeshVertex>(robotMesh_.get(), robotProgram_.get());
        SubAnimationMapping robotAnimationMappings{ AnimationInfo{ 0.0f, 119.0f, 1.0f } };
        robotAnimationState_ = std::make_unique<AnimationState>(robotMesh_.get(), robotAnimationMappings);
    }

    ApplicationNodeImplementation::~ApplicationNodeImplementation()
    {
        if (vaoBackgroundGrid_ != 0) glDeleteVertexArrays(1, &vaoBackgroundGrid_);
        vaoBackgroundGrid_ = 0;
        if (vboBackgroundGrid_ != 0) glDeleteBuffers(1, &vboBackgroundGrid_);
        vboBackgroundGrid_ = 0;
    }

    void ApplicationNodeImplementation::UpdateFrame(double currentTime, double)
    {
        if (!robotAnimationState_->IsPlaying()) robotAnimationState_->Play(currentTime);
        robotAnimationState_->UpdateTime(currentTime);
        robotAnimationState_->ComputeAnimationsFinalBonePoses();

        GetCamera()->SetPosition(camPos_);
        glm::quat pitchQuat = glm::angleAxis(camRot_.x, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat yawQuat = glm::angleAxis(camRot_.y, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat rollQuat = glm::angleAxis(camRot_.z, glm::vec3(0.0f, 0.0f, 1.0f));
        GetCamera()->SetOrientation(yawQuat * pitchQuat * rollQuat);

        //Tracks the mouse
        //connectedDevices_= GetConnectedDevices();
        //size_t controllerindex = 0;
        //for (DeviceInfo d : connectedDevices_) {
        //    if (d.deviceClass == TrackedDeviceClass::CONTROLLER && d.deviceRole == TrackedDeviceRole::CONTROLLER_LEFT_HAND) controllerindex = d.deviceId;
        //}
        //displayPos = GetDisplayPointerPosition(controllerindex);
        //displayPos.x = displayPos.x * 2 - 1;
        //displayPos.y = displayPos.y * 2 - 1;
#ifdef VISCOM_USE_SGCT
        mousepointModelMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3((float)demoSyncInfoLocal_.displayPos0_.x, (float)demoSyncInfoLocal_.displayPos0_.y, 0.0f));
#endif
#ifndef VISCOM_USE_SGCT
        mousepointModelMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3((float)displayPos.x, (float)displayPos.y, 0.0f));
#endif // !VISCOM_USE_SGCT

        //demoCirclesModelMatrix_ = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3((float)posdx*(GetConfig().nearPlaneSize_.x), (float)posdy*(-1.0f), 0.0f)),glm::vec3(static_cast<float>(currentTime)*2.0f));


#ifdef VISCOM_USE_SGCT
        demoCirclesModelMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3(demoSyncInfoLocal_.circleData_.x, demoSyncInfoLocal_.circleData_.y, 0.0f));   //glm::vec3((static_cast<float>(currentTime) - circleMoveStartTime)*2.0f));
#endif // VISCOM_USE_SGCT

#ifndef VISCOM_USE_SGCT
        demoCirclesModelMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3(circlex_, circley_, 0.0f));
#endif // !VISCOM_USE_SGCT

        hitCircle = 0;

#ifdef VISCOM_USE_SGCT
        if (glm::pow(demoSyncInfoLocal_.displayPos0_.x*GetConfig().nearPlaneSize_.x - demoSyncInfoLocal_.circleData_.x*GetConfig().nearPlaneSize_.x, 2.0) + glm::pow(demoSyncInfoLocal_.displayPos0_.y - demoSyncInfoLocal_.circleData_.y, 2.0) < glm::pow(demoSyncInfoLocal_.circleData_.z, 2.0)) {
            hitCircle = 1;
        }
#endif // VISCOM_USE_SGCT

#ifndef VISCOM_USE_SGCT
        if (glm::pow(displayPos.x*GetConfig().nearPlaneSize_.x - circlex_*GetConfig().nearPlaneSize_.x, 2.0) + glm::pow(displayPos.y - circley_, 2.0) < glm::pow(circler_, 2.0)) {
            hitCircle = 1;
        }
#endif // !VISCOM_USE_SGCT

        triangleModelMatrix_ = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)), static_cast<float>(currentTime), glm::vec3(0.0f, 1.0f, 0.0f));
        teapotModelMatrix_ = glm::scale(glm::rotate(glm::translate(glm::mat4(0.01f), glm::vec3(-3.0f, 0.0f, -5.0f)), static_cast<float>(currentTime), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(0.01f));
        robotModelMatrix_ = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.3f, 0.0f)), glm::vec3(0.005f));
    }

    void ApplicationNodeImplementation::ClearBuffer(FrameBuffer& fbo)
    {
        fbo.DrawToFBO([]() {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        });
    }

    void ApplicationNodeImplementation::DrawFrame(FrameBuffer& fbo)
    {
        fbo.DrawToFBO([this]() {
            glBindVertexArray(vaoBackgroundGrid_);
            glBindBuffer(GL_ARRAY_BUFFER, vboBackgroundGrid_);

            auto MVP = GetCamera()->GetViewPerspectiveMatrix();
            auto screenMatrix = GetCamera()->GetLocalCoordMatrix();

#ifdef VISCOM_USE_SGCT
            auto fw = &GetApplication()->GetFramework();
            auto engine = fw->GetEngine();

            int x, y, xSize, ySize;
            engine->getCurrentWindowPtr()->getCurrentViewportPixelCoords(x, y, xSize, ySize);
            int localScreenHeight = ySize;
            int circleSize = int(demoSyncInfoLocal_.circleData_.z * localScreenHeight);
#endif // VISCOM_USE_SGCT

#ifndef VISCOM_USE_SGCT
            int localScreenHeight = int(GetConfig().virtualScreenSize_.y);
            int circleSize = int(circler_ * localScreenHeight);
#endif // !VISCOM_USE_SGCT

            

            {
                glUseProgram(backgroundProgram_->getProgramId());
                glUniformMatrix4fv(backgroundMVPLoc_, 1, GL_FALSE, glm::value_ptr(MVP));
                glDrawArrays(GL_TRIANGLES, 0, numBackgroundVertices_);
            }

            {
                glDisable(GL_CULL_FACE);
                auto triangleMVP = MVP * triangleModelMatrix_;
                glUseProgram(triangleProgram_->getProgramId());
                glUniformMatrix4fv(triangleMVPLoc_, 1, GL_FALSE, glm::value_ptr(triangleMVP));
                glDrawArrays(GL_TRIANGLES, numBackgroundVertices_, 3);
                glEnable(GL_CULL_FACE);
            }
            
            {
                auto mousepointMVP = screenMatrix * mousepointModelMatrix_;
                glPointSize(30);
                glUseProgram(mousepointProgram_->getProgramId());
                glUniformMatrix4fv(mousepointMVPLoc_, 1, GL_FALSE, glm::value_ptr(mousepointMVP));
                glDrawArrays(GL_POINTS, numBackgroundVertices_+3, 1);
            }

            glBindVertexArray(vaoCircles_);
            glBindBuffer(GL_ARRAY_BUFFER, vboCircles_);
            

            {
                auto demoCirclesMVP = screenMatrix * demoCirclesModelMatrix_;
                glm::vec4 windowSize = glm::vec4(GetConfig().nearPlaneSize_.x,GetConfig().nearPlaneSize_.y, 0.0f, 1.0f);
                glUseProgram(demoCirclesProgram_->getProgramId());
                glPointSize(GLfloat(circleSize));
                glUniformMatrix4fv(demoCirclesMVPLoc_, 1, GL_FALSE, glm::value_ptr(demoCirclesMVP));
                glUniform1i(demoCirclesHitLoc_, hitCircle);
                glUniform1i(demoCirclesSizeLoc_, circleSize);
                glDrawArrays(GL_POINTS, 0, numCirclesVertices_);
            }

            glBindVertexArray(vaoBackgroundGrid_);
            glBindBuffer(GL_ARRAY_BUFFER, vboBackgroundGrid_);

            {
                glUseProgram(teapotProgram_->getProgramId());
                glUniformMatrix4fv(teapotVPLoc_, 1, GL_FALSE, glm::value_ptr(MVP));
                teapotRenderable_->Draw(teapotModelMatrix_);
            }

            {
                glUseProgram(robotProgram_->getProgramId());
                glUniformMatrix4fv(robotVPLoc_, 1, GL_FALSE, glm::value_ptr(MVP));
                robotRenderable_->DrawAnimated(robotModelMatrix_, *robotAnimationState_);
            }

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glUseProgram(0);
        });
    }

#ifdef VISCOM_USE_SGCT
    void ApplicationNodeImplementation::PreSync()
    {
        demoSyncInfoSynced_.setVal(demoSyncInfoLocal_);
    }

    void ApplicationNodeImplementation::UpdateSyncedInfo()
    {
        demoSyncInfoLocal_ = demoSyncInfoSynced_.getVal();
    }

     void ApplicationNodeImplementation::EncodeData()
     {
        sgct::SharedData::instance()->writeObj(&demoSyncInfoSynced_);
     }

     void ApplicationNodeImplementation::DecodeData()
     {
        sgct::SharedData::instance()->readObj(&demoSyncInfoSynced_);
     }
#endif

    bool ApplicationNodeImplementation::KeyboardCallback(int key, int scancode, int action, int mods)
    {

        switch (key)
        {
        case GLFW_KEY_W:
            if (action == GLFW_REPEAT || action == GLFW_PRESS) camPos_ += glm::vec3(0.0, 0.0, -1.001);
            return true;

        case GLFW_KEY_S:
            if (action == GLFW_REPEAT || action == GLFW_PRESS) camPos_ += glm::vec3(0.0, 0.0, 1.001);
            return true;

        case GLFW_KEY_A:
            if (action == GLFW_REPEAT || action == GLFW_PRESS) camPos_ += glm::vec3(-0.001, 0.0, 0.0);
            return true;

        case GLFW_KEY_D:
            if (action == GLFW_REPEAT || action == GLFW_PRESS) camPos_ += glm::vec3(0.001, 0.0, 0.0);
            return true;

        case GLFW_KEY_LEFT_CONTROL:
            if (action == GLFW_REPEAT || action == GLFW_PRESS) camPos_ += glm::vec3(0.0, -0.001, 0.0);
            return true;

        case GLFW_KEY_LEFT_SHIFT:
            if (action == GLFW_REPEAT || action == GLFW_PRESS) camPos_ += glm::vec3(0.0, 0.001, 0.0);
            return true;

        case GLFW_KEY_UP:
            if (action == GLFW_REPEAT || action == GLFW_PRESS) camRot_ += glm::vec3(-0.002, 0.0, 0.0);
            return true;

        case GLFW_KEY_DOWN:
            if (action == GLFW_REPEAT || action == GLFW_PRESS) camRot_ += glm::vec3(0.002, 0.0, 0.0);
            return true;

        case GLFW_KEY_LEFT:
            if (action == GLFW_REPEAT || action == GLFW_PRESS) camRot_ += glm::vec3(0.0, -0.002, 0.0);
            return true;

        case GLFW_KEY_RIGHT:
            if (action == GLFW_REPEAT || action == GLFW_PRESS) camRot_ += glm::vec3(0.0, 0.002, 0.0);
            return true;

        case GLFW_KEY_Q:
            if (action == GLFW_REPEAT || action == GLFW_PRESS) camRot_ += glm::vec3(0.0, 0.0, -0.002);
            return true;

        case GLFW_KEY_E:
            if (action == GLFW_REPEAT || action == GLFW_PRESS) camRot_ += glm::vec3(0.0, 0.0, 0.002);
            return true;
        }
        return false;
    }
    /** Gets the display position of the mouse position callback and saves them into internal values.
    '   @return bool if procedure was done succesfull.
    */
    bool ApplicationNodeImplementation::MousePosCallback(double x, double y) {

        if (posx != x || posy != y) {
            posx = x;
            posy = y;
            posdx = posx * 2 - 1;
            posdy = posy * 2 - 1;
            return true;
        }
        return false;
    }

}
