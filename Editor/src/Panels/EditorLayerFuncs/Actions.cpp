#include "Editor_pch.h"
#include "EditorLayer.h"
#include "Panels.h"

namespace Kross {
	void EditorLayer::Actions()
	{
		if (ActionManager::IsActionPerformed("NewScene")) {
			m_Scene = Scene("main");
			sceneHierarchy->SetContext(m_Scene);
			entityProperties->SetContext(m_Scene);
		}
		if (ActionManager::IsActionPerformed("OpenScene")) {
			m_Scene = Scene();
			sceneHierarchy->SetContext(m_Scene);
			entityProperties->SetContext(m_Scene);
			m_Scene.LoadScene();
		}
		if (ActionManager::IsActionPerformed("SaveScene")) {
			m_Scene.SaveScene();
		}
		if (ActionManager::IsActionPerformed("SaveSceneAs")) {
			m_Scene.SaveScene(FileDialog::SaveFile("Kross Scene (.kross)\0*.kross\0\0"));
		}
		if (ActionManager::IsActionPerformed("SelectionTool")) {
			m_GuizmoType = -1;
		}
		if (ActionManager::IsActionPerformed("TranslationTool")) {
			m_GuizmoType = ImGuizmo::TRANSLATE;
		}
		if (ActionManager::IsActionPerformed("RotationTool")) {
			m_GuizmoType = ImGuizmo::ROTATE;
		}
		if (ActionManager::IsActionPerformed("ScaleTool")) {
			m_GuizmoType = ImGuizmo::SCALE;
		}
	}
}
