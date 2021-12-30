#include "Editor_pch.h"
#include "ContentBrowser.h"
#include "Panel.h"
#include <fstream>
#include "IconsData/Folder/FolderIconTexture.h"
#include "IconsData/File/FileIconTexture.h"

static bool borders = false;
static const std::filesystem::path s_AssetsPath("assets");

namespace Kross {
	static std::vector<std::string> cache;
	static auto location(const std::string &key) { return std::lower_bound(cache.begin(), cache.end(), key); }
	static std::string FileName(const std::string &path)
	{
		size_t slash = path.find_last_of("/\\");
		slash = (slash != std::string::npos) ? slash + 1 : 0;
		size_t dot = path.find_last_of(".");
		return path.substr(slash, dot);
	}
	static std::string GetIconByTypeExtension(const std::string &ext)
	{
		if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp")
			return "Image";
		else
			return "FileIcon";
		return "blank";
	}
	static bool DrawIcon(const Ref<Texture::T2D> &texture, const std::string &name, float scale, ImVec4 color, bool activation)
	{
		{
			auto file = Stack<Texture::T2D>::Get("FileIcon");
			auto folder = Stack<Texture::T2D>::Get("FolderIcon");
			if (texture != file && texture != folder) {
				auto i = location(name);
				if (i == cache.end())
					cache.emplace(i, name);
			}
		}

		ImTextureID texID = (void *)(uintptr_t)texture->GetID();
		float ratio = texture->GetWidth() / (float)texture->GetHeight();
		float imgScale = scale * (1.0f - (0.1f * phi<float>()) * 2.0f);
		ImVec2 imgSize = { imgScale * ratio, imgScale };
		if (imgSize.x > imgScale) {
			imgSize.y = imgScale / ratio;
			imgSize.x = imgScale;
		}
		if (imgSize.y > imgScale) {
			imgSize.y = imgScale;
			imgSize.x = imgScale / ratio;
		}

		std::string text = "";
		{
			float w = 0;
			for (int i = 0; i < name.size(); i++) {
				w += ImGui::GetFont()->FindGlyph(name[i])->AdvanceX;
				if (w > imgScale) {
					w = 0;
					text += '\n';
				}
				text += name[i];
			}
		}

		ImVec2 txtSize = { ImGui::CalcTextSize(text.c_str()).x, std::count(text.begin(), text.end(), '\n') * ImGui::GetFontSize() };

		std::string id = "##" + name;

		bool selected = false;
		bool result = false;
		bool hovered = false;

		ImGuiTableFlags tableFlags =
			ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX |
			ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX |
			(borders ? ImGuiTableFlags_Borders : 0);

		ImGui::PushID(id.c_str());
		ImGui::BeginTable(id.c_str(), 1, tableFlags);
		ImGui::TableNextRow(); ImGui::TableNextColumn();
		{
			ImGui::Selectable(id.c_str(), selected, ImGuiSelectableFlags_AllowItemOverlap, {
				scale - ImGui::GetStyle().ItemSpacing.x * 2.0f,
				scale
				});
			hovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped);
			result = hovered && activation;

			ImGui::SameLine(FLT_MIN, 0.0f);
			{
				ImGui::BeginTable(id.c_str(), 1, tableFlags);
				ImGui::TableNextRow(); ImGui::TableNextColumn();
				if (ratio > 1.0f) ImGui::Dummy({ 0.0f, (imgScale - imgSize.y) - ImGui::GetStyle().ItemSpacing.y });
				ImVec2 hRemain = { (scale - imgSize.x) / 2.0f, imgSize.y };

				ImGui::Dummy(hRemain);
				ImGui::SameLine(0.0f, 0.0f);
				if (color.x == -1.0f && color.y == -1.0f && color.z == -1.0f && color.w == -1.0f)
				{
					auto col = ImGui::GetStyle().Colors;
					if (selected) color = col[ImGuiCol_ButtonHovered];
					else color = col[ImGuiCol_Button];
					if (result) color = col[ImGuiCol_ButtonActive];
				}

				ImGui::Image(texID, imgSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), color);
				ImGui::SameLine(0.0f, 0.0f);
				ImGui::Dummy(hRemain);

				ImGui::TableNextColumn();
				ImGui::Dummy({ (scale - txtSize.x) / 2.0f, 1.0f }); ImGui::SameLine(0.0f, 0.0f);

				std::string display;
				if (hovered) display = text;
				else {
					size_t breakLine = text.find_first_of('\n');
					if (breakLine != std::string::npos) display = text.substr(0, breakLine - 3) + "...";
					else display = text;
				}
				ImGui::Text("%s", display.c_str());
				ImGui::EndTable();
			}
		}
		ImGui::EndTable();
		ImGui::PopID();
		return result;
	}
	static bool DrawIcon(const std::string &textureName, const std::string &name, float size, bool activation)
	{
		return DrawIcon(Stack<Texture::T2D>::Get(textureName.c_str()), name, size, { -1.0f, -1.0f, -1.0f, -1.0f }, activation);
	}
	ContentBrowser::ContentBrowser()
		: m_CurrentDirectory(s_AssetsPath)
	{
		m_Flags |= ImGuiWindowFlags_MenuBar;
		SetName("FileSystem");
		Stack<Texture::T2D>::Add(FolderIconTexture::Create());
		Stack<Texture::T2D>::Add(FileIconTexture::Create());
	}

	class PopUpData
	{
	public:
		bool show = false;
		std::string title = "";
		std::string message = "";
		uint8_t button = 0;
		const void *myData = nullptr;
		size_t size = 0;
		std::function<void(const void *, size_t)> func;

		PopUpData() = default;
		PopUpData(const char *title, const char *msg, uint8_t button, const void *data, size_t size)
			: show(true), title(title), message(msg), button(button), size(size)
		{
			void *d = malloc(size);
			memcpy_s(d, size, data, size);
			myData = d;
		}
	};

	void ContentBrowser::Show()
	{
		enum Button { OK = 0b0001, YES = 0b0010, NO = 0b0100, CANCEL = 0b1000, };
		static PopUpData popup;

		static bool clearCache = false;
		if (clearCache) {
			clearCache = false;
			for (auto &key : cache)
				Stack<Texture::T2D>::Del(key.c_str());
			cache.clear();
		}
		if (!Panel::Manager().s_bContentBrowser)
			return;

		ImGuiIO &io = ImGui::GetIO();
		iconScale = std::clamp<int>(iconScale, iconScaleMin, iconScaleMax);
		static constexpr const int mBtn = 0;
		io.MouseDoubleClickTime = doubleClickTiming;

		bool click = io.MouseClicked[mBtn];
		bool doubleClick = io.MouseDoubleClicked[mBtn];

		ImGui::Begin(GetName(), &Panel::Manager().s_bContentBrowser, m_Flags);
		{
			ImGui::BeginMenuBar();
			if (ImGui::MenuItem("<<", nullptr, nullptr, m_CurrentDirectory != std::filesystem::path(s_AssetsPath))) {
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
				clearCache = true;
				KROSS_TRACE("{0}", m_CurrentDirectory.string().c_str());
			}
			std::string path = m_CurrentDirectory.string();
			std::string reconstructed = "";
			size_t div = 0;
			while (div != std::string::npos) {
				div = path.find_first_of('\\');
				std::string subPath = path.substr(0, div);
				reconstructed += subPath + "\\";
				if (ImGui::MenuItem(subPath.c_str())) {
					m_CurrentDirectory = std::filesystem::path(reconstructed);
				}
				path = path.substr(div + 1);
			}
			ImGui::EndMenuBar();
		}
		int columns = std::abs((int)ImGui::GetContentRegionAvail().x / iconScale);
		auto flags = ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingStretchSame | (borders ? ImGuiTableFlags_Borders : 0);
		columns = std::clamp<int>(columns, 1, 64);
		if (!ImGui::BeginTable(GetName(), columns, flags, ImGui::GetContentRegionAvail(), (float)iconScale)) {
			ImGui::End();
			return;
		}

		ImGui::TableNextRow();

		for (auto &it : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			ImGui::TableNextColumn();
			std::filesystem::path filename = it.path().filename();
			std::string absolute = std::filesystem::absolute(it.path()).string();
			std::string name = FileName(it.path().string());
			std::string ext = filename.extension().string();
			if (it.is_directory()) {
				if (DrawIcon("FolderIcon", filename.string(), (float)iconScale, click)) {
					m_CurrentDirectory /= filename;
					clearCache = true;
					KROSS_TRACE("{0}", m_CurrentDirectory.string().c_str());
				}
			} else {
				std::string img = GetIconByTypeExtension(ext);
				if (img == "Image") {
					DrawIcon(Stack<Texture::T2D>::Get(filename.string().c_str(), absolute.c_str()), filename.string().c_str(), (float)iconScale, { 1.0f, 1.0f, 1.0f, 1.0f }, doubleClick);
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
						ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", absolute.c_str(), absolute.length() * sizeof(absolute.c_str()), ImGuiCond_Once);
						ImGui::EndDragDropSource();
					}

				} else {
					if (DrawIcon(GetIconByTypeExtension(ext), filename.string(), (float)iconScale, doubleClick)) {
						if (ext == ".kross") {
							popup = PopUpData("Open Scene", "Load this Scene file?", YES | NO, absolute.c_str(), absolute.size());
							popup.func = [](const void *data, size_t size) { ActionManager::TriggerAction("OpenScene", data, size); };
						}
					}
				}
			}
		}

		if (ImGui::BeginPopupContextWindow("Settings", ImGuiPopupFlags_MouseButtonRight)) {
			ImGui::BeginTable("##ContentBrowserContexMenuSettings", 2);
			ImGui::TableSetupColumn("##Labels", ImGuiTableColumnFlags_WidthFixed, 32.0f);
			ImGui::TableSetupColumn("##Labels", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Scale");
			ImGui::TableNextColumn();
			ImGui::PushItemWidth(256);
			ImGui::SliderInt("##IconSize", &iconScale, iconScaleMin, iconScaleMax);
			ImGui::PopItemWidth();
			ImGui::TableNextColumn();
			ImGui::EndTable();
			ImGui::EndPopup();
		}
		{
			if (popup.show) {
				ImGui::OpenPopup(popup.title.c_str());
				ImGui::SetNextWindowPos({ Application::Get().GetWindow().GetWidth() * 0.5f, Application::Get().GetWindow().GetHeight() * 0.5f }, 0, { 0.5f, 0.5f });
			}
			if (ImGui::BeginPopupModal(popup.title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
				ImGui::Text(popup.message.c_str());
				ImGui::Dummy({ 1, 1 });
				ImVec2 buttonSize = { 50, ImGui::GetFontSize() + ImGui::GetStyle().ItemSpacing.y * 2.0f };
				bool shouldClose = false;
				if (popup.button & OK) { ImGui::SameLine(); if (ImGui::Button("OK", buttonSize)) { shouldClose = true; } }
				if (popup.button & YES) { ImGui::SameLine(); if (ImGui::Button("Yes", buttonSize)) { popup.func(popup.myData, popup.size); shouldClose = true; } }
				if (popup.button & NO) { ImGui::SameLine(); if (ImGui::Button("No", buttonSize)) { shouldClose = true; } }
				if (popup.button & CANCEL) { ImGui::SameLine(); if (ImGui::Button("Cancel", buttonSize)) { shouldClose = true; } }
				if (shouldClose) {
					ImGui::CloseCurrentPopup();
					popup = PopUpData();
				}
				ImGui::EndPopup();
			}
		}
		ImGui::EndTable();
		ImGui::End();
	}
	void ContentBrowser::Settings()
	{
	}
}
