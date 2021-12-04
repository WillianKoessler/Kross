#include "Viewport.h"

namespace Kross {
	Viewport::Viewport(const std::string& name, const Ref<Texture::T2D>& canvas)
		: m_strName(name), m_Canvas(canvas)
	{
	}
	void Viewport::Show(double ts)
	{
		if(!Manager().s_bViewport)
		//if (!m_Canvas) {
		//	KROSS_WARN("Cannot Render Viewport. Canvas is empty");
		//	return;
		//}

		KROSS_TRACE_ONCE("Canvas Name: {0}", m_Canvas->GetName());

		ImGuiIO& io = ImGui::GetIO();
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground;
		if (ImGui::Begin("viewport", &Manager().s_bViewport, flags)) {
			//void* image = (void*)(uintptr_t)m_Canvas->GetID();
			void* image = (void*)(uintptr_t)Stack<Texture::T2D>::instance().Get("mamma")->GetID();
			ImGui::Image(image,
				ImGui::GetContentRegionAvail(),
				ImVec2(0, 1),
				ImVec2(1, 0));
		}
		ImGui::End();
	}
}