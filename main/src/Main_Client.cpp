#define KROSS_MAIN 1
#include "Kross.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Canvas : public Kross::Layer
{
	Kross::Scope<Kross::VertexArray> sqrVA;

	Kross::Scope<Kross::Textures::T2D> texture1, texture2;


	glm::mat4 scale;

public:
	glm::vec4 redColor = glm::vec4(0.8f, 0.2f, 0.2f, 1.0f);
	glm::vec4 blueColor = glm::vec4(0.2f, 0.2f, 0.8f, 1.0f);

	Kross::Camera::Ortho2DCtrl m_Camera;

	Canvas()
		: Layer("Canvas"),
		m_Camera(ar, true),
		scale(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)))
	{
		float sqrVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,	1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f
		};

		uint32_t sqrIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		sqrVA = Kross::VertexArray::CreateScope();

		Kross::Ref <Kross::Buffer::Vertex> sqrVB;
		sqrVB = Kross::Buffer::Vertex::Create(sqrVertices, sizeof(sqrVertices));
		sqrVB->SetLayout({
			{ Kross::Buffer::ShaderDataType::Float3, "a_Position" },
			{ Kross::Buffer::ShaderDataType::Float2, "a_TexCoord" }
			});
		sqrVA->AddVertex(sqrVB);

		Kross::Ref<Kross::Buffer::Index>		sqrIB;
		sqrIB = Kross::Buffer::Index::Create(sqrIndices, sizeof(sqrIndices) / sizeof(uint32_t));
		sqrVA->SetIndex(sqrIB);


		Kross::Renderer::GetShaderLibrary()->Load("grid", "assets/shaders/FlatColor.glsl");

		Kross::Renderer::GetShaderLibrary()->Load("Textures", "assets/shaders/Textures.glsl");

		texture1 = Kross::Textures::T2D::CreateScope("assets/textures/Checkerboard.png");
		texture2 = Kross::Textures::T2D::CreateScope("assets/textures/ChernoLogo.png");
	}
	
	~Canvas(){}
	
	virtual void OnUpdate(Kross::Timestep ts) override
	{
		m_Camera.OnUpdate(ts);

		Kross::Renderer::Command::SetClear({ 0.1f, 0.1f, 0.1f, 1.0f });
		Kross::Renderer::Command::Clear();

		Kross::Renderer::Scene::Begin(*m_Camera.GetCamera());

		Kross::Ref<Kross::Shader>& sqrShader = Kross::Renderer::GetShaderLibrary()->Get("grid");

		sqrShader->Bind();


		for (int x = 0; x < 20; x++)
		{
			for (int y = 0; y < 20; y++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if (x % 2 == 0)
					sqrShader->SetFloat4("u_Color", redColor);
				else
					sqrShader->SetFloat4("u_Color", blueColor);
				Kross::Renderer::Submit(sqrShader, sqrVA, transform);
			}
		}

		Kross::Ref<Kross::Shader>& texShader = Kross::Renderer::GetShaderLibrary()->Get("Textures");

		texShader->Bind();

		texture1->Bind();
		Kross::Renderer::Submit(texShader, sqrVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		texture2->Bind();
		Kross::Renderer::Submit(texShader, sqrVA, glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, -0.25f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
				
		Kross::Renderer::Scene::End();
	}
	
	virtual void OnEvent(Kross::Event& e) override
	{
		m_Camera.OnEvent(e);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UI : public Kross::Layer
{
	Kross::Ref<Canvas> canvas;
	bool* demo_window;
public:
	UI(Kross::Ref<Canvas> c)
		: Layer("User Interface"),
		canvas(c),
		demo_window(nullptr)
	{}

	~UI(){}

	virtual void OnImGuiRender(Kross::Timestep ts) override
	{
		{
			float fps = 1/ts.GetSeconds();
			Kross::Camera::Camera& camera = *canvas->m_Camera.GetCamera();
			glm::vec3 cameraPos = camera.GetPosition();
			ImGuiWindowFlags CameraProperties_Flags = 0;
			CameraProperties_Flags |= ImGuiWindowFlags_NoMove;
			CameraProperties_Flags |= ImGuiWindowFlags_NoCollapse;
			CameraProperties_Flags |= ImGuiWindowFlags_NoDocking;
			CameraProperties_Flags |= ImGuiWindowFlags_AlwaysAutoResize;
			ImGui::Begin("Camera Properties" , nullptr, CameraProperties_Flags);
			//ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, "avg 0.0", -1.0f, 1.0f, ImVec2(0, 80));
			ImGui::Text("FPS: %f", fps);
			ImGui::Text(std::string("Type : " + camera.GetCameraName()).c_str());
			ImGui::Text("Position : ");
			ImGui::Text("	x = %f", cameraPos.x);
			ImGui::Text("	y = %f", cameraPos.y);
			ImGui::Text("	z = %f", cameraPos.z);
			ImGui::Text("Rotation : %f", camera.GetRotation());
			static bool demo_window = false;
			ImGui::Checkbox("ImGui Demo Window", &demo_window);
			if(demo_window)
				ImGui::ShowDemoWindow(&demo_window);
			ImGui::End();
		}
		{
			ImGui::Begin("colors", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::ColorEdit4("red", glm::value_ptr(canvas->redColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions);
			ImGui::SameLine(); ImGui::ColorEdit4("blue", glm::value_ptr(canvas->blueColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions);
			ImGui::End();
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Sandbox2D.h"

#define S2D 1

class Sandbox : public Kross::Application
{
#if !(S2D)
	Kross::Ref<Canvas>	canvas;
	Kross::Ref<UI>		UserInterface;
#endif
public:
	Sandbox()
	{
#if S2D
		PushLayer(Kross::makeRef<Sandbox2D>());
#else
		canvas = std::make_shared<Canvas>();
		UserInterface = std::make_shared<UI>(canvas);
		PushLayer(canvas);
		PushLayer(UserInterface);
#endif
	}
};

Kross::Application* Kross::CreateApp()
{
	return new Sandbox();
}
