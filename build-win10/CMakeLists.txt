cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)

project(PanicPainter C CXX)

set(PP_FILES
        ../source/main.cpp
        ../source/PPApp.cpp
        ../source/PPApp.h
        ../source/controllers/PPGlobalConfigController.cpp
        ../source/controllers/PPGlobalConfigController.h
        ../source/scenes/gameplay/PPCanvas.cpp
        ../source/scenes/gameplay/PPCanvas.h
        ../source/scenes/gameplay/PPCanvasBlock.cpp
        ../source/scenes/gameplay/PPCanvasBlock.h
        ../source/scenes/gameplay/PPColorStrip.cpp
        ../source/scenes/gameplay/PPColorStrip.h
        ../source/scenes/gameplay/PPColorPalette.cpp
        ../source/scenes/gameplay/PPColorPalette.h
        ../source/scenes/gameplay/PPColorPaletteView.cpp
        ../source/scenes/gameplay/PPColorPaletteView.h
        ../source/scenes/gameplay/PPGameScene.cpp
        ../source/scenes/gameplay/PPGameScene.h
        ../source/scenes/loading/PPLoadingScene.h
        ../source/scenes/loading/PPLoadingScene.cpp
        ../source/controllers/PPGameStateController.h
        ../source/controllers/PPGameStateController.cpp
        ../source/utils/PPTimer.cpp
        ../source/utils/PPTimer.h
        ../source/utils/PPTypeDefs.h
        ../source/utils/PPHeader.h
        ../source/controllers/PPInputController.h
        ../source/controllers/PPInputController.cpp
        ../source/models/PPGameState.h
        ../source/utils/PPAnimation.h
        ../source/utils/PPAnimation.cpp
        ../source/controllers/PPActionController.h
        ../source/controllers/PPActionController.cpp
        ../source/scenes/pause/PPPauseScene.h
        ../source/scenes/pause/PPPauseScene.cpp
        ../source/scenes/gameplay/PPGlobalTimer.h
        ../source/scenes/gameplay/PPGlobalTimer.cpp
        ../source/scenes/gameplay/PPSplashEffect.h
        ../source/scenes/gameplay/PPSplashEffect.cpp
        ../source/scenes/menu/PPMenuScene.h
        ../source/scenes/menu/PPMenuScene.cpp)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"
        OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

set(CMAKE_CONFIGURATION_TYPES
        "Debug"
        "Release"
        CACHE STRING "" FORCE
        )

if(MSVC)
    set(CMAKE_C_FLAGS "")
    set(CMAKE_C_FLAGS_DEBUG "")
    set(CMAKE_C_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
            "Package \"${PACKAGE}\" with version \"${VERSION}\" "
            "for target \"${TARGET}\" is ignored!")
endfunction()

include(CMake/Utils.cmake)
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


################################################################################
# BOX2D
################################################################################

