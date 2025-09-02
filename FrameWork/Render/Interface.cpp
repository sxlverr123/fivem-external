#include "Interface.hpp"

#include <Cheat/Options.hpp>
#include <Cheat/Cheat.hpp>
#include <Cheat/ConfigSystem.hpp>
#include <Cheat/FivemSDK/Fivem.hpp>
#include "../Dependencies/ImGui/font.h"
#include "../Dependencies/ImGui/image.h"
#include "../Dependencies/ImGui/imgui_freetype.h"
#include "../Dependencies/ImGui/imgui_settings.h"

inline Cheat::ConfigManager Condif;

struct MarkerT
{
	Vector3D Position;
	std::string Name;
};

std::vector<MarkerT> MarkerList;

DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;


float tab_size = 0.f;
float arrow_roll = 0.f;
bool tab_opening = true;
int rotation_start_index;
static bool Dcheckbox = false;
static bool Headcheckbox = false;
static bool Healthcheckbox = false;
static bool Namecheckbox = false;
static bool Distancecheckbox = false;
static bool weaponcheckbox = false;
static bool Filterteams = false;
static bool Filterteams_map = false;
static bool Bonecheckbox = false;
static float boxtk = 1.f;
static float hptk = 1.f;
static float hdtk = 1.f;
static float bonetk = 1.f;
static bool Flogs[6] = { false, false, false, false,false };
const char* Flogss[6] = { "Head", "Health Bar", "Player Name", "Distance","Weapon" };
ImVec2 button_size(200.0f, 30.0f); /* https://discord.gg/daPJ7BWT7k */
ImVec2 button_size_header(130.0f, 30.0f); /* https://discord.gg/daPJ7BWT7k */

namespace font
{
	ImFont* poppins_medium = nullptr;
	ImFont* poppins_medium_low = nullptr;
	ImFont* tab_icon = nullptr;
	ImFont* chicons = nullptr;
	ImFont* tahoma_bold = nullptr;
	ImFont* tahoma_bold2 = nullptr;
}
namespace image
{
	ID3D11ShaderResourceView* bg = nullptr;
	ID3D11ShaderResourceView* logo = nullptr;
	ID3D11ShaderResourceView* logo_general = nullptr;

	ID3D11ShaderResourceView* arrow = nullptr;
	ID3D11ShaderResourceView* bell_notify = nullptr;
	ID3D11ShaderResourceView* roll = nullptr;


}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); /* https://discord.gg/daPJ7BWT7k */
namespace FrameWork
{
	void Interface::Initialize(HWND Window, HWND TargetWindow, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
	{
		hWindow = Window;
		IDevice = Device;

		ImGui::CreateContext(); /* https://discord.gg/daPJ7BWT7k */
		ImGui_ImplWin32_Init(hWindow); /* https://discord.gg/daPJ7BWT7k */
		ImGui_ImplDX11_Init(Device, DeviceContext); /* https://discord.gg/daPJ7BWT7k */
		ImGuiIO& io = ImGui::GetIO(); /* https://discord.gg/daPJ7BWT7k */ (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImFontConfig cfg;
		cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;
		D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };

		font::poppins_medium = io.Fonts->AddFontFromMemoryTTF(poppins_medium, sizeof(poppins_medium), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic()); /* https://discord.gg/daPJ7BWT7k */
		font::poppins_medium_low = io.Fonts->AddFontFromMemoryTTF(poppins_medium, sizeof(poppins_medium), 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic()); /* https://discord.gg/daPJ7BWT7k */
		font::tab_icon = io.Fonts->AddFontFromMemoryTTF(tabs_icons, sizeof(tabs_icons), 24.f, &cfg, io.Fonts->GetGlyphRangesCyrillic()); /* https://discord.gg/daPJ7BWT7k */
		font::tahoma_bold = io.Fonts->AddFontFromMemoryTTF(tahoma_bold, sizeof(tahoma_bold), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic()); /* https://discord.gg/daPJ7BWT7k */
		font::tahoma_bold2 = io.Fonts->AddFontFromMemoryTTF(tahoma_bold, sizeof(tahoma_bold), 28.f, &cfg, io.Fonts->GetGlyphRangesCyrillic()); /* https://discord.gg/daPJ7BWT7k */
		font::chicons = io.Fonts->AddFontFromMemoryTTF(chicon, sizeof(chicon), 20.f, &cfg, io.Fonts->GetGlyphRangesCyrillic()); /* https://discord.gg/daPJ7BWT7k */


		if (image::bg == nullptr) D3DX11CreateShaderResourceViewFromMemory(Device, background_image, sizeof(background_image), &info, pump, &image::bg, 0); /* https://discord.gg/daPJ7BWT7k */
		if (image::logo == nullptr) D3DX11CreateShaderResourceViewFromMemory(Device, logo, sizeof(logo), &info, pump, &image::logo, 0); /* https://discord.gg/daPJ7BWT7k */
		if (image::logo_general == nullptr) D3DX11CreateShaderResourceViewFromMemory(Device, logo_general, sizeof(logo_general), &info, pump, &image::logo_general, 0); /* https://discord.gg/daPJ7BWT7k */


		if (image::arrow == nullptr) D3DX11CreateShaderResourceViewFromMemory(Device, arrow, sizeof(arrow), &info, pump, &image::arrow, 0); /* https://discord.gg/daPJ7BWT7k */
		if (image::bell_notify == nullptr) D3DX11CreateShaderResourceViewFromMemory(Device, bell_notify, sizeof(bell_notify), &info, pump, &image::bell_notify, 0); /* https://discord.gg/daPJ7BWT7k */
		if (image::roll == nullptr) D3DX11CreateShaderResourceViewFromMemory(Device, roll, sizeof(roll), &info, pump, &image::roll, 0); /* https://discord.gg/daPJ7BWT7k */

		MarkerList.push_back({ Vector3D(212.6681f, -813.7118f, 30.7386f), ("Meeting Point") }); /* https://discord.gg/daPJ7BWT7k */
		MarkerList.push_back({ Vector3D(99.7721f, -743.7130f, 45.7547f), ("FIB-Tower") }); /* https://discord.gg/daPJ7BWT7k */
		MarkerList.push_back({ Vector3D(-1039.2391f, -2666.4702f, 13.8307f), ("Airport") }); /* https://discord.gg/daPJ7BWT7k */
		MarkerList.push_back({ Vector3D(3627.5176f, 3754.3137f, 28.5157f), ("Humanlabs") }); /* https://discord.gg/daPJ7BWT7k */
		MarkerList.push_back({ Vector3D(1404.8857f, 3162.1936f, 40.4341f), ("Sandyshores Airfield") }); /* https://discord.gg/daPJ7BWT7k */
		MarkerList.push_back({ Vector3D(-2326.4241f, 3053.1711f, 32.8150f), ("Armybase") }); /* https://discord.gg/daPJ7BWT7k */
		MarkerList.push_back({ Vector3D(501.6581f, 5604.9321f, 797.9105f), ("Mount Chiliad") }); /* https://discord.gg/daPJ7BWT7k */
		MarkerList.push_back({ Vector3D(-1448.0753f, -766.6392f, 23.5332f), ("Del Perro Pier") }); /* https://discord.gg/daPJ7BWT7k */
		MarkerList.push_back({ Vector3D(298.7408f, -581.9695f, 43.2608f), ("Pillbox") }); /* https://discord.gg/daPJ7BWT7k */
	}


	void ImRotateStart()
	{
		rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
	}
	ImVec2 ImRotationCenter()
	{
		ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); /* https://discord.gg/daPJ7BWT7k */ // bounds

		const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
		for (int i = rotation_start_index; i < buf.Size; i++)
			l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos); /* https://discord.gg/daPJ7BWT7k */

