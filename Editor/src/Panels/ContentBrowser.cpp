#include "Editor_pch.h"
#include "ContentBrowser.h"
#include "Panel.h"
#include <fstream>
#include "IconsData/FolderIconTexture.h"
#include "IconsData/FileIconTexture.h"

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
	static bool DrawIcon(const Ref<Texture::T2D> &texture, const std::string &name, float scale, ImVec4 color)
	{
		auto i = location(name);
		if (i == cache.end()) cache.emplace(i, name);

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
		ImVec2 txtSize = ImGui::CalcTextSize(text.c_str());

		ImVec2 totalSize = { scale, scale + txtSize.y};

		std::string id = "##" + name;

		bool selected = false;
		bool result = false;

		ImGuiTableFlags tableFlags =
			ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX |
			ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX |
			(borders ? ImGuiTableFlags_Borders : 0);

		ImGui::PushID(id.c_str());
		ImGui::BeginTable(id.c_str(), 1, tableFlags);
		ImGui::TableNextRow(); ImGui::TableNextColumn();
		{
			ImGui::Selectable(id.c_str(), selected, ImGuiSelectableFlags_AllowItemOverlap, {
				totalSize.x - ImGui::GetStyle().ItemSpacing.x * 2.0f,
				scale
				});
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped) && ImGui::GetIO().MouseClicked[0])
				result = true;
			ImGui::SameLine(FLT_MIN, 0.0f);
			{
				ImGui::BeginTable(id.c_str(), 1, tableFlags);
				ImGui::TableNextRow(); ImGui::TableNextColumn();
				if (ratio > 1.0f) ImGui::Dummy({ 0.0f, (imgScale - imgSize.y) - ImGui::GetStyle().ItemSpacing.y});
				ImVec2 hRemain = { (totalSize.x - imgSize.x) / 2.0f, imgSize.y };

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
				ImGui::Dummy({ (totalSize.x - txtSize.x) / 2.0f, 1.0f }); ImGui::SameLine(0.0f, 0.0f);
				ImGui::Text("%s", text.c_str());
				ImGui::EndTable();
			}
		}
		ImGui::EndTable();
		ImGui::PopID();
		return result;
	}
	static bool DrawIcon(const std::string &textureName, const std::string &name, float size)
	{
		return DrawIcon(Stack<Texture::T2D>::Get(textureName.c_str()), name, size, { -1.0f, -1.0f, -1.0f, -1.0f });
	}
	ContentBrowser::ContentBrowser()
		: m_Flags(0), m_CurrentDirectory(s_AssetsPath), iconScale(128)
	{
		SetName("FileSystem");
		Stack<Texture::T2D>::Add(FolderIconTexture::Create());
		Stack<Texture::T2D>::Add(FileIconTexture::Create());
	}
	void ContentBrowser::Show()
	{
		if (!Panel::Manager().s_bContentBrowser) {
			for (auto &key : cache) Stack<Texture::T2D>::Del(key.c_str());
			cache.clear();
			return;
		}

		iconScale = std::clamp<float>(iconScale, 64.0f, 512.0f);

		ImGui::Begin(GetName(), &Panel::Manager().s_bContentBrowser, m_Flags);

		if (m_CurrentDirectory != std::filesystem::path(s_AssetsPath)) {
			if (ImGui::Button("<<")) {
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
				KROSS_TRACE("{0}", m_CurrentDirectory.string().c_str());
			}
		}
		int columns = (int)std::floor(std::abs(ImGui::GetContentRegionAvail().x / iconScale));
		auto flags = ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingStretchSame | (borders ? ImGuiTableFlags_Borders : 0);
		columns = std::clamp<int>(columns, 1, 64);
		if (!ImGui::BeginTable(GetName(), columns, flags, ImGui::GetContentRegionAvail(), iconScale)) {
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
				if (DrawIcon("FolderIcon", filename.string(), iconScale)) {
					m_CurrentDirectory /= filename;
					KROSS_TRACE("{0}", m_CurrentDirectory.string().c_str());
				}
			} else {
				std::string img = GetIconByTypeExtension(ext);
				if (img == "Image")
					DrawIcon(
					Stack<Texture::T2D>::Get(filename.string().c_str(), absolute.c_str()),
					filename.string().c_str(),
					iconScale,
					{ 1.0f, 1.0f, 1.0f, 1.0f }
				);
				else
					DrawIcon(GetIconByTypeExtension(ext), filename.string(), iconScale);
			}
		}
		ImGui::EndTable();
		ImGui::End();
	}

}