set(BOX2D_FILES
        "../cugl/external/Box2D/Box2D.h"
        "../cugl/external/Box2D/Collision/b2BroadPhase.cpp"
        "../cugl/external/Box2D/Collision/b2BroadPhase.h"
        "../cugl/external/Box2D/Collision/b2CollideCircle.cpp"
        "../cugl/external/Box2D/Collision/b2CollideEdge.cpp"
        "../cugl/external/Box2D/Collision/b2CollidePolygon.cpp"
        "../cugl/external/Box2D/Collision/b2Collision.cpp"
        "../cugl/external/Box2D/Collision/b2Collision.h"
        "../cugl/external/Box2D/Collision/b2Distance.cpp"
        "../cugl/external/Box2D/Collision/b2Distance.h"
        "../cugl/external/Box2D/Collision/b2DynamicTree.cpp"
        "../cugl/external/Box2D/Collision/b2DynamicTree.h"
        "../cugl/external/Box2D/Collision/b2TimeOfImpact.cpp"
        "../cugl/external/Box2D/Collision/b2TimeOfImpact.h"
        "../cugl/external/Box2D/Collision/Shapes/b2ChainShape.cpp"
        "../cugl/external/Box2D/Collision/Shapes/b2ChainShape.h"
        "../cugl/external/Box2D/Collision/Shapes/b2CircleShape.cpp"
        "../cugl/external/Box2D/Collision/Shapes/b2CircleShape.h"
        "../cugl/external/Box2D/Collision/Shapes/b2EdgeShape.cpp"
        "../cugl/external/Box2D/Collision/Shapes/b2EdgeShape.h"
        "../cugl/external/Box2D/Collision/Shapes/b2PolygonShape.cpp"
        "../cugl/external/Box2D/Collision/Shapes/b2PolygonShape.h"
        "../cugl/external/Box2D/Collision/Shapes/b2Shape.h"
        "../cugl/external/Box2D/Common/b2BlockAllocator.cpp"
        "../cugl/external/Box2D/Common/b2BlockAllocator.h"
        "../cugl/external/Box2D/Common/b2Draw.cpp"
        "../cugl/external/Box2D/Common/b2Draw.h"
        "../cugl/external/Box2D/Common/b2GrowableStack.h"
        "../cugl/external/Box2D/Common/b2Math.cpp"
        "../cugl/external/Box2D/Common/b2Math.h"
        "../cugl/external/Box2D/Common/b2Settings.cpp"
        "../cugl/external/Box2D/Common/b2Settings.h"
        "../cugl/external/Box2D/Common/b2StackAllocator.cpp"
        "../cugl/external/Box2D/Common/b2StackAllocator.h"
        "../cugl/external/Box2D/Common/b2Timer.cpp"
        "../cugl/external/Box2D/Common/b2Timer.h"
        "../cugl/external/Box2D/Dynamics/b2Body.cpp"
        "../cugl/external/Box2D/Dynamics/b2Body.h"
        "../cugl/external/Box2D/Dynamics/b2ContactManager.cpp"
        "../cugl/external/Box2D/Dynamics/b2ContactManager.h"
        "../cugl/external/Box2D/Dynamics/b2Fixture.cpp"
        "../cugl/external/Box2D/Dynamics/b2Fixture.h"
        "../cugl/external/Box2D/Dynamics/b2Island.cpp"
        "../cugl/external/Box2D/Dynamics/b2Island.h"
        "../cugl/external/Box2D/Dynamics/b2TimeStep.h"
        "../cugl/external/Box2D/Dynamics/b2World.cpp"
        "../cugl/external/Box2D/Dynamics/b2World.h"
        "../cugl/external/Box2D/Dynamics/b2WorldCallbacks.cpp"
        "../cugl/external/Box2D/Dynamics/b2WorldCallbacks.h"
        "../cugl/external/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp"
        "../cugl/external/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h"
        "../cugl/external/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp"
        "../cugl/external/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h"
        "../cugl/external/Box2D/Dynamics/Contacts/b2CircleContact.cpp"
        "../cugl/external/Box2D/Dynamics/Contacts/b2CircleContact.h"
        "../cugl/external/Box2D/Dynamics/Contacts/b2Contact.cpp"
        "../cugl/external/Box2D/Dynamics/Contacts/b2Contact.h"
        "../cugl/external/Box2D/Dynamics/Contacts/b2ContactSolver.cpp"
        "../cugl/external/Box2D/Dynamics/Contacts/b2ContactSolver.h"
        "../cugl/external/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp"
        "../cugl/external/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h"
        "../cugl/external/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp"
        "../cugl/external/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h"
        "../cugl/external/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp"
        "../cugl/external/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h"
        "../cugl/external/Box2D/Dynamics/Contacts/b2PolygonContact.cpp"
        "../cugl/external/Box2D/Dynamics/Contacts/b2PolygonContact.h"
        "../cugl/external/Box2D/Dynamics/Joints/b2DistanceJoint.cpp"
        "../cugl/external/Box2D/Dynamics/Joints/b2DistanceJoint.h"
        "../cugl/external/Box2D/Dynamics/Joints/b2FrictionJoint.cpp"
        "../cugl/external/Box2D/Dynamics/Joints/b2FrictionJoint.h"
        "../cugl/external/Box2D/Dynamics/Joints/b2GearJoint.cpp"
        "../cugl/external/Box2D/Dynamics/Joints/b2GearJoint.h"
        "../cugl/external/Box2D/Dynamics/Joints/b2Joint.cpp"
        "../cugl/external/Box2D/Dynamics/Joints/b2Joint.h"
        "../cugl/external/Box2D/Dynamics/Joints/b2MotorJoint.cpp"
        "../cugl/external/Box2D/Dynamics/Joints/b2MotorJoint.h"
        "../cugl/external/Box2D/Dynamics/Joints/b2MouseJoint.cpp"
        "../cugl/external/Box2D/Dynamics/Joints/b2MouseJoint.h"
        "../cugl/external/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp"
        "../cugl/external/Box2D/Dynamics/Joints/b2PrismaticJoint.h"
        "../cugl/external/Box2D/Dynamics/Joints/b2PulleyJoint.cpp"
        "../cugl/external/Box2D/Dynamics/Joints/b2PulleyJoint.h"
        "../cugl/external/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp"
        "../cugl/external/Box2D/Dynamics/Joints/b2RevoluteJoint.h"
        "../cugl/external/Box2D/Dynamics/Joints/b2RopeJoint.cpp"
        "../cugl/external/Box2D/Dynamics/Joints/b2RopeJoint.h"
        "../cugl/external/Box2D/Dynamics/Joints/b2WeldJoint.cpp"
        "../cugl/external/Box2D/Dynamics/Joints/b2WeldJoint.h"
        "../cugl/external/Box2D/Dynamics/Joints/b2WheelJoint.cpp"
        "../cugl/external/Box2D/Dynamics/Joints/b2WheelJoint.h")