		return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); /* https://discord.gg/daPJ7BWT7k */ // or use _ClipRectStack?
	}
	void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
	{
		float s = sin(rad), c = cos(rad); /* https://discord.gg/daPJ7BWT7k */
		center = ImRotate(center, s, c) - center;

		auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
		for (int i = rotation_start_index; i < buf.Size; i++)
			buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
	}
	void Particles()
	{
		ImVec2 screen_size = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };

		static ImVec2 partile_pos[100];
		static ImVec2 partile_target_pos[100];
		static float partile_speed[100];
		static float partile_radius[100];


		for (int i = 1; i < 50; i++)
		{
			if (partile_pos[i].x == 0 || partile_pos[i].y == 0)
			{
				partile_pos[i].x = rand() % (int)screen_size.x + 1;
				partile_pos[i].y = 15.f;
				partile_speed[i] = 1 + rand() % 25;
				partile_radius[i] = rand() % 4;

				partile_target_pos[i].x = rand() % (int)screen_size.x;
				partile_target_pos[i].y = screen_size.y * 2;
			}

			partile_pos[i] = ImLerp(partile_pos[i], partile_target_pos[i], ImGui::GetIO().DeltaTime * (partile_speed[i] / 60)); /* https://discord.gg/daPJ7BWT7k */

			if (partile_pos[i].y > screen_size.y)
			{
				partile_pos[i].x = 0;
				partile_pos[i].y = 0;
			}

			ImGui::GetWindowDrawList()->AddCircleFilled(partile_pos[i], partile_radius[i], ImColor(71, 226, 67, 255 / 2)); /* https://discord.gg/daPJ7BWT7k */
		}

	}

	std::string GetDesktopUsername()
	{
		char username[256];
		DWORD username_len = sizeof(username); /* https://discord.gg/daPJ7BWT7k */
		if (GetUserNameA(username, &username_len))
		{
			return std::string(username); /* https://discord.gg/daPJ7BWT7k */
		}
		return "Unknown User";
	}

	void Interface::UpdateStyle()
	{
		Assets::Initialize(IDevice); /* https://discord.gg/daPJ7BWT7k */
		Condif.Setup(); /* https://discord.gg/daPJ7BWT7k */
	}

	void Interface::RenderGui()
	{
		ImGuiStyle* s = &ImGui::GetStyle(); /* https://discord.gg/daPJ7BWT7k */

		s->WindowPadding = ImVec2(0, 0), s->WindowBorderSize = 0;
		s->ItemSpacing = ImVec2(20, 20); /* https://discord.gg/daPJ7BWT7k */

		s->ScrollbarSize = 8.f;

		//ImGui::GetBackgroundDrawList()->AddImage(image::bg, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255)); /* https://discord.gg/daPJ7BWT7k */

		ImGui::SetNextWindowSize(ImVec2(c::bg::size) + ImVec2(tab_size, 0)); /* https://discord.gg/daPJ7BWT7k */

		if (!bIsMenuOpen)
			return;


		ImGui::Begin("IMGUI MENU", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus); /* https://discord.gg/daPJ7BWT7k */
		{
			const ImVec2& pos = ImGui::GetWindowPos(); /* https://discord.gg/daPJ7BWT7k */
			const auto& p = ImGui::GetWindowPos(); /* https://discord.gg/daPJ7BWT7k */
			const ImVec2 spacing = ImGui::GetStyle().ItemSpacing;

			ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(c::bg::size) + ImVec2(tab_size, 0), ImGui::GetColorU32(c::bg::background), c::bg::rounding); /* https://discord.gg/daPJ7BWT7k */
			ImGui::GetBackgroundDrawList()->AddRect(pos, pos + ImVec2(c::bg::size) + ImVec2(tab_size, 0), ImGui::GetColorU32(c::bg::outline_background), c::bg::rounding); /* https://discord.gg/daPJ7BWT7k */

			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(c::accent_text_color)); /* https://discord.gg/daPJ7BWT7k */

			ImGui::PushFont(font::tahoma_bold2); /* https://discord.gg/daPJ7BWT7k */ ImGui::RenderTextClipped(pos + ImVec2(60, 0) + spacing, pos + spacing + ImVec2(60, 60) + ImVec2(tab_size + (spacing.x / 2) - 30, 0), "Yusei.cc", NULL, NULL, ImVec2(0.5f, 0.5f), NULL); /* https://discord.gg/daPJ7BWT7k */ ImGui::PopFont(); /* https://discord.gg/daPJ7BWT7k */

			ImGui::RenderTextClipped(pos + ImVec2(60 + spacing.x, c::bg::size.y - 60 * 2), pos + spacing + ImVec2(60, c::bg::size.y) + ImVec2(tab_size, 0), "Lifetime", NULL, NULL, ImVec2(0.0f, 0.43f), NULL); /* https://discord.gg/daPJ7BWT7k */
			ImGui::RenderTextClipped(pos + ImVec2(60 + spacing.x, c::bg::size.y - 60 * 2), pos + spacing + ImVec2(60, c::bg::size.y) + ImVec2(tab_size, 0), "hefan2429", NULL, NULL, ImVec2(0.0f, 0.57f), NULL); /* https://discord.gg/daPJ7BWT7k */

			std::string desktop_username = GetDesktopUsername(); /* https://discord.gg/daPJ7BWT7k */
			std::string HeaderText = "Hello, " + desktop_username;

			float offset_x = -67.0f; // Verschiebung nach rechts (anpassen, falls nötig)

			ImGui::PushFont(font::tahoma_bold2); /* https://discord.gg/daPJ7BWT7k */
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 255, 255, 255)); /* https://discord.gg/daPJ7BWT7k */
			ImGui::RenderTextClipped(
				pos + ImVec2(offset_x, 0) + spacing,
				pos + spacing + ImVec2(60, 40) + ImVec2(tab_size + (spacing.x / 2) + 199, 0) + ImVec2(offset_x, 0),
				HeaderText.c_str(),
				NULL, NULL, ImVec2(1.f, 0.5f), NULL
			); /* https://discord.gg/daPJ7BWT7k */
			ImGui::PopFont(); /* https://discord.gg/daPJ7BWT7k */
			ImGui::PopStyleColor(); /* https://discord.gg/daPJ7BWT7k */

			ImGui::GetBackgroundDrawList()->AddImage(image::logo, pos + ImVec2(10, 10), pos + ImVec2(10, 10), ImVec2(100, 100), ImVec2(100, 100), ImColor(255, 255, 255, 255)); /* https://discord.gg/daPJ7BWT7k */


			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(90, 93, 100, 255)); /* https://discord.gg/daPJ7BWT7k */ ImGui::RenderTextClipped(pos + ImVec2(30, 20) + spacing, pos + spacing + ImVec2(60, 80) + ImVec2(tab_size + (spacing.x / 2) + 108, -20), "Welcome Back!", NULL, NULL, ImVec2(1.f, 0.5f), NULL); /* https://discord.gg/daPJ7BWT7k */ ImGui::PopStyleColor(); /* https://discord.gg/daPJ7BWT7k */

			static char Search[64] = { "" };
			ImGui::SetCursorPos(ImVec2(385 + tab_size, -20) + (s->ItemSpacing * 2)); /* https://discord.gg/daPJ7BWT7k */
			ImGui::BeginChild("", " ", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 60)); /* https://discord.gg/daPJ7BWT7k */
			ImGui::PushFont(font::tab_icon); /* https://discord.gg/daPJ7BWT7k */
			ImGui::Text("I"); /* https://discord.gg/daPJ7BWT7k */ ImGui::SameLine(); /* https://discord.gg/daPJ7BWT7k */ ImGui::Text("H"); /* https://discord.gg/daPJ7BWT7k */ ImGui::SameLine(); /* https://discord.gg/daPJ7BWT7k */ ImGui::Text("G"); /* https://discord.gg/daPJ7BWT7k */
			// ImGui::GetWindowDrawList()->AddText(pos + ImVec2(600, 36), ImColor(90, 93, 100, 255), "I"); /* https://discord.gg/daPJ7BWT7k */
			// ImGui::GetWindowDrawList()->AddText(pos + ImVec2(635, 36), ImColor(90, 93, 100, 255), "H"); /* https://discord.gg/daPJ7BWT7k */
			// ImGui::GetWindowDrawList()->AddText(pos + ImVec2(670, 36), ImColor(90, 93, 100, 255), "G"); /* https://discord.gg/daPJ7BWT7k */
			ImGui::PopFont(); /* https://discord.gg/daPJ7BWT7k */
			ImGui::SameLine(); /* https://discord.gg/daPJ7BWT7k */
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20.0f); /* https://discord.gg/daPJ7BWT7k */ 
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8.0f); /* https://discord.gg/daPJ7BWT7k */ 

			if (ImGui::Button("Unload", button_size_header))
			{
				g_options.menu.shutdown = true;
			}

			ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */
			ImGui::PopStyleColor(1); /* https://discord.gg/daPJ7BWT7k */


			const char* nametab_array1[6] = { "E", "D", "A", "B", "C","F" };

			const char* nametab_array[6] = { "Legit", "Visuals", "Skins", "Configs", "Settings","Chat" };
			const char* nametab_hint_array[6] = { "Aimbot,Rcs,Trigger", "Overlay,Chams,World", "All Game Skins", "Save your settings", "Element settings","Contact online users" };


			static int tabs = 0;

			ImGui::SetCursorPos(ImVec2(spacing.x + (60 - 22) / 2, 60 + (spacing.y * 2) + 22)); /* https://discord.gg/daPJ7BWT7k */
			ImGui::BeginGroup(); /* https://discord.gg/daPJ7BWT7k */
			{
				for (int i = 0; i < sizeof(nametab_array1) / sizeof(nametab_array1[0]); /* https://discord.gg/daPJ7BWT7k */ i++)
					if (ImGui::Tab(i == tabs, nametab_array1[i], nametab_array[i], nametab_hint_array[i], ImVec2(35 + tab_size, 35))) tabs = i;
			}
			ImGui::EndGroup(); /* https://discord.gg/daPJ7BWT7k */

			ImGui::SetCursorPos(ImVec2(8, 9) + spacing); /* https://discord.gg/daPJ7BWT7k */

			ImRotateStart(); /* https://discord.gg/daPJ7BWT7k */
			if (ImGui::CustomButton(1, image::roll, ImVec2(35, 35), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::accent_color))) tab_opening = !tab_opening;
			ImRotateEnd(1.57f * arrow_roll); /* https://discord.gg/daPJ7BWT7k */

			ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, -20 + spacing.y) + spacing, pos + spacing + ImVec2(60, c::bg::size.y - 60 - spacing.y * 3) + ImVec2(tab_size, 0), ImGui::GetColorU32(c::child::background), c::child::rounding); /* https://discord.gg/daPJ7BWT7k */
			ImGui::GetBackgroundDrawList()->AddRect(pos + ImVec2(0, -20 + spacing.y) + spacing, pos + spacing + ImVec2(60, c::bg::size.y - 60 - spacing.y * 3) + ImVec2(tab_size, 0), ImGui::GetColorU32(c::child::outline_background), c::child::rounding); /* https://discord.gg/daPJ7BWT7k */

			ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, c::bg::size.y - 60 - spacing.y * 2) + spacing, pos + spacing + ImVec2(60, c::bg::size.y - spacing.y * 2) + ImVec2(tab_size, 0), ImGui::GetColorU32(c::child::background), c::child::rounding); /* https://discord.gg/daPJ7BWT7k */
			ImGui::GetBackgroundDrawList()->AddRect(pos + ImVec2(0, c::bg::size.y - 60 - spacing.y * 2) + spacing, pos + spacing + ImVec2(60, c::bg::size.y - spacing.y * 2) + ImVec2(tab_size, 0), ImGui::GetColorU32(c::child::outline_background), c::child::rounding); /* https://discord.gg/daPJ7BWT7k */

			ImGui::GetWindowDrawList()->AddImage(image::logo, pos + ImVec2(0, c::bg::size.y - 60 - spacing.y * 2) + spacing + ImVec2(12, 12), pos + spacing + ImVec2(60, c::bg::size.y - spacing.y * 2) - ImVec2(12, 12), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255)); /* https://discord.gg/daPJ7BWT7k */

			ImGui::GetWindowDrawList()->AddCircleFilled(pos + ImVec2(63, c::bg::size.y - (spacing.y * 2) + 3), 10.f, ImGui::GetColorU32(c::child::background), 100.f); /* https://discord.gg/daPJ7BWT7k */
			ImGui::GetWindowDrawList()->AddCircleFilled(pos + ImVec2(63, c::bg::size.y - (spacing.y * 2) + 3), 6.f, ImColor(0, 255, 0, 255), 100.f); /* https://discord.gg/daPJ7BWT7k */

			//	Particles(); /* https://discord.gg/daPJ7BWT7k */


			static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;

			tab_alpha = ImClamp(tab_alpha + (4.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f); /* https://discord.gg/daPJ7BWT7k */
			tab_add = ImClamp(tab_add + (std::round(350.f) * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f); /* https://discord.gg/daPJ7BWT7k */

			if (tab_alpha == 0.f && tab_add == 0.f) active_tab = tabs;

			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * s->Alpha); /* https://discord.gg/daPJ7BWT7k */
			if (tabs == 0)
			{


				ImGui::SetCursorPos(ImVec2(60 + tab_size, 60) + (s->ItemSpacing * 2)); /* https://discord.gg/daPJ7BWT7k */
				ImGui::BeginGroup(); /* https://discord.gg/daPJ7BWT7k */
				{
					ImGui::BeginChild("D", "Aimbot", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 315)); /* https://discord.gg/daPJ7BWT7k */
					{

						ImVec2 p = ImGui::GetWindowPos(); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox("Aimbot", &g_options.aim.aimbot.enabled); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Keybind("Aimbot keybind", &g_options.aim.aimbot.bind, true); /* https://discord.gg/daPJ7BWT7k */
						ImGui::CheckPicker("Draw Fov", "Fov Color", &g_options.aim.aimbot.drawfov, g_options.aim.aimbot.fovcolor); /* https://discord.gg/daPJ7BWT7k */
						ImGui::SliderInt(("Fov Size"), &g_options.aim.aimbot.fov, 0, 360); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox("Target Npc", &g_options.aim.aimbot.allownpc); /* https://discord.gg/daPJ7BWT7k */
						ImGui::SliderInt(("Smooth X"), &g_options.aim.aimbot.xsmooth, 0, 100); /* https://discord.gg/daPJ7BWT7k */
						ImGui::SliderInt(("Smooth Y"), &g_options.aim.aimbot.ysmooth, 0, 100); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox(("Lock NPCs"), &g_options.aim.aimbot.allownpc); /* https://discord.gg/daPJ7BWT7k */
						ImGui::SliderInt(("Distance"), &g_options.aim.aimbot.maxdistance, 0, 500); /* https://discord.gg/daPJ7BWT7k */
					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */

					ImGui::BeginChild("D", "Triggerbot", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 180)); /* https://discord.gg/daPJ7BWT7k */
					{

						ImVec2 p = ImGui::GetWindowPos(); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox(("Triggerbot"), &g_options.aim.trigger.enabled); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Keybind("Triggerbot keybind", &g_options.aim.trigger.bind, true); /* https://discord.gg/daPJ7BWT7k */
						ImGui::CheckPicker("Draw Fov", "Fov Color", &g_options.aim.trigger.drawfov, g_options.aim.trigger.fovcolor); /* https://discord.gg/daPJ7BWT7k */
						ImGui::SliderInt(("Fov Size"), &g_options.aim.trigger.fov, 0, 360); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox(("Target NPC"), &g_options.aim.trigger.allownpc); /* https://discord.gg/daPJ7BWT7k */
						ImGui::SliderInt(("Delay"), &g_options.aim.trigger.delay, 0, 1000); /* https://discord.gg/daPJ7BWT7k */
			
					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */


				}
				ImGui::EndGroup(); /* https://discord.gg/daPJ7BWT7k */
				ImGui::SameLine(); /* https://discord.gg/daPJ7BWT7k */

				ImGui::BeginGroup(); /* https://discord.gg/daPJ7BWT7k */
				{

					ImGui::BeginChild("A", "Silent Aim", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 295)); /* https://discord.gg/daPJ7BWT7k */
					{
						ImGui::Checkbox(("Silent Aim"), &g_options.aim.silent.enabled); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Keybind("Silent keybind", &g_options.aim.silent.bind, true); /* https://discord.gg/daPJ7BWT7k */
						ImGui::CheckPicker("Draw Fov", "Fov Color", &g_options.aim.silent.drawfov, g_options.aim.silent.fovcolor); /* https://discord.gg/daPJ7BWT7k */
						ImGui::SliderInt(("Fov Size"), &g_options.aim.silent.fov, 0, 360); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox(("Target NPC"), &g_options.aim.silent.allownpc); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox(("Magic Bullet"), &g_options.aim.silent.magicbullet); /* https://discord.gg/daPJ7BWT7k */
						ImGui::SliderInt(("Distance"), &g_options.aim.silent.maxdistance, 0, 500); /* https://discord.gg/daPJ7BWT7k */
					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */

					ImGui::BeginChild("A", "Weapon", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 200)); /* https://discord.gg/daPJ7BWT7k */
					{
						ImGui::Checkbox("Fast Reload", &g_options.misc.weapon.fastreload); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox("No Recoil", &g_options.misc.weapon.norecoil); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox("No Spread", &g_options.misc.weapon.nospread); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox("Double Shoot", &g_options.misc.weapon.doubleshoot); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox("Fire Ammo", &g_options.misc.weapon.fireammo); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox("Explo Ammo", &g_options.misc.weapon.exploammo); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox("Infinite Ammo", &g_options.misc.weapon.infiniteammo); /* https://discord.gg/daPJ7BWT7k */
					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */
				}
				ImGui::EndGroup(); /* https://discord.gg/daPJ7BWT7k */
			}
			if (tabs == 1)
			{


				ImGui::SetCursorPos(ImVec2(60 + tab_size, 60) + (s->ItemSpacing * 2)); /* https://discord.gg/daPJ7BWT7k */
				ImGui::BeginGroup(); /* https://discord.gg/daPJ7BWT7k */
				{
					ImGui::BeginChild("D", "MAIN", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 420)); /* https://discord.gg/daPJ7BWT7k */
					{

						ImVec2 p = ImGui::GetWindowPos(); /* https://discord.gg/daPJ7BWT7k */

						ImGui::CheckPicker("Box", "Box Color", &g_options.esp.player.box, g_options.esp.colors.boxcolor); /* https://discord.gg/daPJ7BWT7k */

						ImGui::CheckPicker("Skeleton", "Bone Color", &g_options.esp.player.skeleton, g_options.esp.colors.skeletoncolor); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("Health"), &g_options.esp.player.healthbar); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("Armor"), &g_options.esp.player.armorbar); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox("Player Tag", &Headcheckbox); /* https://discord.gg/daPJ7BWT7k */

						ImGui::CheckPicker("Weapon Tag", "Bone Color##2", &g_options.esp.player.weaponname, g_options.esp.colors.weaponnamecolor); /* https://discord.gg/daPJ7BWT7k */

						ImGui::CheckPicker("Distance", "Bone Color##2", &g_options.esp.player.distance, g_options.esp.colors.distancecolor); /* https://discord.gg/daPJ7BWT7k */

						ImGui::CheckPicker("Lines", "Bone Color##2", &g_options.esp.player.lines, g_options.esp.colors.linescolor); /* https://discord.gg/daPJ7BWT7k */

						ImGui::SliderInt("ESP Distance", &g_options.esp.player.renderingdistance, 0, 750); /* https://discord.gg/daPJ7BWT7k */


					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */
				}
				ImGui::EndGroup(); /* https://discord.gg/daPJ7BWT7k */
				ImGui::SameLine(); /* https://discord.gg/daPJ7BWT7k */

				ImGui::BeginGroup(); /* https://discord.gg/daPJ7BWT7k */
				{

					ImGui::BeginChild(" ", "Preview", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 420)); /* https://discord.gg/daPJ7BWT7k */
					{
						ImVec2 pos = ImGui::GetWindowPos(); /* https://discord.gg/daPJ7BWT7k */
						ImDrawList* draw = ImGui::GetWindowDrawList(); /* https://discord.gg/daPJ7BWT7k */
						if (Flogs[0])
						{
							//   ImGui::SetCursorPos(ImVec2(80, 45)); /* https://discord.gg/daPJ7BWT7k */
							ImGui::SetCursorPos(ImVec2(60, 65)); /* https://discord.gg/daPJ7BWT7k */
							ImVec2 pos1 = ImGui::GetCursorScreenPos(); /* https://discord.gg/daPJ7BWT7k */
							draw->AddCircle(ImVec2(pos1.x + 88, pos1.y - 20), hdtk + 1, ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 255)), 12, 0); /* https://discord.gg/daPJ7BWT7k */
							draw->AddCircleFilled(ImVec2(pos1.x + 88, pos1.y - 20), hdtk, ImGui::ColorConvertFloat4ToU32(ImVec4(255, 255, 255, 255)), 12); /* https://discord.gg/daPJ7BWT7k */

							draw->AddShadowCircle(ImVec2(pos1.x + 88, pos1.y - 20), hdtk, ImGui::ColorConvertFloat4ToU32(ImVec4(71, 226, 67, 255 / 2)), 50.f, ImVec2(0, 0), 0, 12); /* https://discord.gg/daPJ7BWT7k */

							//  draw->AddRect(ImVec2(pos1.x + 100, pos1.y - 20), ImVec2(pos1.x + 40, pos1.y + 30), ImColor(255,255,255), 0.0f, 0.0f, hdtk); /* https://discord.gg/daPJ7BWT7k */
						}
						if (g_options.esp.player.box)
						{
							ImGui::SetCursorPos(ImVec2(60, 20)); /* https://discord.gg/daPJ7BWT7k */
							ImVec2 pos1 = ImGui::GetCursorScreenPos(); /* https://discord.gg/daPJ7BWT7k */
							float box_width = 210.0f;
							float box_height = 370.0f;
							float rounding = 5.0f;
							float thickness = 2.0f;

							draw->AddRect(
								ImVec2(pos1.x, pos1.y),
								ImVec2(pos1.x + box_width, pos1.y + box_height),
								ImColor(255, 255, 255, 255),
								rounding,
								ImDrawFlags_RoundCornersAll,
								thickness
							); /* https://discord.gg/daPJ7BWT7k */

							draw->AddShadowRect(
								ImVec2(pos1.x, pos1.y),
								ImVec2(pos1.x + box_width, pos1.y + box_height),
								ImColor(0, 0, 0, 100),
								18.0f,
								ImVec2(0, 0),
								0,
								0
							); /* https://discord.gg/daPJ7BWT7k */
						}


						if (g_options.esp.player.healthbar)
						{
							ImGui::SetCursorPos(ImVec2(42, 15)); /* https://discord.gg/daPJ7BWT7k */
							ImVec2 pos1 = ImGui::GetCursorScreenPos(); /* https://discord.gg/daPJ7BWT7k */
							draw->AddRectFilled(ImVec2(pos1.x, pos1.y), ImVec2(pos1.x + hptk, pos1.y + 360), ImColor(24, 248, 24, 255), 0.0f); /* https://discord.gg/daPJ7BWT7k */
							draw->AddShadowRect(ImVec2(pos1.x, pos1.y), ImVec2(pos1.x + hptk, pos1.y + 360), ImColor(24, 248, 24, 255), 14.0f, ImVec2(0, 0), 0, 0); /* https://discord.gg/daPJ7BWT7k */
						}
						if (g_options.esp.player.armorbar)
						{
							ImGui::SetCursorPos(ImVec2(50, 375)); /* https://discord.gg/daPJ7BWT7k */
							ImVec2 pos1 = ImGui::GetCursorScreenPos(); /* https://discord.gg/daPJ7BWT7k */
							draw->AddRectFilled(ImVec2(pos1.x, pos1.y), ImVec2(pos1.x + 200, pos1.y + 5), ImColor(24, 24, 248, 255), 0.0f); /* https://discord.gg/daPJ7BWT7k */
							draw->AddShadowRect(ImVec2(pos1.x, pos1.y), ImVec2(pos1.x + 200, pos1.y + 5), ImColor(24, 24, 248, 255), 14.0f, ImVec2(0, 0), 0, 0); /* https://discord.gg/daPJ7BWT7k */
						}




						if (g_options.esp.player.name)
						{
							ImGui::SetCursorPos(ImVec2(112, 0)); /* https://discord.gg/daPJ7BWT7k */
							ImVec2 pos1 = ImGui::GetCursorScreenPos(); /* https://discord.gg/daPJ7BWT7k */
							draw->AddText(ImVec2(pos1.x, pos1.y), ImColor(255, 255, 255, 255), "PLAYER NAME"); /* https://discord.gg/daPJ7BWT7k */
						}
						if (g_options.esp.player.distance)
						{
							ImGui::SetCursorPos(ImVec2(254, 12)); /* https://discord.gg/daPJ7BWT7k */
							ImVec2 pos1 = ImGui::GetCursorScreenPos(); /* https://discord.gg/daPJ7BWT7k */
							draw->AddText(ImVec2(pos1.x, pos1.y), ImColor(255, 255, 255, 255), "34m"); /* https://discord.gg/daPJ7BWT7k */
						}
						if (g_options.esp.player.weaponname)
						{
							ImGui::SetCursorPos(ImVec2(254, 23)); /* https://discord.gg/daPJ7BWT7k */
							ImVec2 pos1 = ImGui::GetCursorScreenPos(); /* https://discord.gg/daPJ7BWT7k */
							draw->AddText(ImVec2(pos1.x, pos1.y), ImColor(255, 255, 255, 255), "Ap Pistol"); /* https://discord.gg/daPJ7BWT7k */
						}
						if (g_options.esp.player.skeleton)
						{
							ImGui::SetCursorPos(ImVec2(26, 74)); /* https://discord.gg/daPJ7BWT7k */
							ImVec2 pos1 = ImGui::GetCursorScreenPos(); /* https://discord.gg/daPJ7BWT7k */
							ImVec2 child_size = ImGui::GetWindowSize(); /* https://discord.gg/daPJ7BWT7k */
							ImDrawList* draw_list = ImGui::GetWindowDrawList(); /* https://discord.gg/daPJ7BWT7k */

							ImVec2 neck(122 + pos1.x, pos1.y); /* https://discord.gg/daPJ7BWT7k */

							ImVec2 epaule_droite(70 + pos1.x, 40 + pos1.y); /* https://discord.gg/daPJ7BWT7k */
							ImVec2 coude_droite(45 + pos1.x, 100 + pos1.y); /* https://discord.gg/daPJ7BWT7k */

							ImVec2 epaule_gauche(170 + pos1.x, 20 + pos1.y); /* https://discord.gg/daPJ7BWT7k */
							ImVec2 coude_gauche(210 + pos1.x, 100 + pos1.y); /* https://discord.gg/daPJ7BWT7k */

							ImVec2 bassin(121 + pos1.x, 110 + pos1.y); /* https://discord.gg/daPJ7BWT7k */

							ImVec2 anche_droite(80 + pos1.x, 207 + pos1.y); /* https://discord.gg/daPJ7BWT7k */
							ImVec2 genoux_droite(80 + pos1.x, 258 + pos1.y); /* https://discord.gg/daPJ7BWT7k */

							ImVec2 anche_gauche(175 + pos1.x, 210 + pos1.y); /* https://discord.gg/daPJ7BWT7k */
							ImVec2 genoux_gauche(175 + pos1.x, 261 + pos1.y); /* https://discord.gg/daPJ7BWT7k */

							draw_list->AddLine(neck, bassin, ImColor(255, 255, 255, 255), bonetk); /* https://discord.gg/daPJ7BWT7k */

							draw_list->AddLine(neck, epaule_droite, ImColor(255, 255, 255, 255), bonetk); /* https://discord.gg/daPJ7BWT7k */
							draw_list->AddLine(epaule_droite, coude_droite, ImColor(255, 255, 255, 255), bonetk); /* https://discord.gg/daPJ7BWT7k */

							draw_list->AddLine(neck, epaule_gauche, ImColor(255, 255, 255, 255), bonetk); /* https://discord.gg/daPJ7BWT7k */
							draw_list->AddLine(epaule_gauche, coude_gauche, ImColor(255, 255, 255, 255), bonetk); /* https://discord.gg/daPJ7BWT7k */

							draw_list->AddLine(bassin, anche_droite, ImColor(255, 255, 255, 255), bonetk); /* https://discord.gg/daPJ7BWT7k */
							draw_list->AddLine(anche_droite, genoux_droite, ImColor(255, 255, 255, 255), bonetk); /* https://discord.gg/daPJ7BWT7k */

							draw_list->AddLine(bassin, anche_gauche, ImColor(255, 255, 255, 255), bonetk); /* https://discord.gg/daPJ7BWT7k */
							draw_list->AddLine(anche_gauche, genoux_gauche, ImColor(255, 255, 255, 255), bonetk); /* https://discord.gg/daPJ7BWT7k */
						}
					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */
					ImGui::BeginChild("C", "Friends", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 80)); /* https://discord.gg/daPJ7BWT7k */
					{
						ImGui::ColorEdit4(("Friend Color"), g_options.esp.colors.friendcolor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs); /* https://discord.gg/daPJ7BWT7k */
					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */
					// ImGui::SetCursorPos(ImVec2(260,540)); /* https://discord.gg/daPJ7BWT7k */
					ImGui::SetCursorPos(ImVec2(60 + tab_size, 500) + (s->ItemSpacing * 2)); /* https://discord.gg/daPJ7BWT7k */
					ImGui::BeginChild("A", "Config", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 80)); /* https://discord.gg/daPJ7BWT7k */
					{
						ImGui::Checkbox(("Draw Local Player"), &g_options.esp.player.allowlocalplayer); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox(("Draw NPC"), &g_options.esp.player.allownpc); /* https://discord.gg/daPJ7BWT7k */
					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */
				}
				ImGui::EndGroup(); /* https://discord.gg/daPJ7BWT7k */
			}

			if (tabs == 2)
			{


				ImGui::SetCursorPos(ImVec2(60 + tab_size, 60) + (s->ItemSpacing * 2)); /* https://discord.gg/daPJ7BWT7k */
				ImGui::BeginGroup(); /* https://discord.gg/daPJ7BWT7k */
				{
					ImGui::BeginChild("D", "LocalPlayer", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 515)); /* https://discord.gg/daPJ7BWT7k */
					{

						ImVec2 p = ImGui::GetWindowPos(); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("God Mode"), &g_options.misc.localplayer.godmode); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("SemiGod Mode"), &g_options.misc.localplayer.SemiGod); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("Anti Hs"), &g_options.misc.localplayer.antiheadshot); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("Tiny Player"), &g_options.misc.localplayer.shrink); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("No Clip"), &g_options.misc.localplayer.noclip); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("Override Cam Distance"), &g_options.misc.localplayer.overridecamdistance); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("Super Jump"), &g_options.misc.localplayer.superjump); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("Beast Jump"), &g_options.misc.localplayer.beastjump); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("Beast Mode"), &g_options.misc.localplayer.beastmode); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("One Punch"), &g_options.misc.localplayer.onepunch); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("Explo Punch"), &g_options.misc.localplayer.explomeele); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("Invisible"), &g_options.misc.localplayer.Invisible); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("Steal Car"), &g_options.misc.localplayer.stealcar); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("Seat Belt"), &g_options.misc.localplayer.seatbelt); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("No Collision"), &g_options.misc.localplayer.nocollision); /* https://discord.gg/daPJ7BWT7k *///

						ImGui::Checkbox(("No Ragdoll"), &g_options.misc.localplayer.noragdoll); /* https://discord.gg/daPJ7BWT7k *///

						ImGui::Checkbox(("Reganerate Armor"), &g_options.misc.localplayer.ReganerateArmor); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("Infinite Stamina"), &g_options.misc.localplayer.infinitestamina); /* https://discord.gg/daPJ7BWT7k */

						ImGui::Checkbox(("Speed Walk"), &g_options.misc.localplayer.speedwalk); /* https://discord.gg/daPJ7BWT7k */

						if (ImGui::Button("Revive", button_size))
					    {
						     Cheat::Exploits::Revive(); /* https://discord.gg/daPJ7BWT7k */
					    }
				    	if (ImGui::Button("Suicide", button_size))
					    {
						     Cheat::Exploits::Suicide(); /* https://discord.gg/daPJ7BWT7k */
					    }
						if (ImGui::Button("Full Armor", button_size))
						{
							Cheat::Exploits::FullArmor(); /* https://discord.gg/daPJ7BWT7k */
						}

						if (ImGui::Button("Teleport To Waypoint", button_size))
						{
							Cheat::Exploits::TeleportToWaypoint(); /* https://discord.gg/daPJ7BWT7k */
						}

		


				
					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */
				}
				ImGui::EndGroup(); /* https://discord.gg/daPJ7BWT7k */
				ImGui::SameLine(); /* https://discord.gg/daPJ7BWT7k */

				ImGui::BeginGroup(); /* https://discord.gg/daPJ7BWT7k */
				{
					ImGui::BeginChild("C", "Settings", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 160)); /* https://discord.gg/daPJ7BWT7k */
					{
						ImGui::SliderInt(("No Clip Speed"), &g_options.misc.localplayer.noclipspeed, 0, 100); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Keybind("No Clip keybind", &g_options.misc.localplayer.noclipbind, true); /* https://discord.gg/daPJ7BWT7k */

						ImGui::SliderFloat(("Cam Distance"), &g_options.misc.localplayer.camdistance, 1.f, 100.f, "%.1f"); /* https://discord.gg/daPJ7BWT7k */
						ImGui::SliderFloat(("Speed Multiplier"), &g_options.misc.localplayer.speedwalkvalue, 0.5f, 3.f, "%.1f"); /* https://discord.gg/daPJ7BWT7k */
					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */


					auto* vehicle = Cheat::g_Fivem.GetLocalPlayerInfo().Ped->GetLastVehicle(); /* https://discord.gg/daPJ7BWT7k */

					ImGui::BeginChild("C", "Vehicle", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 345)); /* https://discord.gg/daPJ7BWT7k */
					{
						ImGui::Checkbox(("Godmode"), &g_options.misc.vehicle.godmode); /* https://discord.gg/daPJ7BWT7k */
						ImGui::Checkbox(("Rocket Boost"), &g_options.misc.vehicle.rocketboost); /* https://discord.gg/daPJ7BWT7k */
					
						ImGui::SliderFloat(("Acceleration"), &g_options.misc.vehicle.accelerationvalue, 0.f, 100.f, "%.1f"); /* https://discord.gg/daPJ7BWT7k */
						ImGui::SliderFloat(("Traction"), &g_options.misc.vehicle.tractionvalue, 0.f, 100.f, "%.1f"); /* https://discord.gg/daPJ7BWT7k */	
						if (ImGui::Button("Repair Vehicle", button_size))
											{
												if (Cheat::g_Fivem.GetLocalPlayerInfo().Ped->HasConfigFlag(CPED_CONFIG_FLAG_InVehicle))
												{
													if (Cheat::g_Fivem.GetLocalPlayerInfo().Ped->GetLastVehicle())
													{
														Cheat::g_Fivem.GetLocalPlayerInfo().Ped->GetLastVehicle()->FixVehicle(); /* https://discord.gg/daPJ7BWT7k */
													}
												}
											}
					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */
					// ImGui::SetCursorPos(ImVec2(260,540)); /* https://discord.gg/daPJ7BWT7k */

				}
				ImGui::EndGroup(); /* https://discord.gg/daPJ7BWT7k */
			}

			if (tabs == 3)
			{
			
				ImGui::Text("Player & Vehicle List Coming Soon"); /* https://discord.gg/daPJ7BWT7k */
			

				static Cheat::VehicleInfo SelectedVehicle = Cheat::VehicleInfo(); /* https://discord.gg/daPJ7BWT7k */
				if (SelectedVehicle.Vehicle->GetCoordinate().DistTo(Cheat::g_Fivem.GetLocalPlayerInfo().WorldPos) > 600)
					SelectedVehicle = Cheat::VehicleInfo(); /* https://discord.gg/daPJ7BWT7k */

				static Cheat::Entity SelectedEntity = Cheat::Entity(); /* https://discord.gg/daPJ7BWT7k */
				if (SelectedEntity.Cordinates.DistTo(Cheat::g_Fivem.GetLocalPlayerInfo().WorldPos) > 600)
					SelectedEntity = Cheat::Entity(); /* https://discord.gg/daPJ7BWT7k */

				// Adjust the position and start the group
				ImGui::SetCursorPos(ImVec2(60 + tab_size, 60) + (s->ItemSpacing * 2)); /* https://discord.gg/daPJ7BWT7k */
				ImGui::BeginGroup(); /* https://discord.gg/daPJ7BWT7k */
				{
					// Begin Child for Vehicle List
					ImGui::BeginChild("VehicleList", "Vehicle List", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 250)); /* https://discord.gg/daPJ7BWT7k */
					{
						if (ImGui::BeginListBox("##VehList", ImVec2(ImGui::GetWindowSize().x - 20, 230)))
						{
							if (Cheat::g_Fivem.GetLocalPlayerInfo().Ped)
							{
								for (Cheat::VehicleInfo Current : Cheat::g_Fivem.GetVehicleList())
								{
									std::string Text = Current.Name + (" (") + std::to_string((int)(Current.Vehicle->GetCoordinate().DistTo(Cheat::g_Fivem.GetLocalPlayerInfo().WorldPos))) + ("m)"); /* https://discord.gg/daPJ7BWT7k */
									bool Selected = (Current.Vehicle == SelectedVehicle.Vehicle); /* https://discord.gg/daPJ7BWT7k */

									if (ImGui::Selectable(Text.c_str(), &Selected))
									{
										SelectedVehicle = Current;
									}
								}
							}
							ImGui::EndListBox(); /* https://discord.gg/daPJ7BWT7k */
						}
					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */

					// Begin Child for Player List
					ImGui::SameLine(); /* https://discord.gg/daPJ7BWT7k */
					ImGui::BeginChild("PlayerList", "Player List", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 250)); /* https://discord.gg/daPJ7BWT7k */
					{
						if (ImGui::BeginListBox("##PlayerList", ImVec2(ImGui::GetWindowSize().x - 20, 230)))
						{
							if (Cheat::g_Fivem.GetLocalPlayerInfo().Ped)
							{
								for (Cheat::Entity Current : Cheat::g_Fivem.GetEntitiyList())
								{
									if (Current.StaticInfo.bIsNPC || !Current.StaticInfo.Ped || Current.StaticInfo.bIsLocalPlayer)
										continue;

									ImGui::PushID(Current.StaticInfo.iIndex); /* https://discord.gg/daPJ7BWT7k */
									{
										std::string Text = Current.StaticInfo.Name + (" (") + std::to_string((int)(Current.Cordinates.DistTo(Cheat::g_Fivem.GetLocalPlayerInfo().WorldPos))) + ("m)"); /* https://discord.gg/daPJ7BWT7k */

										if (Current.StaticInfo.IsFriend)
											Text += (" (friend)"); /* https://discord.gg/daPJ7BWT7k */

										bool Selected = (Current.StaticInfo.Ped == SelectedEntity.StaticInfo.Ped); /* https://discord.gg/daPJ7BWT7k */

										if (ImGui::Selectable(Text.c_str(), &Selected))
										{
											SelectedEntity = Current;
										}
									}
									ImGui::PopID(); /* https://discord.gg/daPJ7BWT7k */
								}
							}
							ImGui::EndListBox(); /* https://discord.gg/daPJ7BWT7k */
						}
					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */
				}
				ImGui::EndGroup(); /* https://discord.gg/daPJ7BWT7k */

				ImGui::SameLine(); /* https://discord.gg/daPJ7BWT7k */

				ImGui::BeginGroup(); /* https://discord.gg/daPJ7BWT7k */
				{
					// Vehicle Actions Child
					ImGui::BeginChild("VehicleActions", "Vehicle Actions", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 250)); /* https://discord.gg/daPJ7BWT7k */
					{
						if (SelectedVehicle.iIndex != -1)
						{
							if (ImGui::Button("Teleport to vehicle"))
							{
								Cheat::g_Fivem.TeleportObject((uintptr_t)Cheat::g_Fivem.GetLocalPlayerInfo().Ped,
									Cheat::g_Fivem.GetLocalPlayerInfo().Ped->GetNavigation(),
									Cheat::g_Fivem.GetLocalPlayerInfo().Ped->GetModelInfo(),
									SelectedVehicle.Vehicle->GetCoordinate(),
									SelectedVehicle.Vehicle->GetCoordinate(),
									true); /* https://discord.gg/daPJ7BWT7k */
							}
							if (ImGui::Button("Unlock vehicle"))
							{
								SelectedVehicle.Vehicle->SetDoorLock(1); /* https://discord.gg/daPJ7BWT7k */
							}
							if (ImGui::Button("Lock vehicle"))
							{
								SelectedVehicle.Vehicle->SetDoorLock(2); /* https://discord.gg/daPJ7BWT7k */
							}
							if (ImGui::Button("Unlock Closest Vehicle"))
							{
								Cheat::Exploits::UnlockClosestVehicle(); /* https://discord.gg/daPJ7BWT7k */
							}
						}
						else
						{
							ImGui::Text("Select a vehicle first!"); /* https://discord.gg/daPJ7BWT7k */
						}
					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */

					// Player Actions Child
					ImGui::SameLine(); /* https://discord.gg/daPJ7BWT7k */
					ImGui::BeginChild("PlayerActions", "Player Actions", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 250)); /* https://discord.gg/daPJ7BWT7k */
					{
						if (SelectedEntity.StaticInfo.iIndex > 0)
						{
							if (ImGui::Button("Teleport to player"))
							{
								Cheat::g_Fivem.TeleportObject((uintptr_t)Cheat::g_Fivem.GetLocalPlayerInfo().Ped,
									Cheat::g_Fivem.GetLocalPlayerInfo().Ped->GetNavigation(),
									Cheat::g_Fivem.GetLocalPlayerInfo().Ped->GetModelInfo(),
									SelectedEntity.Cordinates,
									SelectedEntity.Cordinates,
									true); /* https://discord.gg/daPJ7BWT7k */
							}

							if (ImGui::Button("Spectate Player"))
							{
								Cheat::g_Fivem.SpectatePed((uint64_t)SelectedEntity.StaticInfo.Ped, true); /* https://discord.gg/daPJ7BWT7k */
								g_options.misc.localplayer.bIsSpectatingEntity = true;
								g_options.misc.localplayer.SpectatingEntity = (uint64_t)SelectedEntity.StaticInfo.Ped;
							}

							if (ImGui::Button("Steal Outfit"))
							{
								Cheat::CPed* ped = SelectedEntity.StaticInfo.Ped;

								if (ped)
								{
									uintptr_t drawhandler = FrameWork::Memory::ReadMemory<uintptr_t>(reinterpret_cast<uintptr_t>(ped) + 0x48); /* https://discord.gg/daPJ7BWT7k */

									BYTE shoes = FrameWork::Memory::ReadMemory<BYTE>(drawhandler + 0x100); /* https://discord.gg/daPJ7BWT7k */
									BYTE leg = FrameWork::Memory::ReadMemory<BYTE>(drawhandler + 0xF8); /* https://discord.gg/daPJ7BWT7k */
									BYTE body = FrameWork::Memory::ReadMemory<BYTE>(drawhandler + 0x114); /* https://discord.gg/daPJ7BWT7k */
									BYTE tshirt = FrameWork::Memory::ReadMemory<BYTE>(drawhandler + 0x108); /* https://discord.gg/daPJ7BWT7k */
									BYTE mask = FrameWork::Memory::ReadMemory<BYTE>(drawhandler + 0xEC); /* https://discord.gg/daPJ7BWT7k */
									BYTE hand = FrameWork::Memory::ReadMemory<BYTE>(drawhandler + 0xF4); /* https://discord.gg/daPJ7BWT7k */
									BYTE bag = FrameWork::Memory::ReadMemory<BYTE>(drawhandler + 0xFC); /* https://discord.gg/daPJ7BWT7k */
									BYTE armor = FrameWork::Memory::ReadMemory<BYTE>(drawhandler + 0x10C); /* https://discord.gg/daPJ7BWT7k */

									uintptr_t local_drawhandler = FrameWork::Memory::ReadMemory<uintptr_t>(reinterpret_cast<uintptr_t>(Cheat::g_Fivem.GetLocalPlayerInfo().Ped) + 0x48); /* https://discord.gg/daPJ7BWT7k */

									FrameWork::Memory::WriteMemory<BYTE>(local_drawhandler + 0x100, shoes); /* https://discord.gg/daPJ7BWT7k */
									FrameWork::Memory::WriteMemory<BYTE>(local_drawhandler + 0xF8, leg); /* https://discord.gg/daPJ7BWT7k */
									FrameWork::Memory::WriteMemory<BYTE>(local_drawhandler + 0x114, body); /* https://discord.gg/daPJ7BWT7k */
									FrameWork::Memory::WriteMemory<BYTE>(local_drawhandler + 0x108, tshirt); /* https://discord.gg/daPJ7BWT7k */
									FrameWork::Memory::WriteMemory<BYTE>(local_drawhandler + 0xEC, mask); /* https://discord.gg/daPJ7BWT7k */
									FrameWork::Memory::WriteMemory<BYTE>(local_drawhandler + 0xF4, hand); /* https://discord.gg/daPJ7BWT7k */
									FrameWork::Memory::WriteMemory<BYTE>(local_drawhandler + 0xFC, bag); /* https://discord.gg/daPJ7BWT7k */
									FrameWork::Memory::WriteMemory<BYTE>(local_drawhandler + 0x10C, armor); /* https://discord.gg/daPJ7BWT7k */
									FrameWork::Memory::WriteMemory<BYTE>(local_drawhandler + 0xAC, 0.f); /* https://discord.gg/daPJ7BWT7k */
								}
							}

							if (!SelectedEntity.StaticInfo.IsFriend)
							{
								if (ImGui::Button("Add friend"))
								{
									if (!Cheat::g_Fivem.AllEntitesList[SelectedEntity.StaticInfo.Ped].IsFriend || !SelectedEntity.StaticInfo.IsFriend)
										Cheat::g_Fivem.FriendList[SelectedEntity.StaticInfo.Ped] = SelectedEntity.StaticInfo;

									Cheat::g_Fivem.AllEntitesList[SelectedEntity.StaticInfo.Ped].IsFriend = true;
									SelectedEntity.StaticInfo.IsFriend = true;
								}
							}
							else
							{
								if (ImGui::Button("Remove friend"))
								{
									if (Cheat::g_Fivem.AllEntitesList[SelectedEntity.StaticInfo.Ped].IsFriend || SelectedEntity.StaticInfo.IsFriend)
										Cheat::g_Fivem.FriendList.erase(SelectedEntity.StaticInfo.Ped); /* https://discord.gg/daPJ7BWT7k */

									Cheat::g_Fivem.AllEntitesList[SelectedEntity.StaticInfo.Ped].IsFriend = false;
									SelectedEntity.StaticInfo.IsFriend = false;
								}
							}
						}
						else
						{
							ImGui::Text("Select a player first!"); /* https://discord.gg/daPJ7BWT7k */
						}
					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */
				}
				ImGui::EndGroup(); /* https://discord.gg/daPJ7BWT7k */
			}



			if (tabs == 5)
			{
				ImGui::SetCursorPos(ImVec2(60 + tab_size, 60) + (s->ItemSpacing * 2)); /* https://discord.gg/daPJ7BWT7k */
				ImGui::BeginGroup(); /* https://discord.gg/daPJ7BWT7k */
				{
					ImGui::BeginChild("D", "Online Chat -> Online users:48", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) + 18, 520)); /* https://discord.gg/daPJ7BWT7k */
					{

					}
					ImGui::EndChild(); /* https://discord.gg/daPJ7BWT7k */
				}
				ImGui::EndGroup(); /* https://discord.gg/daPJ7BWT7k */
			}

			ImGui::PopStyleVar(); /* https://discord.gg/daPJ7BWT7k */
		}
		ImGui::End(); /* https://discord.gg/daPJ7BWT7k */
	}

	void Interface::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SIZE:
			if (wParam != SIZE_MINIMIZED)
			{
				ResizeWidht = (UINT)LOWORD(lParam); /* https://discord.gg/daPJ7BWT7k */
				ResizeHeight = (UINT)HIWORD(lParam); /* https://discord.gg/daPJ7BWT7k */
			}
			break;
		}

		if (bIsMenuOpen)
		{
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam); /* https://discord.gg/daPJ7BWT7k */
		}
	}

	void Interface::HandleMenuKey()
	{
		static bool MenuKeyDown = false;
		if (GetAsyncKeyState(g_options.menu.menubind) & 0x8000)
		{
			if (!MenuKeyDown)
			{
				MenuKeyDown = true;
				bIsMenuOpen = !bIsMenuOpen;

				if (bIsMenuOpen)
				{
					SetWindowLong(hWindow, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE); /* https://discord.gg/daPJ7BWT7k */
					SetForegroundWindow(hWindow); /* https://discord.gg/daPJ7BWT7k */
				}
				else
				{
					SetWindowLong(hWindow, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_NOACTIVATE); /* https://discord.gg/daPJ7BWT7k */
					SetForegroundWindow(hTargetWindow); /* https://discord.gg/daPJ7BWT7k */
				}
			}
		}
		else
		{
			MenuKeyDown = false;
		}
	}

	void Interface::ShutDown()
	{
		SafeCall(ExitProcess)(0); /* https://discord.gg/daPJ7BWT7k */
	}
}