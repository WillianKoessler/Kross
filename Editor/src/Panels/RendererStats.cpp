#include "RendererStats.h"

namespace Kross {
	RendererStats::RendererStats(const char* name)
		: frames(new float[plotsize])
	{
		SetName(name);
		KROSS_INFO("RendererStats Panel '{0}' Constructed", name);
	}
	RendererStats::~RendererStats()
	{
		delete[] frames;
	}
	void RendererStats::Show(double ts)
	{
		if (!Manager().s_bRendererStats) return;

		float f = 0.0f;
		if (plot_type == 0) f = (float)(0.1 / ts);
		else if (plot_type == 1) f = (float)ts;

		static bool once = false;
		if (!once) { for (size_t i = 0; i < plotsize; i++)  frames[i] = f; once = true; }
		static float temp;
		if (!pause_frame_plot_animation)
		{
			frames[plotsize - 1] = f;
			for (size_t i = 0; i < plotsize; i++)
			{
				temp = frames[i + 1];
				frames[i] = temp;
			}
		}
		framerate_buffer += 0.1f / (float)ts;
		if (frame_count++ % rate_tick == 0) { FrameRate = framerate_buffer / rate_tick; framerate_buffer = 0; }


		std::string title = GetName();
		char buf[64];
		if(plot_type == 0) sprintf_s(buf, " | FPS: %.1f###StatusTitle", FrameRate);
		else if(plot_type == 1) sprintf_s(buf, " | Elapsed: %.4lf###StatusTitle", ts);
		title += buf;
		if (ImGui::Begin(title.c_str(), &Manager().s_bRendererStats, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::Combo("mode", &plot_type, frame_plot_options, 2); ImGui::SameLine(); ShowHelperMarker(
				"Select what plot type you want."
				"Frames per Second mode, or FPS, is the number of how many frames the application can push out in a single second."
				"Elapsed Time mode, is the plot of how many milisseconds has passed since the last frame was drawn."
			);

			ImGui::Checkbox("Pause Animation", &pause_frame_plot_animation);
			float max = 0;
			for (size_t i = 0; i < plotsize; i++)
				max = std::max(frames[i], max);

			ImGui::PlotLines("", frames, plotsize - 1, 0, "", 0, std::min(120.0f, max)); ImGui::SameLine();
			ImGui::Text(plot_type ? "Timestep" : "Frames");
			ImGui::Separator();
			ImGui::Text("Quad Count: %d", Renderer2D::getStats().QuadCount);
			ImGui::Text("Draw Calls: %d", Renderer2D::getStats().DrawCount);
		}
		ImGui::End();
	}
}