add_library(Box2D STATIC ${BOX2D_FILES})
use_props(Box2D "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Box2D)
set_target_properties(Box2D PROPERTIES
        VS_GLOBAL_KEYWORD "Win32Proj"
        )
set_target_properties(Box2D PROPERTIES
        TARGET_NAME_DEBUG   "Box2D"
        TARGET_NAME_RELEASE "Box2D"
        )
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(Box2D PROPERTIES
            OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/x64/$<CONFIG>/"
            OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/x64/$<CONFIG>/"
            )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(Box2D PROPERTIES
            OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/x86/$<CONFIG>/"
            OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/x86/$<CONFIG>/"
            )
endif()
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET Box2D PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
        MultiThreadedDebugDLL
        >
        $<$<CONFIG:Release>:
        MultiThreadedDLL
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
        )
set_target_properties(Box2D PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})
target_include_directories(Box2D PUBLIC
        "../cugl/external"
        )
target_compile_definitions(Box2D PRIVATE
        "$<$<CONFIG:Release>:"
        "NDEBUG"
        ">"
        "_CRT_SECURE_NO_WARNINGS"
        )
if(MSVC)
    target_compile_options(Box2D PRIVATE
            $<$<CONFIG:Debug>:
            /Od;
            /RTC1
            >
            $<$<CONFIG:Release>:
            /Ox;
            /GF;
            /fp:fast;
            /GR-
            >
            /Gy;
            /W4;
            /WX;
            /Zi;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
            )
    target_link_options(Box2D PRIVATE
            $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
            >
            /MACHINE:X64;
            /SUBSYSTEM:WINDOWS
            )
endif()


