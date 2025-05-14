#include <stdio.h>
////////Core
#include "Novar/Core/Base.h"
#include "Novar/Core/Application.h"
#include "Novar/Core/Layer.h"
#include "Novar/Core/Timestep.h"

#include "Novar/Core/KeyCodes.h"
#include "Novar/Core/MouseButtonCodes.h"
#include "Novar/Core/Input.h"

#include "Novar/Core/UUID.h"
#include "Novar/Core/LayerStack.h"

#include "Novar/Debug/Instrumentor.h"

#include "Novar/Imgui/ImguiLayer.h"


//Scene
#include "Novar/Scene/Components.h"
#include "Novar/Scene/Scene.h"
#include "Novar/Scene/Entity.h"
#include "Novar/Scene/ScriptableEntity.h"
#include "Novar/Scene/SceneHierachyPanel.h"
#include "Novar/Scene/SceneSerializer.h"

//////////Event
#include "Novar/Events/Event.h"
#include "Novar/Events/KeyEvent.h"
#include "Novar/Events/MouseEvent.h"


///////////Renderer

#include "Novar/Renderer/Renderer.h"
#include "Novar/Renderer/RendererCommand.h"
#include "Novar/Renderer/Renderer2D.h"
#include "Novar/Renderer/Shader.h"
#include "Novar/Renderer/Buffer.h"
#include "Novar/Renderer/FrameBuffer.h"
#include "Novar/Renderer/VertexArray.h"
#include "Novar/Renderer/Texture.h"
#include "Novar/Renderer/OrthographicCamera.h"
#include "Novar/Renderer/OrthographicCameraControl.h"


/////////EntryPoint



