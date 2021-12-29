#include "Editor_pch.h"
#include "RendererStats.h"
#include "Panel.h"

namespace Kross {
	RendererStats::RendererStats()
		: frames(new float[plotsize])
	{
		memset(frames, 0, plotsize);
		SetName("Renderer Status");
		KROSS_INFO("Panel '{0}' Constructed", GetName());
	}
	RendererStats::~RendererStats()
	{
		delete[] frames;
	}
	void RendererStats::Show(double ts)
	{
		if (!Panel::Manager().s_bRendererStats) return;

		float f = 0.0f;
		if (plot_type == 0) f = (float)(0.1 / ts);
		else if (plot_type == 1) f = (float)ts;

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


		std::string title(GetName());
		char buf[64] = { 0 };
		if(plot_type == 0) sprintf_s(buf, " | FPS: %.1f###StatusTitle", FrameRate);
		else if(plot_type == 1) sprintf_s(buf, " | Elapsed: %.4lf###StatusTitle", ts);
		title += buf;
		if (ImGui::Begin(title.c_str(), &Panel::Manager().s_bRendererStats, ImGuiWindowFlags_NoCollapse))
		{
			ImGui::PushItemWidth(ImGui::CalcTextSize(frame_plot_options[plot_type]).x + (ImGui::GetFontSize()*2));
			ImGui::Combo("mode", &plot_type, frame_plot_options, 2); ImGui::SameLine(); Panel::ShowHelperMarker(
				"Select what plot type you want."
				"Frames per Second mode, or FPS, is the number of how many frames the application can push out in a single second."
				"Elapsed Time mode, is the plot of how many milisseconds has passed since the last frame was drawn."
			);

			ImGui::Checkbox("Pause Animation", &pause_frame_plot_animation);
			float max = 0;
			for (size_t i = 0; i < plotsize; i++)
				max = std::max(frames[i], max);
			const char *l = plot_type ? "Timestep" : "Frames";
			static int oldWidth = 0;
			int width = int(ImGui::GetContentRegionAvail().x - ImGui::GetFontSize() - ImGui::CalcTextSize(l).x);
			if (oldWidth != width) {
				float *newFrame = new float[width];
				if (width > plotsize) {
					for (int i = 0; i < plotsize; i++) newFrame[i] = frames[i];
					for (int i = plotsize - 1; i < width; i++) newFrame[i] = f;
				} else for (int i = 0; i < width; i++) newFrame[i] = frames[i];
				delete[] frames;
				frames = newFrame;
				plotsize = width;
				oldWidth = width;
			}
			ImGui::PlotLines("", frames, plotsize - 1, 0, "", 0, std::min(120.0f, max), ImVec2((float)width, 32)); ImGui::SameLine();
			ImGui::Text(plot_type ? "Timestep" : "Frames");
			ImGui::Separator();
			ImGui::Text("Quad Count: %d", Renderer2D::getStats().QuadCount);
			ImGui::Text("Draw Calls: %d", Renderer2D::getStats().DrawCount);
		}
		ImGui::End();
	}
}