################################################################################
# CUGL
################################################################################
set(CUGL_FILES
        "../cugl/lib/assets/CUFontLoader.cpp"
        "../cugl/lib/assets/CUJsonLoader.cpp"
        "../cugl/lib/assets/CUJsonValue.cpp"
        "../cugl/lib/assets/CUScene2Loader.cpp"
        "../cugl/lib/assets/CUSoundLoader.cpp"
        "../cugl/lib/assets/CUTextureLoader.cpp"
        "../cugl/lib/assets/CUWidgetLoader.cpp"
        "../cugl/lib/audio/codecs/CUAudioDecoder.cpp"
        "../cugl/lib/audio/codecs/CUFLACDecoder.cpp"
        "../cugl/lib/audio/codecs/CUMP3Decoder.cpp"
        "../cugl/lib/audio/codecs/CUOGGDecoder.cpp"
        "../cugl/lib/audio/codecs/CUWAVDecoder.cpp"
        "../cugl/lib/audio/CUAudioDevices.cpp"
        "../cugl/lib/audio/CUAudioEngine.cpp"
        "../cugl/lib/audio/CUAudioQueue.cpp"
        "../cugl/lib/audio/CUAudioSample.cpp"
        "../cugl/lib/audio/CUAudioWaveform.cpp"
        "../cugl/lib/audio/CUSound.cpp"
        "../cugl/lib/audio/graph/CUAudioFader.cpp"
        "../cugl/lib/audio/graph/CUAudioInput.cpp"
        "../cugl/lib/audio/graph/CUAudioMixer.cpp"
        "../cugl/lib/audio/graph/CUAudioNode.cpp"
        "../cugl/lib/audio/graph/CUAudioOutput.cpp"
        "../cugl/lib/audio/graph/CUAudioPanner.cpp"
        "../cugl/lib/audio/graph/CUAudioPlayer.cpp"
        "../cugl/lib/audio/graph/CUAudioResampler.cpp"
        "../cugl/lib/audio/graph/CUAudioScheduler.cpp"
        "../cugl/lib/audio/graph/CUAudioSpinner.cpp"
        "../cugl/lib/audio/graph/CUAudioSynchronizer.cpp"
        "../cugl/lib/base/CUApplication.cpp"
        "../cugl/lib/base/CUDisplay.cpp"
        "../cugl/lib/base/platform/CUDisplay-impl.h"
        "../cugl/lib/base/platform/CUDisplay-SDL.cpp"
        "../cugl/lib/input/CUAccelerometer.cpp"
        "../cugl/lib/input/CUInput.cpp"
        "../cugl/lib/input/CUKeyboard.cpp"
        "../cugl/lib/input/CUMouse.cpp"
        "../cugl/lib/input/CUTextInput.cpp"
        "../cugl/lib/input/CUTouchscreen.cpp"
        "../cugl/lib/input/gestures/CUPanInput.cpp"
        "../cugl/lib/input/gestures/CUPinchInput.cpp"
        "../cugl/lib/input/gestures/CURotationInput.cpp"
        "../cugl/lib/io/CUBinaryReader.cpp"
        "../cugl/lib/io/CUBinaryWriter.cpp"
        "../cugl/lib/io/CUJsonReader.cpp"
        "../cugl/lib/io/CUJsonWriter.cpp"
        "../cugl/lib/io/CUTextReader.cpp"
        "../cugl/lib/io/CUTextWriter.cpp"
        "../cugl/lib/math/cuACC128.inl"
        "../cugl/lib/math/CUAffine2.cpp"
        "../cugl/lib/math/CUColor4.cpp"
        "../cugl/lib/math/CUEasingBezier.cpp"
        "../cugl/lib/math/CUEasingFunction.cpp"
        "../cugl/lib/math/CUFrustum.cpp"
        "../cugl/lib/math/CUGeometry.cpp"
        "../cugl/lib/math/CUMat4.cpp"
        "../cugl/lib/math/CUMathBase.cpp"
        "../cugl/lib/math/CUPlane.cpp"
        "../cugl/lib/math/CUPoly2.cpp"
        "../cugl/lib/math/CUPolynomial.cpp"
        "../cugl/lib/math/CUQuaternion.cpp"
        "../cugl/lib/math/CURay.cpp"
        "../cugl/lib/math/CURect.cpp"
        "../cugl/lib/math/CUSize.cpp"
        "../cugl/lib/math/CUSpline2.cpp"
        "../cugl/lib/math/CUVec2.cpp"
        "../cugl/lib/math/CUVec3.cpp"
        "../cugl/lib/math/CUVec4.cpp"
        "../cugl/lib/math/dsp/CUBiquadIIR.cpp"
        "../cugl/lib/math/dsp/cuDSP128.inl"
        "../cugl/lib/math/dsp/CUDSPMath.cpp"
        "../cugl/lib/math/dsp/CUFIRFilter.cpp"
        "../cugl/lib/math/dsp/CUIIRFilter.cpp"
        "../cugl/lib/math/dsp/CUOnePoleIIR.cpp"
        "../cugl/lib/math/dsp/CUOneZeroFIR.cpp"
        "../cugl/lib/math/dsp/CUPoleZeroIIR.cpp"
        "../cugl/lib/math/dsp/CUTwoPoleIIR.cpp"
        "../cugl/lib/math/dsp/CUTwoZeroFIR.cpp"
        "../cugl/lib/math/polygon/CUComplexExtruder.cpp"
        "../cugl/lib/math/polygon/CUComplexTriangulator.cpp"
        "../cugl/lib/math/polygon/CUPathSmoother.cpp"
        "../cugl/lib/math/polygon/CUPolyFactory.cpp"
        "../cugl/lib/math/polygon/CUPolySplineFactory.cpp"
        "../cugl/lib/math/polygon/CUSimpleExtruder.cpp"
        "../cugl/lib/math/polygon/CUSimpleTriangulator.cpp"
        "../cugl/lib/physics2/CUBoxObstacle.cpp"
        "../cugl/lib/physics2/CUCapsuleObstacle.cpp"
        "../cugl/lib/physics2/CUComplexObstacle.cpp"
        "../cugl/lib/physics2/CUObstacle.cpp"
        "../cugl/lib/physics2/CUObstacleSelector.cpp"
        "../cugl/lib/physics2/CUObstacleWorld.cpp"
        "../cugl/lib/physics2/CUPolygonObstacle.cpp"
        "../cugl/lib/physics2/CUSimpleObstacle.cpp"
        "../cugl/lib/physics2/CUWheelObstacle.cpp"
        "../cugl/lib/render/CUCamera.cpp"
        "../cugl/lib/render/CUFont.cpp"
        "../cugl/lib/render/CUGradient.cpp"
        "../cugl/lib/render/CUOrthographicCamera.cpp"
        "../cugl/lib/render/CUPerspectiveCamera.cpp"
        "../cugl/lib/render/CURenderTarget.cpp"
        "../cugl/lib/render/CUScissor.cpp"
        "../cugl/lib/render/CUShader.cpp"
        "../cugl/lib/render/CUSpriteBatch.cpp"
        "../cugl/lib/render/CUTexture.cpp"
        "../cugl/lib/render/CUUniformBuffer.cpp"
        "../cugl/lib/render/CUVertexBuffer.cpp"
        "../cugl/lib/render/shaders/ColorTexture.frag"
        "../cugl/lib/render/shaders/ColorTexture.vert"
        "../cugl/lib/render/shaders/SpriteShader.frag"
        "../cugl/lib/render/shaders/SpriteShader.vert"
        "../cugl/lib/scene2/CUScene2.cpp"
        "../cugl/lib/scene2/CUScene2Texture.cpp"
        "../cugl/lib/scene2/graph/CUAnimationNode.cpp"
        "../cugl/lib/scene2/graph/CUPathNode.cpp"
        "../cugl/lib/scene2/graph/CUPolygonNode.cpp"
        "../cugl/lib/scene2/graph/CUSceneNode.cpp"
        "../cugl/lib/scene2/graph/CUTexturedNode.cpp"
        "../cugl/lib/scene2/graph/CUWireNode.cpp"
        "../cugl/lib/scene2/layout/CUAnchoredLayout.cpp"
        "../cugl/lib/scene2/layout/CUFloatLayout.cpp"
        "../cugl/lib/scene2/layout/CUGridLayout.cpp"
        "../cugl/lib/scene2/layout/CULayout.cpp"
        "../cugl/lib/scene2/ui/CUButton.cpp"
        "../cugl/lib/scene2/ui/CULabel.cpp"
        "../cugl/lib/scene2/ui/CUNinePatch.cpp"
        "../cugl/lib/scene2/ui/CUProgressBar.cpp"
        "../cugl/lib/scene2/ui/CUSlider.cpp"
        "../cugl/lib/scene2/ui/CUTextField.cpp"
        "../cugl/lib/util/CUDebug.cpp"
        "../cugl/lib/util/CUFiletools.cpp"
        "../cugl/lib/util/CUStrings.cpp"
        "../cugl/lib/util/CUThreadPool.cpp"
        "../cugl/include/cugl/cugl.h"
        "../cugl/include/cugl/assets/cu_assets.h"
        "../cugl/include/cugl/assets/CUAsset.h"
        "../cugl/include/cugl/assets/CUAssetManager.h"
        "../cugl/include/cugl/assets/CUFontLoader.h"
        "../cugl/include/cugl/assets/CUGenericLoader.h"
        "../cugl/include/cugl/assets/CUJsonLoader.h"
        "../cugl/include/cugl/assets/CUJsonValue.h"
        "../cugl/include/cugl/assets/CULoader.h"
        "../cugl/include/cugl/assets/CUScene2Loader.h"
        "../cugl/include/cugl/assets/CUSoundLoader.h"
        "../cugl/include/cugl/assets/CUTextureLoader.h"
        "../cugl/include/cugl/assets/CUWidgetLoader.h"
        "../cugl/include/cugl/assets/CUWidgetValue.h"
        "../cugl/include/cugl/audio/cu_audio.h"
        "../cugl/include/cugl/audio/CUAudioDevices.h"
        "../cugl/include/cugl/audio/CUAudioEngine.h"
        "../cugl/include/cugl/audio/CUAudioQueue.h"
        "../cugl/include/cugl/audio/CUAudioSample.h"
        "../cugl/include/cugl/audio/CUAudioWaveform.h"
        "../cugl/include/cugl/audio/CUSound.h"
        "../cugl/include/cugl/audio/codecs/cu_codecs.h"
        "../cugl/include/cugl/audio/codecs/CUAudioDecoder.h"
        "../cugl/include/cugl/audio/codecs/CUFLACDecoder.h"
        "../cugl/include/cugl/audio/codecs/CUMP3Decoder.h"
        "../cugl/include/cugl/audio/codecs/CUOGGDecoder.h"
        "../cugl/include/cugl/audio/codecs/CUWAVDecoder.h"
        "../cugl/include/cugl/audio/graph/cu_audio_graph.h"
        "../cugl/include/cugl/audio/graph/CUAudioFader.h"
        "../cugl/include/cugl/audio/graph/CUAudioInput.h"
        "../cugl/include/cugl/audio/graph/CUAudioMixer.h"
        "../cugl/include/cugl/audio/graph/CUAudioNode.h"
        "../cugl/include/cugl/audio/graph/CUAudioOutput.h"
        "../cugl/include/cugl/audio/graph/CUAudioPanner.h"
        "../cugl/include/cugl/audio/graph/CUAudioPlayer.h"
        "../cugl/include/cugl/audio/graph/CUAudioResampler.h"
        "../cugl/include/cugl/audio/graph/CUAudioScheduler.h"
        "../cugl/include/cugl/audio/graph/CUAudioSpinner.h"
        "../cugl/include/cugl/audio/graph/CUAudioSynchronizer.h"
        "../cugl/include/cugl/base/cu_platform.h"
        "../cugl/include/cugl/base/CUApplication.h"
        "../cugl/include/cugl/base/CUBase.h"
        "../cugl/include/cugl/base/CUDisplay.h"
        "../cugl/include/cugl/base/CUEndian.h"
        "../cugl/include/cugl/base/platform/CUDisplay-impl.h"
        "../cugl/external/cJSON/cJSON.c"
        "../cugl/include/clipper/clipper.hpp"
        "../cugl/include/poly2tri/poly2tri.h"
        "../cugl/include/poly2tri/common/shapes.h"
        "../cugl/include/poly2tri/common/utils.h"
        "../cugl/include/poly2tri/sweep/advancing_front.h"
        "../cugl/include/poly2tri/sweep/cdt.h"
        "../cugl/include/poly2tri/sweep/sweep.h"
        "../cugl/include/poly2tri/sweep/sweep_context.h"
        "../cugl/include/cugl/input/cu_input.h"
        "../cugl/include/cugl/input/CUAccelerometer.h"
        "../cugl/include/cugl/input/CUInput.h"
        "../cugl/include/cugl/input/CUKeyboard.h"
        "../cugl/include/cugl/input/CUMouse.h"
        "../cugl/include/cugl/input/CUTextInput.h"
        "../cugl/include/cugl/input/CUTouchscreen.h"
        "../cugl/include/cugl/input/gestures/cu_gesture.h"
        "../cugl/include/cugl/input/gestures/CUPanInput.h"
        "../cugl/include/cugl/input/gestures/CUPinchInput.h"
        "../cugl/include/cugl/input/gestures/CURotationInput.h"
        "../cugl/include/cugl/io/cu_io.h"
        "../cugl/include/cugl/io/CUBinaryReader.h"
        "../cugl/include/cugl/io/CUBinaryWriter.h"
        "../cugl/include/cugl/io/CUJsonReader.h"
        "../cugl/include/cugl/io/CUJsonWriter.h"
        "../cugl/include/cugl/io/CUTextReader.h"
        "../cugl/include/cugl/io/CUTextWriter.h"
        "../cugl/include/cugl/math/cu_math.h"
        "../cugl/include/cugl/math/CUAffine2.h"
        "../cugl/include/cugl/math/CUColor4.h"
        "../cugl/include/cugl/math/CUEasingBezier.h"
        "../cugl/include/cugl/math/CUEasingFunction.h"
        "../cugl/include/cugl/math/CUFrustum.h"
        "../cugl/include/cugl/math/CUGeometry.h"
        "../cugl/include/cugl/math/CUMat4.h"
        "../cugl/include/cugl/math/CUMathBase.h"
        "../cugl/include/cugl/math/CUPlane.h"
        "../cugl/include/cugl/math/CUPoly2.h"
        "../cugl/include/cugl/math/CUPolynomial.h"
        "../cugl/include/cugl/math/CUQuaternion.h"
        "../cugl/include/cugl/math/CURay.h"
        "../cugl/include/cugl/math/CURect.h"
        "../cugl/include/cugl/math/CUSize.h"
        "../cugl/include/cugl/math/CUSpline2.h"
        "../cugl/include/cugl/math/CUVec2.h"
        "../cugl/include/cugl/math/CUVec3.h"
        "../cugl/include/cugl/math/CUVec4.h"
        "../cugl/include/cugl/math/dsp/cu_dsp.h"
        "../cugl/include/cugl/math/dsp/CUBiquadIIR.h"
        "../cugl/include/cugl/math/dsp/CUDSPMath.h"
        "../cugl/include/cugl/math/dsp/CUFIRFilter.h"
        "../cugl/include/cugl/math/dsp/CUIIRFilter.h"
        "../cugl/include/cugl/math/dsp/CUOnePoleIIR.h"
        "../cugl/include/cugl/math/dsp/CUOneZeroFIR.h"
        "../cugl/include/cugl/math/dsp/CUPoleZeroIIR.h"
        "../cugl/include/cugl/math/dsp/CUTwoPoleIIR.h"
        "../cugl/include/cugl/math/dsp/CUTwoZeroFIR.h"
        "../cugl/include/cugl/math/polygon/cu_polygon.h"
        "../cugl/include/cugl/math/polygon/CUComplexExtruder.h"
        "../cugl/include/cugl/math/polygon/CUComplexTriangulator.h"
        "../cugl/include/cugl/math/polygon/CUPathSmoother.h"
        "../cugl/include/cugl/math/polygon/CUPolyEnums.h"
        "../cugl/include/cugl/math/polygon/CUPolyFactory.h"
        "../cugl/include/cugl/math/polygon/CUPolySplineFactory.h"
        "../cugl/include/cugl/math/polygon/CUSimpleExtruder.h"
        "../cugl/include/cugl/math/polygon/CUSimpleTriangulator.h"
        "../cugl/include/cugl/physics2/cu_physics2.h"
        "../cugl/include/cugl/physics2/CUBoxObstacle.h"
        "../cugl/include/cugl/physics2/CUCapsuleObstacle.h"
        "../cugl/include/cugl/physics2/CUComplexObstacle.h"
        "../cugl/include/cugl/physics2/CUObstacle.h"
        "../cugl/include/cugl/physics2/CUObstacleSelector.h"
        "../cugl/include/cugl/physics2/CUObstacleWorld.h"
        "../cugl/include/cugl/physics2/CUPolygonObstacle.h"
        "../cugl/include/cugl/physics2/CUSimpleObstacle.h"
        "../cugl/include/cugl/physics2/CUWheelObstacle.h"
        "../cugl/include/cugl/render/cu_render.h"
        "../cugl/include/cugl/render/CUCamera.h"
        "../cugl/include/cugl/render/CUFont.h"
        "../cugl/include/cugl/render/CUGradient.h"
        "../cugl/include/cugl/render/CUMesh.h"
        "../cugl/include/cugl/render/CUOrthographicCamera.h"
        "../cugl/include/cugl/render/CUPerspectiveCamera.h"
        "../cugl/include/cugl/render/CURenderTarget.h"
        "../cugl/include/cugl/render/CUScissor.h"
        "../cugl/include/cugl/render/CUShader.h"
        "../cugl/include/cugl/render/CUSpriteBatch.h"
        "../cugl/include/cugl/render/CUSpriteVertex.h"
        "../cugl/include/cugl/render/CUTexture.h"
        "../cugl/include/cugl/render/CUUniformBuffer.h"
        "../cugl/include/cugl/render/CUVertexBuffer.h"
        "../cugl/include/cugl/scene2/cu_scene2.h"
        "../cugl/include/cugl/scene2/CUScene2.h"
        "../cugl/include/cugl/scene2/CUScene2Texture.h"
        "../cugl/include/cugl/scene2/graph/CUAnimationNode.h"
        "../cugl/include/cugl/scene2/graph/CUPathNode.h"
        "../cugl/include/cugl/scene2/graph/CUPolygonNode.h"
        "../cugl/include/cugl/scene2/graph/CUSceneNode.h"
        "../cugl/include/cugl/scene2/graph/CUTexturedNode.h"
        "../cugl/include/cugl/scene2/graph/CUWireNode.h"
        "../cugl/include/cugl/scene2/layout/cu_layout.h"
        "../cugl/include/cugl/scene2/layout/CUAnchoredLayout.h"
        "../cugl/include/cugl/scene2/layout/CUFloatLayout.h"
        "../cugl/include/cugl/scene2/layout/CUGridLayout.h"
        "../cugl/include/cugl/scene2/layout/CULayout.h"
        "../cugl/include/cugl/scene2/ui/CUButton.h"
        "../cugl/include/cugl/scene2/ui/CULabel.h"
        "../cugl/include/cugl/scene2/ui/CUNinePatch.h"
        "../cugl/include/cugl/scene2/ui/CUProgressBar.h"
        "../cugl/include/cugl/scene2/ui/CUSlider.h"
        "../cugl/include/cugl/scene2/ui/CUTextField.h"
        "../cugl/include/cugl/util/cu_util.h"
        "../cugl/include/cugl/util/CUAligned.h"
        "../cugl/include/cugl/util/CUDebug.h"
        "../cugl/include/cugl/util/CUFiletools.h"
        "../cugl/include/cugl/util/CUFreeList.h"
        "../cugl/include/cugl/util/CUGreedyFreeList.h"
        "../cugl/include/cugl/util/CUStrings.h"
        "../cugl/include/cugl/util/CUThreadPool.h"
        "../cugl/include/cugl/util/CUTimestamp.h"
        "../cugl/lib/assets/CUAssetManager.cpp"
        "../cugl/external/clipper/clipper.cpp"
        "../cugl/external/poly2tri/common/shapes.cc"
        "../cugl/external/poly2tri/sweep/advancing_front.cc"
        "../cugl/external/poly2tri/sweep/cdt.cc"
        "../cugl/external/poly2tri/sweep/sweep.cc"
        "../cugl/external/poly2tri/sweep/sweep_context.cc"
        )
add_library(cugl STATIC ${CUGL_FILES})
use_props(cugl "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set_target_properties(cugl PROPERTIES
        VS_GLOBAL_KEYWORD "Win32Proj"
        )
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(cugl PROPERTIES
            OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/x64/$<CONFIG>/"
            OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/x64/$<CONFIG>/"
            )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(cugl PROPERTIES
            OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/x86/$<CONFIG>/"
            OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/x86/$<CONFIG>/"
            )
endif()
set_target_properties(cugl PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
        )
target_include_directories(cugl PUBLIC
        "../cugl/include"
        "../cugl/build-win10/include"
        )
target_compile_definitions(cugl PRIVATE
        "$<$<CONFIG:Debug>:"
        "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
        "NDEBUG"
        ">"
        "WIN64;"
        "_WINDOWS;"
        "_LIB;"
        "_CRT_SECURE_NO_WARNINGS;"
        "UNICODE;"
        "_UNICODE"
        )
if(MSVC)
    target_compile_options(cugl PRIVATE
            $<$<CONFIG:Debug>:
            /Od;
            /Zi
            >
            $<$<CONFIG:Release>:
            /O2;
            /Oi;
            /Gy;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
            >
            /W3;
            /wd4068;
            /wd4018;
            /wd4800;
            /wd4267;
            /wd26812;
            /wd26495;
            /wd4244;
            /wd4305;
            /IGNORE:4006,4221;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /GS-;
            /Y-
            )
    target_link_options(cugl PRIVATE
            $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
            >
            /SUBSYSTEM:WINDOWS
            )
endif()
target_link_libraries(cugl PUBLIC Box2D)
set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "SDL2;"
        "SDL2main;"
        "SDL2_ttf;"
        "SDL2_image;"
        "SDL2_codec"
        )
target_link_libraries(cugl PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(cugl PUBLIC
            "$<$<CONFIG:Debug>:"
            "../cugl/build-win10/lib/Debug/x64"
            ">"
            "$<$<CONFIG:Release>:"
            "../cugl/build-win10/lib/Release/x64"
            ">"
            )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(cugl PUBLIC
            "$<$<CONFIG:Debug>:"
            "../cugl/build-win10/lib/Debug/x86"
            ">"
            "$<$<CONFIG:Release>:"
            "../cugl/build-win10/lib/Release/x86"
            ">"
            )
endif()

################################################################################
# PANIC PAINTER
################################################################################

add_executable(PanicPainter ${PP_FILES})

use_props(PanicPainter "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE PanicPainter)

set_target_properties(PanicPainter PROPERTIES
        VS_GLOBAL_KEYWORD "Win32Proj"
        )
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(PanicPainter PROPERTIES
            OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/x64/$<CONFIG>/"
            OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/x64/$<CONFIG>/"
            )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(PanicPainter PROPERTIES
            OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/x86/$<CONFIG>/"
            OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/x86/$<CONFIG>/"
            )
endif()
set_target_properties(PanicPainter PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
        )
target_include_directories(PanicPainter PUBLIC
        "../cugl/build-win10/include"
        "../cugl/build-win10/source"
        "../cugl/build-win10/cugl/include"
        "../source"
        )
target_compile_definitions(PanicPainter PRIVATE
        "$<$<CONFIG:Debug>:"
        "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
        "NDEBUG"
        ">"
        "_WINDOWS"
        )

if(MSVC)
    target_compile_options(PanicPainter PRIVATE
            $<$<CONFIG:Debug>:
            /Od;
            /Zi;
            /wd4305;
            /GS-
            >
            $<$<CONFIG:Release>:
            /O2;
            /Oi;
            /Gy;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            /wd4305
            >
            /W3;
            /wd4068;
            /wd4018;
            /wd4244;
            /wd4267;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
            )
    target_link_options(PanicPainter PRIVATE
            $<$<CONFIG:Debug>:
            /INCREMENTAL
            >
            $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:WINDOWS;
            /IGNORE:4099
            )
endif()

add_dependencies(PanicPainter
        cugl
        )

target_link_libraries(PanicPainter PRIVATE
        cugl
        )

set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "cugl"
        )
target_link_libraries(PanicPainter PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(PanicPainter PRIVATE
        "${OUTPUT_DIRECTORY}"
        )
